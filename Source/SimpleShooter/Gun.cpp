// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "ShooterCharacter.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::PullTrigger()
{
	if (CurrentAmmo <= 0)
    {
        // 여기서 빈탄창 사운드 같은 거 틀어도 됨
        return;
    }

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

    FHitResult Hit;
	FVector ShotDirection;
    bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			// ★ 팀킬 체크 추가
			if (ShouldDamageTarget(HitActor))
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
		}
	}

	CurrentAmmo -= 1;

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

bool AGun::ShouldDamageTarget(AActor* HitActor) const
{
	if (!HitActor || !GetOwner())
		return false;

	// ShooterCharacter로 캐스팅
	AShooterCharacter* Shooter = Cast<AShooterCharacter>(GetOwner());
	AShooterCharacter* Target = Cast<AShooterCharacter>(HitActor);

	// 둘 다 ShooterCharacter가 아니면 데미지 (벽, 오브젝트 등)
	if (!Shooter || !Target)
		return true;

	// ★ 팀이 다르면 데미지
	return Shooter->GetTeam() != Target->GetTeam();
}

AController* AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;
	return OwnerPawn->GetController();
}

void AGun::AddAmmo(int NewAmmo)
{
	int AmmoTemp = NewAmmo + CurrentAmmo;
	CurrentAmmo = FMath::Clamp(AmmoTemp, 0, MaxAmmo);
}

int AGun::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

int AGun::GetTotalAmmo() const
{
	return MaxAmmo;
}

int AGun::GetReloadAmmo() const
{
	return ReloadAmmo;
}