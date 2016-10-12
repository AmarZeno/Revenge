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
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Battery_StaticMeshComponent;



	UPROPERTY(EditAnywhere)
	bool B_IsCollected;

	UPROPERTY(EditAnywhere)
	float F_AvailableCharge;

	
	
};
