// Fill out your copyright notice in the Description page of Project Settings.

#include "Revenge.h"
#include "ABattery.h"


// Sets default values
AABattery::AABattery()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//initialize the static mesh component
	Battery_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BATTERY_STATIC_MESH"));
	RootComponent = Battery_StaticMeshComponent;
	
}

// Called when the game starts or when spawned
void AABattery::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AABattery::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

