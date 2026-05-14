#include "Characters/PlayerCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "WizardFortress.h"

#include "Animation/MainAnimInstance.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	bIsRotating = false;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bIsRotating)
	//{
	//	FQuat CurrentQuat = GetActorQuat();

	//	// Interpolação suave com Quaternion (sem gimbal lock!)
	//	FQuat NewQuat = FQuat::Slerp(CurrentQuat, TargetRotation, DeltaTime * 10.f);

	//	SetActorRotation(NewQuat);
	//	// Verifica se chegou no alvo
	//	if (NewQuat.Equals(TargetRotation, 0.5f))
	//	{
	//		SetActorRotation(TargetRotation);
	//		bIsRotating = false;
	//	}
	//}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

	}
	else
	{
		UE_LOG(LogWizardFortress, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void APlayerCharacter::Jump()
{
	Super::Jump();
	UE_LOG(LogTemp, Display, TEXT("Jumping"));
	UMainAnimInstance* AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->bJumpStarted = true;
	}
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();

	UMainAnimInstance* AnimInstance = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
	UE_LOG(LogTemp, Display, TEXT("Stop jump"));
	if (AnimInstance)
	{
		AnimInstance->bJumpStarted = false;
	}
}

void APlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector (world)
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// immediately rotate to opposite side, if necessary
		FVector FwdVec = GetActorForwardVector();
		FVector MovementDirection = FVector::ZeroVector;

		if (Forward > 0)
		{
			MovementDirection = { 1.0f, 0.f, 0.f };
		}
		else
		{
			MovementDirection = { -1.0f, 0.f, 0.f };
		}

		double DP = FVector::DotProduct(MovementDirection, FwdVec);
		if (DP < 0 && !bIsRotating) // < 0 = points to opposite directions
		{
			FRotator NewRotation = FRotator::ZeroRotator;
			if (Forward > 0)
			{
				GetMesh()->SetRelativeScale3D(FVector(MovementDirection.X, 1, 1)); // mesh flip (precisa mudar alguns materials para two sided, senão causa bug visual)
				GetCharacterMovement()->Velocity.X = 0;
				TurnCharacter(180.f);
			}
			else
			{
				GetMesh()->SetRelativeScale3D(FVector(MovementDirection.X, 1, 1));
				GetCharacterMovement()->Velocity.X = 0;
				TurnCharacter(-180.f);
			}
		}

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
	}
}

void APlayerCharacter::DoJumpStart()
{
	Jump();
}

void APlayerCharacter::DoJumpEnd()
{
	StopJumping();
}

void APlayerCharacter::TurnCharacter(float Degrees)
{
	FQuat CurrentQuat = GetActorQuat();

	// Cria uma rotação de 180° no eixo Z (Yaw)
	FQuat TurnQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(Degrees));

	TargetRotation = CurrentQuat * TurnQuat;
	SetActorRotation(TargetRotation);
	//bIsRotating = true;
}

void APlayerCharacter::HandleDeath()
{
	UAnimMontage* LoadedMontage = DeathMontage.LoadSynchronous();
	if (LoadedMontage)
	{
		UE_LOG(LogTemp, Display, TEXT("Playing death montage for actor %s!"), *DisplayName.ToString());

		APlayerController* PC = Cast<APlayerController>(GetController());

		if (PC)
		{
			PC->StopMovement();
			DisableInput(PC);
		}

		UMainAnimInstance* AnimIns = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimIns)
		{
			GetCapsuleComponent()->SetCollisionEnabled(
				ECollisionEnabled::NoCollision
			);

			GetMesh()->SetCollisionEnabled(
				ECollisionEnabled::NoCollision
			);
			AnimIns->bIsAnimFullBody = true;
		}

		GetCapsuleComponent()->SetCanEverAffectNavigation(false);

		PlayAnimMontage(LoadedMontage);
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("No death montage set for %s!"), *DisplayName.ToString());
}
