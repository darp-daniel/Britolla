#include "SocketServer.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include "Runtime/Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Runtime/Sockets/Public/SocketSubsystem.h"

ASocketListener::ASocketListener()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // Enable replication if needed
    // bReplicates = true;
}

void ASocketListener::BeginPlay()
{
    Super::BeginPlay();
    StartTCPReceiver();
}

void ASocketListener::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void ASocketListener::SendMessage(const FString& Message)
{
    if (!ConnectionSocket)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sem conexão impossível enviar mensagem"));
        return;
    }

    FTCHARToUTF8 Convert(*Message);
    int32 BytesSent = 0;
    ConnectionSocket->Send((uint8*)Convert.Get(), Convert.Length(), BytesSent);
    
    if (BytesSent <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failha ao enviar mensagem: %s"), *Message);
    }
}

void ASocketListener::StartTCPReceiver()
{
    ListenerSocket = CreateTCPConnectionListener();
    
    if (ListenerSocket)
    {
        GetWorldTimerManager().SetTimer(TCPConnectionListenerTimerHandle, this, &ASocketListener::TCPSocketListener, 0.01, true);
        UE_LOG(LogTemp, Log, TEXT("TCP Listener %s:%d"), *ListenAddress, Port);
    }
}

FSocket* ASocketListener::CreateTCPConnectionListener()
{
    FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCPListener"), false);
    
    FIPv4Address IPAddress;
    if (!FIPv4Address::Parse(ListenAddress, IPAddress))
    {
        UE_LOG(LogTemp, Error, TEXT("IP Inválido %s"), *ListenAddress);
        return nullptr;
    }
    
    FIPv4Endpoint Endpoint(IPAddress, Port);
    
    if (!Socket->Bind(*Endpoint.ToInternetAddr()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to bind socket to %s:%d"), *ListenAddress, Port);
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return nullptr;
    }
    
    if (!Socket->Listen(1))
    {
        UE_LOG(LogTemp, Error, TEXT("Inviável escuta"));
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return nullptr;
    }
    
    return Socket;
}

void ASocketListener::TCPSocketListener()
{
    if (!bShouldListen || !ListenerSocket) return;
    
    TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool Pending;
    
    if (ListenerSocket->HasPendingConnection(Pending) && Pending)
    {
        ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("TCP Connection"));
        
        if (ConnectionSocket)
        {
            UE_LOG(LogTemp, Log, TEXT("Conexão de %s"), *RemoteAddress->ToString(true));
            
            uint32 Size;
            TArray<uint8> ReceivedData;
            
            while (ConnectionSocket->HasPendingData(Size))
            {
                ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
                
                int32 Read = 0;
                ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
                
                if (Read > 0)
                {
                    ReceivedData.Add(0); // Add null terminator
                    LastReceivedMessage = FString(UTF8_TO_TCHAR(ReceivedData.GetData()));
                    UE_LOG(LogTemp, Log, TEXT("Received: %s"), *LastReceivedMessage);
                    
                    // Broadcast to Blueprint
                    OnMessageReceived.Broadcast(LastReceivedMessage);
                }
            }
        }
    }
}