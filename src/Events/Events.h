#pragma once

#include "Events/Handlers/CurrentRadiationSource.h"
#include "Events/Handlers/ItemCrafted.h"
#include "Events/Handlers/PipboyLightEvent.h"

namespace Events
{
	void Register()
	{
		RE::CurrentRadiationSourceCount::GetEventSource()->RegisterSink(Handlers::CurrentRadiationSourceCountHandler::GetSingleton());
		RE::PipboyLightEvent::GetEventSource()->RegisterSink(Handlers::PipboyLightEventHandler::GetSingleton());
		
		if (auto UI = RE::UI::GetSingleton())
		{
			RE::ItemCrafted::RegisterSink(Handlers::ItemCraftedHandler::GetSingleton());
			UI->RegisterSink<RE::MenuOpenCloseEvent>(Handlers::ItemCraftedHandler::GetSingleton());
		}

		REX::DEBUG("Registered Event Handlers."sv);
	}
}
