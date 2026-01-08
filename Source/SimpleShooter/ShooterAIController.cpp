// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

    //if (AIBehavior != nullptr)
    //{
    //    RunBehaviorTree(AIBehavior);
    //    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    //    GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //    GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    //}
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!AIBehavior) return;

    RunBehaviorTree(AIBehavior);

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB) return;

    // 시작 위치
    BB->SetValueAsVector(TEXT("StartLocation"), InPawn->GetActorLocation());

    // 플레이어 위치
    //APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    //if (PlayerPawn)
    //{
    //    BB->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const
{
   AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
   if (ControlledCharacter != nullptr)
   {
       return ControlledCharacter->IsDead();
   }

   return true;
}




