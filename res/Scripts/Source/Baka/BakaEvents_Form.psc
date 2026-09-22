Scriptname BakaEvents_Form Native Hidden

;PipboyLight

Function RegisterForPipboyLight(Form akForm) Native Global
Function UnregisterForPipboyLight(Form akForm) Native Global

Event OnPipboyLight(bool abActive)
EndEvent

;RadiationCount

Function RegisterForRadiationCount(Form akForm) Native Global
Function UnregisterForRadiationCount(Form akForm) Native Global

Event OnRadiationCount(int aiCount)
EndEvent
