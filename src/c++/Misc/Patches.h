#pragma once

#include "Patches/LoadEditorIDs.h"

namespace Patches
{
	void Install()
	{
		if (*Settings::Patches::EnableLoadingEditorIDs)
		{
			LoadEditorIDs::InstallHooks();
		}
	}
}
