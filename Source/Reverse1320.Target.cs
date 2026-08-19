using UnrealBuildTool;
using System.Collections.Generic;

public class Reverse1320Target : TargetRules
{
    public Reverse1320Target(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Reverse1320");
    }
}
