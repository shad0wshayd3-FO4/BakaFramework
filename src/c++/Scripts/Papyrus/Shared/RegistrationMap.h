#pragma once

#include "FunctionArgs.h"

namespace Papyrus
{
	namespace Impl
	{
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

		template<class... Args>
		class RegistrationMap :
			public RegistrationMapBase
		{
		public:
			RegistrationMap() = default;
			~RegistrationMap() = default;

			inline void DispatchEvent(const std::string_view& a_eventName, Args... a_args)
			{
				const RE::BSAutoLock locker{ _lock };
				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					auto args = FunctionArgs{ GameVM->GetVM().get(), a_args... };
					for (auto& _event : _events)
					{
						GameVM->GetVM()->DispatchMethodCall(
							_event.first,
							_event.second,
							a_eventName,
							args.CreateThreadScrapFunction(),
							nullptr);
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
				if (auto GameVM = RE::GameVM::GetSingleton())
				{
					auto args = FunctionArgs{ GameVM->GetVM().get() };
					for (auto& _event : _events)
					{
						GameVM->GetVM()->DispatchMethodCall(
							_event.first,
							_event.second,
							a_eventName,
							args.CreateThreadScrapFunction(),
							nullptr);
					}
				}
			}
		};
	}

	template<class T>
	using RegistrationMap = Impl::RegistrationMap<T>;
}
