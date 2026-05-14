// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardFortressPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UMainHUDWidget;

UCLASS(abstract)
class AWizardFortressPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HidePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TogglePauseMenu();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	TObjectPtr<UMainHUDWidget> MainHUD;

	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenu;

	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	bool ShouldUseTouchControls() const;

	void HandleHealthChanged(float CurrentHealth, float MaxHealth);
	void HandleManaChanged(int32 CurrentMana, int32 MaxMana);

};
