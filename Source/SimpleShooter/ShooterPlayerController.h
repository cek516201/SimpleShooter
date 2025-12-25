// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    virtual void GameHasEnded(class AActor *EndGameFocus = nullptr, bool bIsWinner = false) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void RemoveStartScreen();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> AmmoClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> StartScreenClass;
	UPROPERTY()
    UUserWidget* StartScreen;

    FTimerHandle StartScreenTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;

	FTimerHandle RestartTimer;
	FTimerHandle StartScreenTimer;

	UUserWidget *HUD;

	

	
};
