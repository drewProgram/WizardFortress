// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Combat/SkillDefinition.h"

#include "SkillManager.generated.h"

// TODO: implementar essa struct aqui. Usar quando for executar uma skill pelo skill behavior
/*
struct FSkillExecutionContext
{
    UWorld* World;
    ABaseCharacter* Caster;
    USkillManager* SkillManager;
    const USkillDefinition* Definition;
};

*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillFinished);

USTRUCT(BlueprintType)
struct FSkillRuntimeData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<USkillDefinition> Definition = nullptr;

    // Timestamp (via GetWorld()->GetTimeSeconds()) de quando o cooldown termina
    float CooldownEndTime = 0.0f;

    bool IsOnCooldown(float CurrentTime) const
    {
        return CurrentTime < CooldownEndTime;
    }
    
    void StartCooldown(float CurrentTime)
    {
        if (Definition)
            CooldownEndTime = CurrentTime + Definition->Stats.Cooldown;
    }
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WIZARDFORTRESS_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillManager();

    UFUNCTION(BlueprintCallable)
    bool TryCastSkill(FGameplayTag SkillId);

    UFUNCTION(BlueprintCallable)
    void ExecuteCurrentSkill();

    UPROPERTY(BlueprintAssignable)
    FOnSkillFinished OnSkillFinished;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FGameplayTag, FSkillRuntimeData> Skills;

    UPROPERTY()
    FGameplayTag CurrentSkill;

    TArray<FSkillRuntimeData*> SkillsOnCooldown;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
