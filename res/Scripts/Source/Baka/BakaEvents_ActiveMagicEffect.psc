Scriptname BakaEvents_ActiveMagicEffect Native Hidden

;PipboyLight

Function RegisterForPipboyLight(ActiveMagicEffect akActiveMagicEffect) Native Global
Function UnregisterForPipboyLight(ActiveMagicEffect akActiveMagicEffect) Native Global

Event OnPipboyLight(bool abActive)
EndEvent

;RadiationCount

Function RegisterForRadiationCount(ActiveMagicEffect akActiveMagicEffect) Native Global
Function UnregisterForRadiationCount(ActiveMagicEffect akActiveMagicEffect) Native Global

Event OnRadiationCount(int aiCount)
EndEvent
