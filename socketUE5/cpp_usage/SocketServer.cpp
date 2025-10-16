// Fill out your copyright notice in the Description page of Project Settings.


#include "TCPCon.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Endpoint.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"

ATCPCon::ATCPCon()
{
    PrimaryActorTick.bCanEverTick = false;

    // Enable replication if needed
    // bReplicates = true;
}

void ATCPCon::BeginPlay()
{
    Super::BeginPlay();
    StartTCPReceiver();
}

void ATCPCon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    bShouldListen = false;

    if (ListenerSocket)
    {
        ListenerSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
    }

    if (ConnectionSocket)
    {
        ConnectionSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
    }
}

void ATCPCon::SendMessage(const FString& Message)
{
    if (!ConnectionSocket)
    {
        UE_LOG(LogTemp, Warning, TEXT("No connection to send message"));
        return;
    }

    FTCHARToUTF8 Convert(*Message);
    int32 BytesSent = 0;
    ConnectionSocket->Send((uint8*)Convert.Get(), Convert.Length(), BytesSent);

    if (BytesSent <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to send message"));
    }
}

void ATCPCon::StartTCPReceiver()
{
    ListenerSocket = CreateTCPConnectionListener();

    if (ListenerSocket)
    {
        GetWorldTimerManager().SetTimer(TCPConnectionListenerTimerHandle, this, &ATCPCon::TCPSocketListener, 0.01, true);
        UE_LOG(LogTemp, Log, TEXT("TCP Listener started on %s:%d"), *ListenAddress, Port);
    }
}

FSocket* ATCPCon::CreateTCPConnectionListener()
{
    FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCPListener"), false);

    FIPv4Address IPAddress;
    if (!FIPv4Address::Parse(ListenAddress, IPAddress))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid IP address: %s"), *ListenAddress);
        return nullptr;
    }

    FIPv4Endpoint Endpoint(IPAddress, Port);

    if (!Socket->Bind(*Endpoint.ToInternetAddr()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to bind socket"));
        return nullptr;
    }

    if (!Socket->Listen(1))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to listen on socket"));
        return nullptr;
    }

    return Socket;
}

void ATCPCon::ReadTCPData()
{
    if (!ConnectionSocket) return;

    uint32 Size;
    while (ConnectionSocket->HasPendingData(Size))
    {
        TArray<uint8> ReceivedData;
        ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));

        int32 Read = 0;
        if (ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read))
        {
            if (Read > 0)
            {
                ReceivedData.Add(0); // Add null terminator
                LastReceivedMessage = FString(UTF8_TO_TCHAR(ReceivedData.GetData()));
                UE_LOG(LogTemp, Log, TEXT("[TCP] Received: %s"), *LastReceivedMessage);
                OnMessageReceived.Broadcast(LastReceivedMessage);

            }
        }
    }
}


void ATCPCon::TCPSocketListener()
{
    if (!bShouldListen || !ListenerSocket) return;

    TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool Pending;

    if (ListenerSocket->HasPendingConnection(Pending) && Pending)
    {
        ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("TCP Received Socket"));

        if (ConnectionSocket)
        {
            UE_LOG(LogTemp, Log, TEXT("Accepted connection from %s"), *RemoteAddress->ToString(true));

            // Inicia leitura contínua com timer
            GetWorldTimerManager().SetTimer(ReadSocketTimerHandle, this, &ATCPCon::ReadTCPData, 0.05f, true);
        }
    }
}

