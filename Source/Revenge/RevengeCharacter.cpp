// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Revenge.h"
#include "RevengeCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
#include "ABattery.h"

//////////////////////////////////////////////////////////////////////////
// ARevengeCharacter

ARevengeCharacter::ARevengeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	//create the collection sphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionTrigger"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);
	
	//set the character power charge
	InitialPower = 2000.0f;
	CharacterPower = InitialPower;

	TimeLeft = ResetTime;
	ResetTime = 1000.0f;	//time is reset to this value
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARevengeCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Raycast", IE_Pressed, this, &ARevengeCharacter::PerformRaycast);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ARevengeCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARevengeCharacter::MoveRight);

	InputComponent->BindAction("CollectBattery", IE_Pressed, this, &ARevengeCharacter::CollectBattery);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ARevengeCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ARevengeCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ARevengeCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ARevengeCharacter::TouchStopped);
}

void ARevengeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ARevengeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ARevengeCharacter::TriggerEnter(AActor *, UPrimitiveComponent * otherComponent, int32 otherBodyIndex)
{
	otherComponent->SetActive(false);	
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	for (int32 eachCollected = 0; eachCollected < CollectedActors.Num(); eachCollected++) {
		AABattery* const TempBattery = Cast<AABattery>(CollectedActors[eachCollected]);

		//if the cast is successfull
		if (TempBattery) {
			TempBattery->SetActive(false);
			TimeLeft = ResetTime;	//reset the time of the player
		}
	}
}

void ARevengeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARevengeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARevengeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARevengeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARevengeCharacter::PerformRaycast()
{

	/*
	FVector mouseLocation, mouseDirection;

	APlayerController* PlayerController = (APlayerController*)GetWorld()->GetFirstPlayerController();

	PlayerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

	FRotator currentCharacterRotation = this->GetActorRotation();
	FRotator targetRotation = mouseDirection.Rotation();

	FRotator newRotation = FRotator(currentCharacterRotation.Pitch, targetRotation.Yaw, currentCharacterRotation.Roll);
	*/

	APlayerController* PlayerController = (APlayerController*)GetWorld()->GetFirstPlayerController();

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	FVector WorldLoc, WorldDir;
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
		if (Viewport)
		{
			FVector2D ViewportSize;
			LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
			const int32 X = static_cast<int32>(ViewportSize.X * 0.5f);
			const int32 Y = static_cast<int32>(ViewportSize.Y * 0.5f);
			PlayerController->DeprojectScreenPositionToWorld(X, Y, WorldLoc, WorldDir);
		}
	}


	
	//WorldDir.ForwardVector

	//this->SetActorRotation(newRotation);
	

	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = this->GetActorLocation();

	FVector ForwardVector = WorldDir;
	//FVector ForwardVector = newRotation.Vector();

	FVector EndTrace = ((ForwardVector * 2000.0f) + StartTrace);
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	if (GetWorld() -> LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams)) {
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HIT the targetr"), *HitResult->Actor->GetName()));
	}
}

void ARevengeCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	//keep decreasing the time
	TimeLeft--;
	
	CollectBattery();

}

void ARevengeCharacter::CollectBattery() {
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);



	for (int32 eachCollected = 0; eachCollected < CollectedActors.Num(); eachCollected++) {
		AABattery* const TempBattery = Cast<AABattery>(CollectedActors[eachCollected]);

		//if the cast is successfull
		if (TempBattery) {
			TempBattery->SetActive(false);
			TimeLeft = ResetTime;
		}
	}
}

float ARevengeCharacter::f_getInitialPower() {
	return InitialPower;
}

float ARevengeCharacter::f_getCharacterPower() {
	return CharacterPower;
}

float ARevengeCharacter::f_getTimeLeft() {
	return TimeLeft;
}
