// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WizardFortressPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UMainHUDWidget;

UENUM(BlueprintType)
enum class EPlayerInputMode : uint8
{
	None,
	Gameplay UMETA(DisplayName = "Gameplay"),
	UI     UMETA(DisplayName = "UI")
};

UCLASS(abstract)
class AWizardFortressPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWizardFortressPlayerController();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HidePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TogglePauseMenu();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TryInteract();

	UFUNCTION(BlueprintPure, Category = "UI")
	class UUIManagerComponent* GetUIManager() const;

	UFUNCTION(BlueprintCallable, Category = "Input|Input Mappings")
	void ChangeInputMapping(EPlayerInputMode NewInputMode);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<TObjectPtr<UInputMappingContext>> GameplayMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<TObjectPtr<UInputMappingContext>> UIMappingContexts;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	EPlayerInputMode CurrentInputMode = EPlayerInputMode::None;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetUIManager, Category = "Components")
	TObjectPtr<class UUIManagerComponent> UIManager;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
