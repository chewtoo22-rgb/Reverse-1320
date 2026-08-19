using UnrealBuildTool;
using System.Collections.Generic;

public class Reverse1320EditorTarget : TargetRules
{
    public Reverse1320EditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Reverse1320");
    }
}
