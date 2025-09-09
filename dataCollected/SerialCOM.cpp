#include "SerialCOM.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/MinWindows.h"
#include "Windows/HideWindowsPlatformTypes.h"

#define BOOL2bool(B) B == 0 ? false : true

// =====================================================================
// -------- BYTE / FLOAT / INT UTILS (Static) --------------------------
// =====================================================================

int32 ASerialCOM::BytesToInt(TArray<uint8> Bytes)
{
	if (Bytes.Num() != 4)
	{
		return 0;
	}
	return *reinterpret_cast<int32*>(Bytes.GetData());
}

TArray<uint8> ASerialCOM::IntToBytes(const int32& Int)
{
	TArray<uint8> Bytes;
	Bytes.Append(reinterpret_cast<const uint8*>(&Int), sizeof(int32));
	return Bytes;
}

float ASerialCOM::BytesToFloat(TArray<uint8> Bytes)
{
	if (Bytes.Num() != 4)
	{
		return 0.f;
	}
	return *reinterpret_cast<float*>(Bytes.GetData());
}

TArray<uint8> ASerialCOM::FloatToBytes(const float& Float)
{
	TArray<uint8> Bytes;
	Bytes.Append(reinterpret_cast<const uint8*>(&Float), sizeof(float));
	return Bytes;
}

// =====================================================================
// -------- CONSTRUTOR / LIFECYCLE -------------------------------------
// =====================================================================

ASerialCOM::ASerialCOM()
	: WriteLineEnd(ELineEnd::n)
	, m_hIDComDev(nullptr)
	, m_Port(-1)
	, m_Baud(-1)
{
	PrimaryActorTick.bCanEverTick = true;

	// Limpa as structs OVERLAPPED. Não precisa mais de 'new'.
	FMemory::Memzero(&m_OverlappedRead, sizeof(OVERLAPPED));
	FMemory::Memzero(&m_OverlappedWrite, sizeof(OVERLAPPED));
}

void ASerialCOM::BeginPlay()
{
	Super::BeginPlay();
}

void ASerialCOM::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// Garante que a porta seja fechada quando o ator for destruído
	Close();
}

void ASerialCOM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// =====================================================================
// -------- ABRIR / FECHAR PORTA ---------------------------------------
// =====================================================================

bool ASerialCOM::Open(int32 Port, int32 BaudRate)
{
	return OpenWithFlowControl(Port, BaudRate, true, true);
}

bool ASerialCOM::OpenWithFlowControl(int32 nPort, int32 nBaud, bool bDTR, bool bRTS)
{
	if (IsOpened())
	{
		UE_LOG(LogTemp, Warning, TEXT("Serial port COM%d is already open. Close it before opening a new one."), m_Port);
		return false;
	}
	if (nPort < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid port number: %d"), nPort);
		return false;
	}

	FString szPort = (nPort < 10) ? FString::Printf(TEXT("COM%d"), nPort)
		: FString::Printf(TEXT("\\\\.\\COM%d"), nPort);

	m_hIDComDev = CreateFile(*szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if (m_hIDComDev == NULL || m_hIDComDev == INVALID_HANDLE_VALUE)
	{
		unsigned long dwError = GetLastError();
		UE_LOG(LogTemp, Error, TEXT("Failed to open port COM%d (%s). Error Code: %lu"), nPort, *szPort, dwError);
		m_hIDComDev = nullptr;
		return false;
	}

	FMemory::Memzero(&m_OverlappedRead, sizeof(OVERLAPPED));
	FMemory::Memzero(&m_OverlappedWrite, sizeof(OVERLAPPED));

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 10;
	if (!SetCommTimeouts(m_hIDComDev, &CommTimeOuts))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set COM timeouts for COM%d."), nPort);
		Close();
		return false;
	}

	m_OverlappedRead.hEvent = CreateEvent(NULL, true, false, NULL);
	m_OverlappedWrite.hEvent = CreateEvent(NULL, true, false, NULL);

	DCB dcb;
	SecureZeroMemory(&dcb, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	if (!GetCommState(m_hIDComDev, &dcb))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get COM state for COM%d."), nPort);
		Close();
		return false;
	}

	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fDtrControl = bDTR ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
	dcb.fRtsControl = bRTS ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;

	if (!SetCommState(m_hIDComDev, &dcb) ||
		!SetupComm(m_hIDComDev, 10000, 10000) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL)
	{
		unsigned long dwError = GetLastError();
		UE_LOG(LogTemp, Error, TEXT("Failed to setup port COM%d. Error Code: %lu"), nPort, dwError);
		Close();
		return false;
	}

	m_Port = nPort;
	m_Baud = nBaud;
	UE_LOG(LogTemp, Log, TEXT("Serial Port COM%d opened successfully at %d baud."), m_Port, m_Baud);
	return true;
}

void ASerialCOM::Close()
{
	if (!IsOpened()) return;

	if (m_OverlappedRead.hEvent != NULL) CloseHandle(m_OverlappedRead.hEvent);
	if (m_OverlappedWrite.hEvent != NULL) CloseHandle(m_OverlappedWrite.hEvent);
	CloseHandle(m_hIDComDev);

	m_hIDComDev = nullptr;
	m_Port = -1;
	m_Baud = -1;

	// Limpa as structs para reutilização segura
	FMemory::Memzero(&m_OverlappedRead, sizeof(OVERLAPPED));
	FMemory::Memzero(&m_OverlappedWrite, sizeof(OVERLAPPED));

	UE_LOG(LogTemp, Log, TEXT("Serial Port closed."));
}

// =====================================================================
// -------- READ -------------------------------------------------------
// =====================================================================

