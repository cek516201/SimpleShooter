// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (StartScreenClass)
    {
    StartScreen = CreateWidget<UUserWidget>(this, StartScreenClass);
    if (StartScreen != nullptr)
    {
        StartScreen->AddToViewport();
        GetWorld()->GetTimerManager().SetTimer(StartScreenTimer, this, &AShooterPlayerController::RemoveStartScreen, 3.0f, false);
    }
}

    HUD = CreateWidget(this, HUDClass);
        if (HUD != nullptr)
        {
            HUD->AddToViewport();
        }


}

void AShooterPlayerController::GameHasEnded(class AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();

    if (bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if (WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if (LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }
    
    

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);

}

void AShooterPlayerController::RemoveStartScreen()
{
    if (StartScreen)
    {
        StartScreen->RemoveFromViewport();
        StartScreen = nullptr;
    }
}


