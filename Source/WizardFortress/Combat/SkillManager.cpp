// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/SkillManager.h"
#include "Characters/BaseCharacter.h"

#include "Systems/AttributeSystem.h"
#include "Combat/SkillActor.h"
#include "WFGameplayTags.h"
#include "Combat/SkillBehavior.h"

// Sets default values for this component's properties
USkillManager::USkillManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSkill = FGameplayTag::EmptyTag;

}

bool USkillManager::TryCastSkill(FGameplayTag SkillId)
{
	FSkillRuntimeData* Skill = Skills.Find(SkillId);
	if (Skill)
	{
		if (CurrentSkill.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Already casting a skill!"));
			return false;
		}

		if (Skill->IsOnCooldown(GetWorld()->GetTimeSeconds()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
			return false;
		}

		if (ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner()))
		{
			if (Owner->GetAttributeComponent()->RequestUpdateMana((Skill->Definition->Stats.ManaCost) * -1))
			{
				UAnimMontage* LoadedMontage = Skill->Definition->Montage.LoadSynchronous();
				if (LoadedMontage)
				{
					UE_LOG(LogTemp, Display, TEXT("Playing montage for skill %s!"), *Skill->Definition->DisplayName.ToString());
					CurrentSkill = Skill->Definition->SkillId;
					Owner->PlayAnimMontage(LoadedMontage);
					return true;
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Could not play montage!"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Skill not found!"));
	return false;
}

void USkillManager::ExecuteCurrentSkill()
{
	FSkillRuntimeData* Skill = Skills.Find(CurrentSkill);
	if (!Skill || !Skill->Definition || !Skill->Definition->Behavior)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid current skill."));
		CurrentSkill = FGameplayTag::EmptyTag;
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Casting skill %s!"), *Skill->Definition->DisplayName.ToString());
	const bool bExecuted = Skill->Definition->Behavior->Execute(this, *Skill);

	if (bExecuted)
	{
		if (Skill->Definition->Stats.Cooldown > 0)
		{
			Skill->StartCooldown(GetWorld()->GetTimeSeconds());
			SkillsOnCooldown.Add(Skill);
		}

		OnSkillFinished.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not execute skill."));
	}

	CurrentSkill = FGameplayTag::EmptyTag;
}


// Called when the game starts
void USkillManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void USkillManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SkillsOnCooldown.Num() > 0)
	{
		TArray<FSkillRuntimeData*> SkillsToBeRemoved;
		for (FSkillRuntimeData* Skill : SkillsOnCooldown)
		{
			if (!Skill->IsOnCooldown(GetWorld()->GetTimeSeconds()))
			{
				SkillsToBeRemoved.Push(Skill);
			}
		}

		if (SkillsToBeRemoved.Num() > 0)
		{
			for (FSkillRuntimeData* Skill : SkillsToBeRemoved)
			{
				SkillsOnCooldown.Remove(Skill);
			}
		}
	}
}

