#pragma once

namespace Papyrus::Shared
{
	class RegistrationMap
	{
	public:
		virtual ~RegistrationMap() { Revert(); }

		virtual std::uint32_t GetRecordSig() = 0;
		virtual std::uint32_t GetRecordVer() = 0;
		virtual const char*   GetEventName() = 0;

		void Register(const RE::BSScript::Variable* a_variable)
		{
			if (!a_variable->is<RE::BSScript::Object>())
			{
				return;
			}

			const RE::BSAutoLock lock{ spin };
			if (auto GameVM = RE::GameVM::GetSingleton())
			{
				auto object = RE::BSScript::UnpackVariable<RE::BSTSmartPointer<RE::BSScript::Object>>(*a_variable);
				if (!object || object->GetHandle() == GameVM->handlePolicy.EmptyHandle())
				{
					return;
				}

				if (!object->type)
				{
					return;
				}

				GameVM->handlePolicy.PersistHandle(object->GetHandle());
				map.emplace(
					object->GetHandle(),
					object->type->GetName());
			}
		}

		void Unregister(const RE::BSScript::Variable* a_variable)
		{
			if (!a_variable->is<RE::BSScript::Object>())
			{
				return;
			}

			const RE::BSAutoLock lock{ spin };
			if (auto GameVM = RE::GameVM::GetSingleton())
			{
				auto object = RE::BSScript::UnpackVariable<RE::BSTSmartPointer<RE::BSScript::Object>>(*a_variable);
				if (!object || object->GetHandle() == GameVM->handlePolicy.EmptyHandle())
				{
					return;
				}

				GameVM->handlePolicy.ReleaseHandle(object->GetHandle());
				map.erase(object->GetHandle());
			}
		}

		void Revert()
		{
			const RE::BSAutoLock lock{ spin };
			if (auto GameVM = RE::GameVM::GetSingleton())
			{
				for (auto& [handle, string] : map)
				{
					GameVM->handlePolicy.ReleaseHandle(handle);
				}
			}

			map.clear();
		}

		void Save(const F4SE::SerializationInterface* a_intfc)
		{
			const RE::BSAutoLock lock{ spin };
			a_intfc->OpenRecord(
				GetRecordSig(),
				GetRecordVer());

			std::uint32_t size{ map.size() };
			a_intfc->WriteRecordData(&size, sizeof(size));

			for (auto& [handle, string] : map)
			{
				a_intfc->WriteRecordData(&handle, sizeof(handle));
				std::uint32_t length{ string.size() };
				a_intfc->WriteRecordData(&length, sizeof(length));
				a_intfc->WriteRecordData(string.data(), length);
			}
		}

		void Load(const F4SE::SerializationInterface* a_intfc)
		{
			Revert();

			const RE::BSAutoLock lock{ spin };
			if (auto GameVM = RE::GameVM::GetSingleton())
			{
				std::uint32_t sig{ 0 }, ver{ 0 }, len{ 0 };
				while (a_intfc->GetNextRecordInfo(sig, ver, len))
				{
					if (sig != GetRecordSig() || ver != GetRecordVer())
					{
						continue;
					}

					std::uint32_t size{ 0 };
					a_intfc->ReadRecordData(&size, sizeof(size));

					for (std::uint32_t i = 0; i < size; i++)
					{
						std::uint64_t handle{ 0 };
						a_intfc->ReadRecordData(&handle, sizeof(handle));

						auto resolve = a_intfc->ResolveHandle(handle);
						if (!resolve)
						{
							continue;
						}

						std::uint32_t length{ 0 };
						a_intfc->ReadRecordData(&length, sizeof(length));

						std::string string;
						string.reserve(length);
						a_intfc->ReadRecordData(string.data(), length);

						handle = resolve.value();
						GameVM->handlePolicy.PersistHandle(handle);
						map.emplace(
							handle,
							string.c_str());
					}

					return;
				}
			}
		}

	protected:
		template <class... Args>
		void DispatchImpl(Args... a_args)
		{
			auto GameVM = RE::GameVM::GetSingleton();
			if (!GameVM)
			{
				return;
			}

			auto VM = GameVM->GetVM();
			if (!VM)
			{
				return;
			}

			const RE::BSAutoLock lock{ spin };
			for (auto& [handle, string] : map)
			{
				VM->DispatchMethodCall(
					handle,
					string,
					GetEventName(),
					nullptr,
					a_args...);
			}
		}

	private:
		RE::BSSpinLock                                   spin;
		RE::BSTHashMap<std::uint64_t, RE::BSFixedString> map;
	};
}
