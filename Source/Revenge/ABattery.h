// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ABattery.generated.h"

UCLASS()
class REVENGE_API AABattery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABattery();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	/*holds the static mesh component*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Battery_StaticMeshComponent;

	/*has the battery been collected*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool B_IsCollected;
	
	/*the available charge in the battery*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float F_AvailableCharge;

	/*When player collects the battery*/
	void BatterCollected();
	
	bool IsActive();

	void SetActive(bool IsEnabled);

	/*returns the charge in the battery*/
	float f_getPower();	


	void DestroyAfterDelay();


	UPROPERTY(EditAnywhere)
	UParticleSystem* ParticleSystemTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles", Meta = (BlueprintProtected = "true"))
	class UParticleSystemComponent* ParticleSystem;

};
