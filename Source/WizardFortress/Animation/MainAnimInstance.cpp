#include "Animation/MainAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/BaseCharacter.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Char = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (!Char)
	{
		//UE_LOG(LogTemp, Error, TEXT("UMainAnimInstance::NativeInitializeAnimation() -> COULD NOT CAST TO PLAYER CHARACTER!!"));
	}

	CharacterRotationYaw = 0.f;
	CharacterRotationLastYaw = 0.f;
	RootRotationYaw = 0.f;
	bJumpStarted = false;
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Char)
	{
		// speed
		Velocity = Char->GetVelocity();
		FVector VelocityNoZ = Velocity;
		VelocityNoZ.Z = 0.f;
		Speed = VelocityNoZ.Length();							// pegando velocidade escalar

		// aceleração
		//bIsAccelerating = Char->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f;
		bIsAccelerating = Speed > 10.0f;

		//está no ar ou não
		bIsInAir = Char->GetCharacterMovement()->IsFalling();

		// turn in place logic
		CharacterRotationLastYaw = CharacterRotationYaw;

		CharacterRotationYaw = Char->GetActorRotation().Yaw;

		const float CharacterRotationYawDelta = CharacterRotationYaw - CharacterRotationLastYaw;

		RootRotationYaw -= CharacterRotationYawDelta;
	}
}
