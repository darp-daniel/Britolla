#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SocketServer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageReceivedSignature, const FString&, ReceivedMessage);

UCLASS(Blueprintable, BlueprintType)
class YOURPROJECT_API ASocketListener : public AActor
{
    GENERATED_BODY()
    
public:    
    ASocketListener();
    
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    // Blueprint callable function to get the last message
    UFUNCTION(BlueprintCallable, Category = "TCP Socket")
    FString GetLastReceivedMessage() const { return LastReceivedMessage; }
    
    // Blueprint callable function to send a message
    UFUNCTION(BlueprintCallable, Category = "TCP Socket")
    void SendMessage(const FString& Message);
    
    // Blueprint assignable event for when messages are received
    UPROPERTY(BlueprintAssignable, Category = "TCP Socket")
    FOnMessageReceivedSignature OnMessageReceived;
    
    // Expose the port as an editable property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Socket")
    int32 Port = 12345;
    
    // Expose the IP address as an editable property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Socket")
    FString ListenAddress = "127.0.0.1";
    
private:
    FSocket* ListenerSocket;
    FSocket* ConnectionSocket;
    
    UPROPERTY()
    FString LastReceivedMessage;
    
    void StartTCPReceiver();
    void TCPSocketListener();
    FSocket* CreateTCPConnectionListener();
    
    FTimerHandle TCPConnectionListenerTimerHandle;
    bool bShouldListen = true;
};