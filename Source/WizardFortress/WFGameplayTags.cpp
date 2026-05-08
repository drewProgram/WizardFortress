#include "WFGameplayTags.h"

namespace WFGameplayTags {
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Id_Healing, "Skill.Id.Healing", "Heals caster.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Id_MagicArrow, "Skill.Id.MagicArrow", "Magic Projectile.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Id_UseConsumable, "Skill.Id.UseConsumable", "Uses a consumable.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Id_SwordAttack, "Skill.Id.SwordAttack", "Melee basic sword attack.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Type_Area, "Skill.Type.Area", "Skill affects an area.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Type_Projectile, "Skill.Type.Projectile", "Skill is a projectile. A projectile spawns from a point nearby the caster and has a travel time.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Type_Self, "Skill.Type.Self", "Skill targets caster.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Type_Melee, "Skill.Type.Melee", "Melee contact skills.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Type_Summon, "Skill.Type.Summon", "Skill summons something.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Type_Target, "Skill.Type.Target", "Skill has a target. Istant cast on target.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Element_Fire, "Element.Fire", "Fire. Can cause burn.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Element_Magic, "Element.Magic", "Magic. Don't cause any status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Element_Ice, "Element.Ice", "Ice. Can cause freeze.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Element_Lightning, "Element.Lightning", "Lightning. Can cause slow.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Burning, "Status.Burning", "Burn. Receives damage over time.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Cursed, "Status.Cursed", "Curse. Receives more damage.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Frozen, "Status.Frozen", "Freeze. Can't move or attack.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Slowed, "Status.Slowed", "Slow. Movement speed penalty.");
}