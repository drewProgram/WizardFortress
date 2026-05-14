// Copyright Epic Games, Inc. All Rights Reserved.


#include "WizardFortressPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "WizardFortress.h"
#include "Widgets/Input/SVirtualJoystick.h"

#include "UI/MainHUDWidget.h"
#include "Systems/AttributeSystem.h"
#include "Characters/BaseCharacter.h"

void AWizardFortressPlayerController::ShowPauseMenu()
{
}

void AWizardFortressPlayerController::HidePauseMenu()
{
}

void AWizardFortressPlayerController::TogglePauseMenu()
{
}

void AWizardFortressPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogWizardFortress, Error, TEXT("Could not spawn mobile controls widget."));

		}
	}

	if (MainHUDClass)
	{
		MainHUD = CreateWidget<UMainHUDWidget>(this, MainHUDClass);
		if (MainHUD)
		{
			MainHUD->AddToViewport();
			bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
		}
	}
}

void AWizardFortressPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void AWizardFortressPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UAttributeSystem* AttrSys = (Cast<ABaseCharacter>(GetPawn()))->GetAttributeComponent())
	{
		AttrSys->OnHealthChanged.AddUObject(this, &AWizardFortressPlayerController::HandleHealthChanged);
		AttrSys->OnManaChanged.AddUObject(this, &AWizardFortressPlayerController::HandleManaChanged);

		HandleHealthChanged(AttrSys->GetHealth(), AttrSys->GetMaxHealth());
		HandleManaChanged(AttrSys->GetMana(), AttrSys->GetMaxMana());
	}
}

bool AWizardFortressPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AWizardFortressPlayerController::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	if (MainHUD)
	{
		MainHUD->SetHealth(CurrentHealth, MaxHealth);
	}
}

void AWizardFortressPlayerController::HandleManaChanged(int32 CurrentMana, int32 MaxMana)
{
	if (MainHUD)
	{
		MainHUD->SetMana(CurrentMana, MaxMana);
	}
}
