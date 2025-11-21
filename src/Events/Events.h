#pragma once

#include "Events/Handlers/CurrentRadiationSource.h"
#include "Events/Handlers/ItemCrafted.h"
#include "Events/Handlers/PipboyLightEvent.h"

namespace Events
{
	void Register()
	{
		REX::DEBUG("Registering Event Handlers."sv);

		RE::CurrentRadiationSourceCount::GetEventSource()->RegisterSink(Handlers::CurrentRadiationSourceCountHandler::GetSingleton());
		RE::PipboyLightEvent::GetEventSource()->RegisterSink(Handlers::PipboyLightEventHandler::GetSingleton());
		RE::ItemCrafted::RegisterSink(Handlers::ItemCraftedHandler::GetSingleton());

		if (auto UI = RE::UI::GetSingleton())
		{
			UI->RegisterSink<RE::MenuOpenCloseEvent>(Handlers::ItemCraftedHandler::GetSingleton());
		}
	}
}
