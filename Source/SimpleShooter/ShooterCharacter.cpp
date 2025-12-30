// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SimpleShooterGameModeBase.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	ActiveIndex = 0;
	UE_LOG(LogTemp, Warning, TEXT("ActiveIndex = %d"), ActiveIndex);
	for (int32 Index = 0; Index < 3; Index++)
	{
		Gun[Index] = GetWorld()->SpawnActor<AGun>(GunClass[Index]);

		if (Gun[Index])
		{
			Gun[Index]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			Gun[Index]->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("Ataached mesh number %d"), Index);

			if (Index != ActiveIndex)
			{
				Gun[Index]->SetActorHiddenInGame(true);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn gun number %d"), Index);
		}
	}
	
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
	//PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Interact);
	PlayerInputComponent->BindAxis(TEXT("SwitchWeapons"), this, &AShooterCharacter::SwitchWeapons);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
    Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		 
		
	}

	return DamageToApply;

}

void AShooterCharacter::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun[ActiveIndex]->PullTrigger();
}

void AShooterCharacter::SwitchWeapons(float Slot)
{
	int32 SlotIndex = static_cast<int32>(Slot);
	Gun[ActiveIndex]->SetActorHiddenInGame(true);
	ActiveIndex += SlotIndex;
	ActiveIndex = FMath::Clamp(ActiveIndex, 0, 2);
	Gun[ActiveIndex]->SetActorHiddenInGame(false);
}


void AShooterCharacter::Reload()
{
    if (!Gun[ActiveIndex]) return;
    if (!Ammos.IsValidIndex(ActiveIndex)) return;

    const int32 Max = Gun[ActiveIndex]->GetTotalAmmo();
    const int32 Current = Gun[ActiveIndex]->GetCurrentAmmo();
	const int32 Reload = Gun[ActiveIndex]->GetReloadAmmo();
    const int32 Need = Reload - Current;
    if (Need <= 0) return;

    const int32& Reserve = Ammos[ActiveIndex];
	if (Reserve <= 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NoAmmoSound, GetActorLocation());
		return;
	}

    const int32 Take = FMath::Min(Need, Reserve);
    Gun[ActiveIndex]->AddAmmo(Take);
    Ammos[ActiveIndex] -= Take;
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
}

int32 AShooterCharacter::GetCurrentAmmo() const
{
    if (!Gun[ActiveIndex]) return 0;
    return Gun[ActiveIndex]->GetCurrentAmmo();
}

int32 AShooterCharacter::GetTotalAmmo() const
{
    if (!Gun[ActiveIndex]) return 0;
    const int32 Reserve = Ammos.IsValidIndex(ActiveIndex) ? Ammos[ActiveIndex] : 0;
    return Reserve;
}

int32 AShooterCharacter::GetReloadAmmo() const
{
	if (!Gun[ActiveIndex]) return 0;
	return Gun[ActiveIndex]->GetReloadAmmo();
}


