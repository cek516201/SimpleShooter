// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	void Reload();
	void AddAmmo(int NewAmmo);
	int GetCurrentAmmo() const;
    int GetTotalAmmo() const;
	int GetReloadAmmo() const;  // 추가
    bool HasAmmo() const { return CurrentAmmo > 0; } // 추가(선택)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame 
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(EditAnywhere)
	int CurrentAmmo;

    UPROPERTY(EditAnywhere)
	int MaxAmmo;

	UPROPERTY(EditAnywhere)
	int ReloadAmmo;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

	

};
