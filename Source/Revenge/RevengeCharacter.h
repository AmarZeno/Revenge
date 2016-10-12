// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once


#include "GameFramework/Character.h"
#include "RevengeCharacter.generated.h"


UCLASS(config=Game)
class ARevengeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ARevengeCharacter();

	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//---------------------CUSTOM CODE------------------------------------------

	/*batteries within the sphere will get consumed*/
	UPROPERTY(EditAnywhere)
	USphereComponent* CollectionSphere;

	/*called when the player collects the battery*/
	void CollectBattery();

	/*starting charge of character*/
	UPROPERTY(EditAnywhere, Category = AAAAAAAAAA)
	float InitialPower;

	UPROPERTY(EditAnywhere, Category = AAAAAAAAAA)
	float CharacterPower;

	/*indicates the current time left for the player in order to get a charge*/
	UPROPERTY(EditAnywhere, Category = AAAAAAAAAA)
	float TimeLeft;

	/*player time is reset to this value*/
	UPROPERTY(EditAnywhere, Category = AAAAAAAAAA)
	float ResetTime;

	

	//getters
	float f_getInitialPower();

	float f_getCharacterPower();

	float f_getTimeLeft();

	

	


protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void TriggerEnter(class AActor*, class UPrimitiveComponent* otherComponent, int32 otherBodyIndex);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

