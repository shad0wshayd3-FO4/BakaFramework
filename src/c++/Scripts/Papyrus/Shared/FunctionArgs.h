#pragma once

namespace Papyrus
{
	namespace detail
	{
		template<class... Args>
		RE::BSScrapArray<RE::BSScript::Variable>
			PackVariables(Args&&... a_args)
		{
			constexpr auto size = sizeof...(a_args);
			auto args = std::make_tuple(std::forward<Args>(a_args)...);
			RE::BSScrapArray<RE::BSScript::Variable> result{ size };
			[&]<std::size_t... p>(std::index_sequence<p...>)
			{
				((RE::BSScript::PackVariable(result.at(p), std::get<p>(args))), ...);
			}
			(std::make_index_sequence<size>{});
			return result;
		}

		class FunctionArgsBase
		{
		public:
			FunctionArgsBase() = delete;
			FunctionArgsBase(RE::BSScript::IVirtualMachine* a_vm) :
				vm(a_vm)
			{}

			RE::msvc::function<bool(RE::BSScrapArray<RE::BSScript::Variable>&)>
				CreateThreadScrapFunction()
			{
				using func_t = RE::msvc::function<bool(RE::BSScrapArray<RE::BSScript::Variable>&)> (*)(FunctionArgsBase&);
				REL::Relocation<func_t> func{ REL::ID(69733) };
				return func(*this);
			}

		protected:
			// members
			RE::BSScript::ArrayWrapper<RE::BSScript::Variable>* args;  // 00
			RE::BSScript::IVirtualMachine* vm;						   // 08
		};
		static_assert(sizeof(FunctionArgsBase) == 0x10);
	}

	template<class... Args>
	class FunctionArgs :
		public detail::FunctionArgsBase
	{
	public:
		FunctionArgs() = delete;
		FunctionArgs(RE::BSScript::IVirtualMachine* a_vm, Args... a_args) :
			FunctionArgsBase(a_vm)
		{
			auto scrap = detail::PackVariables(a_args...);
			args = new RE::BSScript::ArrayWrapper<RE::BSScript::Variable>(scrap, *vm);
		}
	};
	static_assert(sizeof(FunctionArgs<std::monostate>) == 0x10);
}
