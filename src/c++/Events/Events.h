#pragma once

#include "Events/Handlers/CurrentRadiationSource.h"
#include "Events/Handlers/ItemCrafted.h"
#include "Events/Handlers/PipboyLightEvent.h"

namespace Events
{
	void Register()
	{
		logger::debug("Registering Event Handlers."sv);

		RE::CurrentRadiationSourceCount::GetEventSource()->RegisterSink(Handlers::CurrentRadiationSourceCountHandler::GetSingleton());
		RE::PipboyLightEvent::GetEventSource()->RegisterSink(Handlers::PipboyLightEventHandler::GetSingleton());

		Handlers::ItemCraftedHandler::Register();
	}
}
