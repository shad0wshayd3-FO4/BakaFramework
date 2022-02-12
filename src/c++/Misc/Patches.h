#pragma once

#include "Patches/LoadEditorIDs.h"
#include "Patches/RandomMods.h"

namespace Patches
{
	void Install()
	{
		if (*Settings::Patches::EnableLoadingEditorIDs)
		{
			LoadEditorIDs::InstallHooks();
		}

		if (*Settings::Patches::EnableRandomMods)
		{
			RandomMods::InstallHooks();
		}
	}
}
