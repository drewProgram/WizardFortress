#pragma once

#include "NativeGameplayTags.h"

namespace WFGameplayTags {
    ///////////////////////////////////////////////////
    // Skills
    // ////////////////////////////////////////////////
    
    // Skill Names
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Id_MagicArrow);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Id_Healing);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Id_SwordAttack);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Id_UseConsumable);

    // Skill Types
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Type_Area);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Type_Projectile);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Type_Self);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Type_Melee);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Type_Summon);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Type_Target);
    
    // Elements
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Element_Fire);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Element_Magic);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Element_Ice);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Element_Lightning);

    // Status effects
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Burning);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Cursed);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Frozen);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Slowed);
}