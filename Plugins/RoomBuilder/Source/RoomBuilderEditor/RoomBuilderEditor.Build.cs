using UnrealBuildTool;

public class RoomBuilderEditor : ModuleRules
{
    public RoomBuilderEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.Add("EditorSubsystem");

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "UnrealEd",
            "RoomBuilder",
            "GameplayTags"
        });
    }
}