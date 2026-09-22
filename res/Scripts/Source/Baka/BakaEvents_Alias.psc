Scriptname BakaEvents_Alias Native Hidden

;PipboyLight

Function RegisterForPipboyLight(Alias akAlias) Native Global
Function UnregisterForPipboyLight(Alias akAlias) Native Global

Event OnPipboyLight(bool abActive)
EndEvent

;RadiationCount

Function RegisterForRadiationCount(Alias akAlias) Native Global
Function UnregisterForRadiationCount(Alias akAlias) Native Global

Event OnRadiationCount(int aiCount)
EndEvent
