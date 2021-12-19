#pragma once

namespace Papyrus
{
	namespace Impl
	{
		namespace detail
		{
			using RetnType = RE::BSTThreadScrapFunction<bool(RE::BSScrapArray<RE::BSScript::Variable>&)>;

			class RetnTypeInput
			{
			public:
				RetnTypeInput() = delete;
				RetnTypeInput(RE::BSScrapArray<RE::BSScript::Variable>& a_toCopy, RE::BSScript::IVirtualMachine* a_vm)
				{
					_array = new RE::BSScript::ArrayWrapper<RE::BSScript::Variable>(a_toCopy, *a_vm);
					_vm = a_vm;
				}

			private:
				// members
				RE::BSScript::ArrayWrapper<RE::BSScript::Variable>* _array{ nullptr };	// 00
				RE::BSScript::IVirtualMachine* _vm{ nullptr };							// 08
			};
			static_assert(sizeof(RetnTypeInput) == 0x10);

			static RetnType CreateThreadScrapFunction(RetnTypeInput& a_input)
			{
				using func_t = decltype(&CreateThreadScrapFunction);
				REL::Relocation<func_t> func{ REL::ID(69733) };
				return func(a_input);
			}

			template<class T>
			RE::BSScrapArray<RE::BSScript::Variable> GetScrapArray(std::vector<T>& a_args)
			{
				RE::BSScrapArray<RE::BSScript::Variable> result;
				for (auto iter : a_args)
				{
					RE::BSScript::Variable var;
					RE::BSScript::PackVariable<T>(var, iter);
					result.emplace_back(var);
				}

				return result;
			}

			template<class T>
			RetnType PackArguments(RE::BSScript::IVirtualMachine* a_vm, std::vector<T>& a_args)
			{
				auto scrap = GetScrapArray<T>(a_args);
				auto input = RetnTypeInput(scrap, a_vm);
				return CreateThreadScrapFunction(input);
			}
		}

		class RegistrationMapBase
		{
		public:
			RegistrationMapBase() = default;
			virtual ~RegistrationMapBase();

			virtual constexpr std::uint32_t GetRecordVersion() = 0;
			virtual constexpr std::uint32_t GetRecordType() = 0;
			virtual constexpr std::string_view GetRecordName() = 0;
			virtual bool Save(const F4SE::SerializationInterface* a_intfc);
			virtual bool Load(const F4SE::SerializationInterface* a_intfc);
			virtual bool LoadImpl(const F4SE::SerializationInterface* a_intfc);
			virtual void Clear();

			void Register(const RE::BSScript::Object& a_object);
			void Unregister(const RE::BSScript::Object& a_object);

		protected:
			RE::BSSpinLock _lock;
			RE::BSTHashMap<std::size_t, RE::BSFixedString> _events;
		};

		template<class RetnType>
		class RegistrationMap :
			public RegistrationMapBase
		{
		public:
			RegistrationMap() = default;
			~RegistrationMap() = default;

			inline void DispatchEvent(const std::string_view& a_eventName, std::vector<RetnType>& a_args)
			{
				const RE::BSAutoLock locker{ _lock };
				if (auto VM = RE::GameVM::GetSingleton() ? RE::GameVM::GetSingleton()->GetVM() : nullptr; VM)
				{
					auto arguments = detail::PackArguments<RetnType>(VM.get(), a_args);
					for (auto& _event : _events)
					{
						VM->DispatchMethodCall(_event.first, _event.second, a_eventName, arguments, nullptr);
					}
				}
			}
		};

		template<>
		class RegistrationMap<void> :
			public RegistrationMapBase
		{
		public:
			RegistrationMap() = default;
			~RegistrationMap() = default;

			inline void DispatchEvent(const std::string_view& a_eventName)
			{
				const RE::BSAutoLock locker{ _lock };
				if (auto VM = RE::GameVM::GetSingleton() ? RE::GameVM::GetSingleton()->GetVM() : nullptr; VM)
				{
					auto emptyArgs = std::vector<std::uint8_t>();
					auto arguments = detail::PackArguments<std::uint8_t>(VM.get(), emptyArgs);
					for (auto& _event : _events)
					{
						VM->DispatchMethodCall(_event.first, _event.second, a_eventName, arguments, nullptr);
					}
				}
			}
		};
	}

	template<class T>
	using RegistrationMap = Impl::RegistrationMap<T>;
}
