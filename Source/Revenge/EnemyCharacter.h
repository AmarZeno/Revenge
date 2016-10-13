// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class REVENGE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Enemy_StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	bool B_IsAlive;

	UPROPERTY(EditAnywhere)
	FVector F_Position;

	/*used to detect collision with the player*/
	UPROPERTY(EditAnywhere)
	USphereComponent* CollectionPlayer;

	void DetectCollisionWithPlayer();

	TArray<AActor*> nonCastedArray;
	TArray<AEnemyCharacter*> arrayToHoldThePlayer;

	//3 vectors holding the position of the enemy once player reaches to it.
	UPROPERTY(EditAnywhere)
	FVector SecondLevelPosition;

	UPROPERTY(EditAnywhere)
	FVector ThirdLevelPosition;

	UPROPERTY(EditAnywhere)
	FVector FourthLevelPosition;

	/*maintains the current level*/
	UPROPERTY(EditAnywhere)
	int CurrentLevel;


};
