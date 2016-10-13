// Fill out your copyright notice in the Description page of Project Settings.

#include "Revenge.h"

#include "RevengeCharacter.h"

#include "EnemyCharacter.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//initialize the static mesh component
	Enemy_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ENEMY_STATIC_MESH"));
	RootComponent = Enemy_StaticMeshComponent;


	//lets initialize the capsule trigger component
	CollectionPlayer = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyTrigger"));
	CollectionPlayer->SetupAttachment(RootComponent);
	CollectionPlayer->SetSphereRadius(300.0f);

	CurrentLevel = 1;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	DetectCollisionWithPlayer();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

/*responsible for detecting collision with the player (CALLED IN TICK)*/
void AEnemyCharacter::DetectCollisionWithPlayer() {
	CollectionPlayer->GetOverlappingActors(nonCastedArray);

	for (int32 eachCollected = 0; eachCollected < nonCastedArray.Num(); eachCollected++) {





		ARevengeCharacter* const TempPlayer = Cast<ARevengeCharacter>(nonCastedArray[eachCollected]);

		//so, we got the player
		if (TempPlayer) {
			//transport the enemy to the next location
			if (CurrentLevel < 4) {
				if (CurrentLevel == 1) {
					//transport the enemy to the second vector.. and so for the rest of the if-else loops
					this->SetActorLocation(SecondLevelPosition);
				}
				else if (CurrentLevel == 2) {
					this->SetActorLocation(ThirdLevelPosition);
				}
				else if(CurrentLevel == 3){
					this->SetActorLocation(FourthLevelPosition);
				}
				CurrentLevel += 1;	//increment the level
			}
		}
	}
}

