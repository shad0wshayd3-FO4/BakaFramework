#pragma once

#include "Patches/LoadEditorIDs.h"

namespace Patches
{
	void Install()
	{
		if (Config::Patches::bEnableLoadingEditorIDs)
		{
			LoadEditorIDs::Install();
		}
	}
}
