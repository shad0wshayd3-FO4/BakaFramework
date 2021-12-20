#pragma once

namespace Fixes
{
	class OnStoryCraftItem
	{
	public:
		static void StoryEventArgumentConverter(void* a_this, const RE::BGSStoryEvent& a_event, RE::BSScript::IVirtualMachine& a_vm, const char* a_arg1, const char* a_arg2, [[maybe_unused]] const char* a_arg3)
		{
			_StoryEventArgumentConverter(a_this, a_event, a_vm, a_arg1, a_arg2, "O1");
		}
		static inline REL::Relocation<std::uintptr_t> Target{ REL::ID(1026783), 0xD26 };
		static inline REL::Relocation<decltype(&OnStoryCraftItem::StoryEventArgumentConverter)> _StoryEventArgumentConverter;
	};
}
