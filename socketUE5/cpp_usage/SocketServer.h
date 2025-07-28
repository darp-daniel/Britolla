// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TCPCon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageReceivedSignature, const FString&, ReceivedMessage);

UCLASS(Blueprintable, BlueprintType)
class ALPHA_TEST_API ATCPCon : public AActor
{
    GENERATED_BODY()

public:
    ATCPCon();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Blueprint callable function to get the last message
    UFUNCTION(BlueprintCallable, Category = "TCP Socket")
    FString GetLastReceivedMessage() const { return LastReceivedMessage; }

    // Blueprint callable function to send a message
    UFUNCTION(BlueprintCallable, Category = "TCP Socket")
    void SendMessage(const FString& Message);

    void ReadTCPData();
    // Blueprint assignable event for when messages are received
    UPROPERTY(BlueprintAssignable, Category = "TCP Socket")
    FOnMessageReceivedSignature OnMessageReceived;

    // Expose the port as an editable property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Socket")
    int32 Port = 12345;

    // Expose the IP address as an editable property
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCP Socket")
    FString ListenAddress = "0.0.0.0";

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
    FTimerHandle ReadSocketTimerHandle;

};