FString ASerialCOM::ReadString(bool& bSuccess)
{
	return ReadStringUntil(bSuccess, '\0');
}

FString ASerialCOM::Readln(bool& bSuccess)
{
	return ReadStringUntil(bSuccess, '\n');
}

FString ASerialCOM::ReadStringUntil(bool& bSuccess, uint8 Terminator)
{
	bSuccess = false;
	if (!IsOpened()) return TEXT("");

	TArray<uint8> Chars;
	uint8 Byte = 0x0;
	bool bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);
	if (!ComStat.cbInQue) return TEXT("");

	do {
		bReadStatus = BOOL2bool(ReadFile(
			m_hIDComDev,
			&Byte,
			1,
			&dwBytesRead,
			&m_OverlappedRead)); // Passando o endereço

		if (!bReadStatus)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, true);
			}
			else
			{
				Chars.Add(0x0); // Adiciona terminador nulo em caso de erro
				break;
			}
		}

		if (dwBytesRead > 0)
		{
			if (Byte == Terminator)
			{
				if (Chars.Num() > 0 && Terminator == '\n' && Chars.Last() == '\r') Chars.Pop(false);
				break;
			}
			Chars.Add(Byte);
		}

	} while (dwBytesRead > 0 && ComStat.cbInQue > 0);

	Chars.Add(0x0); // Adiciona terminador nulo para a string
	bSuccess = true;
	return FString(FUTF8ToTCHAR((const ANSICHAR*)Chars.GetData()).Get());
}

float ASerialCOM::ReadFloat(bool& bSuccess)
{
	bSuccess = false;
	TArray<uint8> Bytes = ReadBytes(sizeof(float));
	if (Bytes.Num() != sizeof(float)) return 0.f;

	bSuccess = true;
	return BytesToFloat(Bytes);
}

int32 ASerialCOM::ReadInt(bool& bSuccess)
{
	bSuccess = false;
	TArray<uint8> Bytes = ReadBytes(sizeof(int32));
	if (Bytes.Num() != sizeof(int32)) return 0;

	bSuccess = true;
	return BytesToInt(Bytes);
}

uint8 ASerialCOM::ReadByte(bool& bSuccess)
{
	bSuccess = false;
	if (!IsOpened()) return 0x0;

	uint8 Byte = 0x0;
	bool bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);
	if (!ComStat.cbInQue) return 0x0;

	bReadStatus = BOOL2bool(ReadFile(
		m_hIDComDev,
		&Byte,
		1,
		&dwBytesRead,
		&m_OverlappedRead)); // Passando o endereço

	if (!bReadStatus)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, true);
		}
		else
		{
			return 0x0;
		}
	}

	bSuccess = dwBytesRead > 0;
	return Byte;
}

TArray<uint8> ASerialCOM::ReadBytes(int32 Limit)
{
	TArray<uint8> Data;
	if (!IsOpened() || Limit <= 0) return Data;

	DWORD dwBytesRead = 0, dwErrorFlags = 0;
	COMSTAT ComStat;
	ClearCommError(m_hIDComDev, &dwErrorFlags, &ComStat);

	if (!ComStat.cbInQue) return Data;

	DWORD BytesToRead = FMath::Min((DWORD)Limit, ComStat.cbInQue);
	Data.SetNumUninitialized(BytesToRead);

	if (!ReadFile(m_hIDComDev, Data.GetData(), BytesToRead, &dwBytesRead, &m_OverlappedRead))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, true);
		}
		else
		{
			dwBytesRead = 0;
		}
	}

	Data.SetNum(dwBytesRead);
	return Data;
}

// =====================================================================
// -------- WRITE ------------------------------------------------------
// =====================================================================

bool ASerialCOM::Print(FString String)
{
	if (String.IsEmpty()) return true;
	FTCHARToUTF8 Converter(*String);
	// O buffer precisa existir enquanto a escrita acontece
	TArray<uint8> Data;
	Data.Append((uint8*)Converter.Get(), Converter.Length());
	return WriteBytes(Data);
}

bool ASerialCOM::Println(FString String)
{
	return Print(String + LineEndToStr(WriteLineEnd));
}

bool ASerialCOM::WriteFloat(float Value)
{
	return WriteBytes(FloatToBytes(Value));
}

bool ASerialCOM::WriteInt(int32 Value)
{
	return WriteBytes(IntToBytes(Value));
}

bool ASerialCOM::WriteByte(uint8 Value)
{
	TArray<uint8> Buffer({ Value });
	return WriteBytes(Buffer);
}

bool ASerialCOM::WriteBytes(TArray<uint8> Buffer)
{
	if (!IsOpened() || Buffer.Num() == 0) return false;

	DWORD dwBytesWritten = 0;
	if (!WriteFile(m_hIDComDev, Buffer.GetData(), Buffer.Num(), &dwBytesWritten, &m_OverlappedWrite))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			if (GetOverlappedResult(m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, true))
			{
				return dwBytesWritten == Buffer.Num();
			}
		}
		// Se falhou por outro motivo
		return false;
	}
	return true;
}

void ASerialCOM::Flush()
{
	if (!IsOpened()) return;
	PurgeComm(m_hIDComDev, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

// =====================================================================
// -------- UTILS ------------------------------------------------------
// =====================================================================

FString ASerialCOM::LineEndToStr(ELineEnd LineEnd)
{
	switch (LineEnd)
	{
	case ELineEnd::rn:
		return TEXT("\r\n");
	case ELineEnd::n:
		return TEXT("\n");
	case ELineEnd::r:
		return TEXT("\r");
	case ELineEnd::nr:
		return TEXT("\n\r");
	default:
		return TEXT("");
	}
}