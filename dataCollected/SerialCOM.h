// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/MinWindows.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include "SerialCOM.generated.h"

UENUM(BlueprintType, Category = "Communication Serial")
enum class ELineEnd : uint8
{
	rn	UMETA(DisplayName = "\r\n"),
	n	UMETA(DisplayName = "\n"),
	r	UMETA(DisplayName = "\r"),
	nr	UMETA(DisplayName = "\n\r")
};

UCLASS(Blueprintable, Category = "Communication Serial", meta = (Keywords = "com arduino serial arduino duino"))
class SERIAL_TEST_API ASerialCOM : public AActor
{
	GENERATED_BODY()

public:
	/** Determines the line ending used when writing lines to serial port with PrintLine. */
	UPROPERTY(BlueprintReadWrite, Category = "Communication Serial | String")
	ELineEnd WriteLineEnd;

public:
	ASerialCOM();
	virtual ~ASerialCOM() = default;

	/** Chamado quando o jogo começa (para Actor). */
	virtual void BeginPlay() override;

	/** Chamado quando o ator é destruído ou o jogo termina. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Chamado a cada frame, caso queira polling serial. */
	virtual void Tick(float DeltaSeconds) override;

	// ----------- MÉTODOS DE INSTÂNCIA PARA ABRIR A PORTA (CORRIGIDO) ----------
	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Open Serial Port", Keywords = "communication com serial open port"))
	bool Open(int32 Port = 1, int32 BaudRate = 9600);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Open Serial Port With Flow Control", Keywords = "communication com serial open port flow control"))
	bool OpenWithFlowControl(int32 Port = 1, int32 BaudRate = 9600, bool DTR = true, bool RTS = true);

	// ----------- STATIC HELPERS (Correto manter como static) ----------
	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Serial Bytes to Int", Keywords = "communication com SERIALCOM duino arduino serial cast concatenate group bit bitwise bytes to int"))
	static int32 BytesToInt(TArray<uint8> Bytes);

	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Serial Int to Bytes", Keywords = "communication com SERIALCOM duino arduino serial cast separate bit bitwise int to bytes"))
	static TArray<uint8> IntToBytes(const int32& Int);

	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Serial Bytes to Float", Keywords = "communication com SERIALCOM duino arduino serial cast concatenate group bit bitwise bytes to float"))
	static float BytesToFloat(TArray<uint8> Bytes);

	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Serial Float to Bytes", Keywords = "communication com SERIALCOM duino arduino serial cast separate bit bitwise flowat to bytes"))
	static TArray<uint8> FloatToBytes(const float& Float);

	// ----------- MÉTODOS DE INSTÂNCIA (originais) ----------
	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Close Serial Port", Keywords = "communication com SERIALCOM duino arduino serial end finish release close port"))
	void Close();

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Read String", Keywords = "read string communication com SERIALCOM duino arduino serial get read receive string words text characters"))
	FString ReadString(bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Read Line", Keywords = "communication com SERIALCOM duino arduino serial read line get read receive string words text characters"))
	FString Readln(bool& bSuccess);

	FString ReadStringUntil(bool& bSuccess, uint8 Terminator);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Read Float", Keywords = "communication com SERIALCOM duino arduino serial read a float get read receive"))
	float ReadFloat(bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Read Int", Keywords = "communication com SERIALCOM duino arduino serial read an int get read receive integer"))
	int32 ReadInt(bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Read Byte", Keywords = "communication com SERIALCOM duino arduino serial read a byte get read receive"))
	uint8 ReadByte(bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Read Bytes", Keywords = "communication com SERIALCOM duino arduino serial read bytes get read receive"))
	TArray<uint8> ReadBytes(int32 Limit = 256);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Print", Keywords = "communication com SERIALCOM duino arduino serial print send write string words text characters"))
	bool Print(FString String);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Print Line", Keywords = "communication com SERIALCOM duino arduino serial print line send write string words text characters"))
	bool Println(FString String);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Write Float", Keywords = "communication com SERIALCOM duino arduino serial write a float send"))
	bool WriteFloat(float Value);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Write Int", Keywords = "communication com SERIALCOM duino arduino serial write an int integer send"))
	bool WriteInt(int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Write Byte", Keywords = "communication com SERIALCOM duino arduino serial write a byte send"))
	bool WriteByte(uint8 Value);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Write Bytes", Keywords = "communication com SERIALCOM duino arduino serial write bytes send"))
	bool WriteBytes(TArray<uint8> Buffer);

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Flush Serial Port"))
	void Flush();

	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Is Serial Port Open?"))
	bool IsOpened() const { return m_hIDComDev != nullptr; }

	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Get Serial Port Number"))
	int32 GetPort() const { return m_Port; }

	UFUNCTION(BlueprintPure, Category = "Communication Serial", meta = (DisplayName = "Get Serial Port Baud Rate"))
	int32 GetBaud() const { return m_Baud; }

	UFUNCTION(BlueprintCallable, Category = "Communication Serial", meta = (DisplayName = "Serial Line End to String", Keywords = "communication com SERIALCOM duino arduino serial cast convert line end to string"))
	FString LineEndToStr(ELineEnd LineEnd);


protected:
	void* m_hIDComDev;

	OVERLAPPED m_OverlappedRead;
	OVERLAPPED m_OverlappedWrite;

	int32 m_Port;
	int32 m_Baud;
};