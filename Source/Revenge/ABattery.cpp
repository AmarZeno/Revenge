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

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> Template
	//(TEXT("ParticleSystem'/Revenge/Content/StarterContent/Particles/P_Sparks'"));

	/*if (Template.Succeeded()) {
		ParticleSystemTemplate = Template.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'P_electricity_arc' not found"));
	}*/
	
	/*the battery has not been collected yet*/
	B_IsCollected = false;
	F_AvailableCharge = 150.0f;
}

// Called when the game starts or when spawned
void AABattery::BeginPlay()
{
	Super::BeginPlay();
}

/*returns the charge left in the battery*/
float AABattery::f_getPower() {
	return F_AvailableCharge;
}

// Called every frame
void AABattery::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//ParticleSystem = UGameplayStatics::SpawnEmitterAttached(ParticleSystemTemplate, Cast<USceneComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass())), NAME_None);
//	UpdateBeamTargetPoint();

	/*
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AABattery::DestroyAfterDelay, 1, false);*/

}

bool AABattery::IsActive() {
	if (B_IsCollected != true) {
		return true;
	} else {
		return false;
	}
}

void AABattery::BatterCollected() {
	FString DebugLogmessage = GetName();
	UE_LOG(LogClass, Log, TEXT("Collected %s"), *DebugLogmessage);
	Destroy();	//destroy the battery
}


void AABattery::SetActive(bool Enable) {
	if (Enable == false) {
		Destroy();
		//Battery_StaticMeshComponent->SetActive(false);
		//RootComponent->SetActive(false);
	}
}
/*
void AABattery::WasCollected() {
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &AABattery::DestroyAfterDelay, 10, false);
}
/*
void AABattery::UpdateBeamTargetPoint()
{
	ACharacter* Character = GetWorld()->GetFirstPlayerController()->GetCharacter();
	FVector SocketLocation = Character->GetMesh()->GetSocketLocation("spine_02");
	ParticleSystem->SetBeamTargetPoint(0, SocketLocation, 0);
}
*/
void AABattery::DestroyAfterDelay() {
	Destroy();
}

