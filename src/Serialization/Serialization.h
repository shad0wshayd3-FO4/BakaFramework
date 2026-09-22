#pragma once

namespace Serialization
{
	enum : std::uint32_t
	{
		kSerializationVersion = 3,
	};

	class EventManager :
		public REX::TSingleton<EventManager>
	{
	public:
		enum : std::uint32_t
		{
			kPipboyLight = 'PLEH',
			kRadiationCount = 'RCEH',
		};

		void Save(const F4SE::SerializationInterface* a_intfc)
		{
			pipboyLight.Save(a_intfc, kPipboyLight, kSerializationVersion);
			radiationCount.Save(a_intfc, kRadiationCount, kSerializationVersion);
		}

		void Load(const F4SE::SerializationInterface* a_intfc)
		{
			std::uint32_t type;
			std::uint32_t version;
			std::uint32_t length;
			while (a_intfc->GetNextRecordInfo(type, version, length))
			{
				if (version != kSerializationVersion)
					continue;

				switch (type)
				{
				case kPipboyLight:
					pipboyLight.Load(a_intfc);
					break;
				case kRadiationCount:
					radiationCount.Load(a_intfc);
					break;
				default:
					break;
				}
			}
		}

		void Revert(const F4SE::SerializationInterface* a_intfc)
		{
			pipboyLight.Revert(a_intfc);
			radiationCount.Revert(a_intfc);
		}

		void FormDelete(std::uint64_t a_handle)
		{
			pipboyLight.Unregister(a_handle);
			radiationCount.Unregister(a_handle);
		}

		F4SE::RegistrationSet<bool>         pipboyLight{ "OnPipboyLight"sv };
		F4SE::RegistrationSet<std::int32_t> radiationCount{ "OnRadiationCount"sv };
	};
}

namespace Serialization
{
	void SaveCallback(const F4SE::SerializationInterface* a_intfc)
	{
		EventManager::GetSingleton()->Save(a_intfc);
	}

	void LoadCallback(const F4SE::SerializationInterface* a_intfc)
	{
		EventManager::GetSingleton()->Load(a_intfc);
	}

	void RevertCallback(const F4SE::SerializationInterface* a_intfc)
	{
		EventManager::GetSingleton()->Revert(a_intfc);
	}

	void FormDeleteCallback(std::uint64_t a_handle)
	{
		EventManager::GetSingleton()->FormDelete(a_handle);
	}
}
