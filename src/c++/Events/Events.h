#pragma once

#include "Events/Handlers/CurrentRadiationSource.h"
#include "Events/Handlers/ItemCrafted.h"
#include "Events/Handlers/PipboyLightEvent.h"
#include "Events/Handlers/TESContainerChanged.h"

namespace Events
{
	void Register()
	{
		logger::debug("Registering Event Handlers."sv);
		
		RE::CurrentRadiationSourceCount::GetEventSource()->RegisterSink(Handlers::CurrentRadiationSourceCountHandler::GetSingleton());
		RE::PipboyLightEvent::GetEventSource()->RegisterSink(Handlers::PipboyLightEventHandler::GetSingleton());
		RE::TESContainerChangedEvent::GetEventSource()->RegisterSink(Handlers::TESContainerChangedEventHandler::GetSingleton());

		Handlers::ItemCraftedHandler::Register();
	}
}
