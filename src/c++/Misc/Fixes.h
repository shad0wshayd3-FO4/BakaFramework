#pragma once

#include "Misc/Fixes/OnStoryCraftItem.h"

namespace Fixes
{
	void Install()
	{
		auto& trampoline = F4SE::GetTrampoline();

		// OnStoryCraftItem
		OnStoryCraftItem::_StoryEventArgumentConverter =
			trampoline.write_call<5>(OnStoryCraftItem::Target.address(), OnStoryCraftItem::StoryEventArgumentConverter);
	}
}
