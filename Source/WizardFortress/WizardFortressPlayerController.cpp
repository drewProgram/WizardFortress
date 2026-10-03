// Copyright Epic Games, Inc. All Rights Reserved.


#include "WizardFortressPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "WizardFortress.h"

#include "UI/HUDWidget.h"
#include "Systems/AttributeSystem.h"
#include "UI/UIManagerComponent.h"
#include "Characters/BaseCharacter.h"

AWizardFortressPlayerController::AWizardFortressPlayerController()
{
	UIManager = CreateDefaultSubobject<UUIManagerComponent>(TEXT("UIManager"));
}

void AWizardFortressPlayerController::ShowPauseMenu()
{
}

void AWizardFortressPlayerController::HidePauseMenu()
{
}

void AWizardFortressPlayerController::TogglePauseMenu()
{
}

void AWizardFortressPlayerController::ToggleInventory()
{
}

void AWizardFortressPlayerController::TryInteract()
{
}

UUIManagerComponent* AWizardFortressPlayerController::GetUIManager() const
{
	return UIManager;
}

void AWizardFortressPlayerController::ChangeInputMapping(EPlayerInputMode NewInputMode)
{
	if (CurrentInputMode == NewInputMode)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			GetLocalPlayer());

	if (!IsValid(Subsystem))
	{
		return;
	}

	switch (CurrentInputMode)
	{
	case EPlayerInputMode::Gameplay:
	{
		for (UInputMappingContext* Context : GameplayMappingContexts)
		{
			Subsystem->RemoveMappingContext(Context);
		}

		for (UInputMappingContext* Context : UIMappingContexts)
		{
			Subsystem->AddMappingContext(Context, 0);
			SetPause(true);
		}

		break;
	}

	case EPlayerInputMode::UI:
	{
		for (UInputMappingContext* Context : UIMappingContexts)
		{
			Subsystem->RemoveMappingContext(Context);
		}

		for (UInputMappingContext* Context : GameplayMappingContexts)
		{
			Subsystem->AddMappingContext(Context, 0);
			SetPause(false);
		}

		break;
	}

	case EPlayerInputMode::None:
		for (UInputMappingContext* Context : GameplayMappingContexts)
		{
			Subsystem->AddMappingContext(Context, 0);
		}
		break;
	}

	CurrentInputMode = NewInputMode;
}

void AWizardFortressPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AWizardFortressPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ChangeInputMapping(EPlayerInputMode::Gameplay);
}

void AWizardFortressPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UAttributeSystem* AttrSys = (Cast<ABaseCharacter>(GetPawn()))->GetAttributeComponent())
	{
		if (UIManager)
		{
			UIManager->InitUI();

			UIManager->BindHUDToAttributes(AttrSys);
		}
	}
}

void AWizardFortressPlayerController::OnUnPossess()
{
	UIManager->UnbindHUDToAttributes();

	Super::OnUnPossess();
}
