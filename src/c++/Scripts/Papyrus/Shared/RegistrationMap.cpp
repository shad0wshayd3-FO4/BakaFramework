#include "RegistrationMap.h"

namespace Papyrus
{
	namespace Impl
	{
		RegistrationMapBase::~RegistrationMapBase()
		{
			Clear();
		}

		bool RegistrationMapBase::Save(const F4SE::SerializationInterface* a_intfc)
		{
			const RE::BSAutoLock locker{ _lock };
			if (!a_intfc->OpenRecord(GetRecordType(), GetRecordVersion()))
			{
				logger::warn(FMT_STRING("{:s}: Failed to create record."), GetRecordName());
				return false;
			}

			std::uint32_t count{ 0 }, rsize{ _events.size() };
			if (!a_intfc->WriteRecordData(&rsize, sizeof(rsize)))
			{
				logger::warn(FMT_STRING("{:s}: Failed to write record size."), GetRecordName());
				return false;
			}

			for (auto& [handle, name] : _events)
			{
				if (!a_intfc->WriteRecordData(&handle, sizeof(handle)))
				{
					logger::warn(FMT_STRING("{:s}: Failed to write registration hndl (idx#{:d}/{:d})."), GetRecordName(), count, rsize);
					return false;
				}

				auto nsze = name.size() + 1;
				if (!a_intfc->WriteRecordData(&nsze, sizeof(nsze)) || !a_intfc->WriteRecordData(name.data(), nsze))
				{
					logger::warn(FMT_STRING("{:s}: Failed to write registration data (idx#{:d}/{:d})."), GetRecordName(), count, rsize);
					return false;
				}

				count++;
			}

			logger::info(FMT_STRING("{:s}: Saved {:d}/{:d} event registrations."), GetRecordName(), count, rsize);
			return true;
		}

		bool RegistrationMapBase::Load(const F4SE::SerializationInterface* a_intfc)
		{
			std::uint32_t type{ 0 }, version{ 0 }, length{ 0 };
			while (a_intfc->GetNextRecordInfo(type, version, length))
			{
				if (type == GetRecordType())
				{
					return LoadImpl(a_intfc);
				}
			}

			return true;
		}

		bool RegistrationMapBase::LoadImpl(const F4SE::SerializationInterface* a_intfc)
		{
			std::uint32_t count{ 0 }, rsize{ 0 };
			if (sizeof(rsize) != a_intfc->ReadRecordData(&rsize, sizeof(rsize)))
			{
				logger::warn(FMT_STRING("{:s}: Failed to read record size."), GetRecordName());
				return false;
			}

			std::string name;
			_events.clear();

			const RE::BSAutoLock locker{ _lock };
			for (std::uint32_t i = 0; i < rsize; i++)
			{
				std::uint64_t hndl{ 0 };
				if (sizeof(hndl) != a_intfc->ReadRecordData(&hndl, sizeof(hndl)))
				{
					logger::warn(FMT_STRING("{:s}: Failed to read registration hndl (idx#{:d}/{:d})."), GetRecordName(), count, rsize);
					return false;
				}

				std::uint32_t nsze{ 0 };
				if (sizeof(nsze) != a_intfc->ReadRecordData(&nsze, sizeof(nsze)))
				{
					logger::warn(FMT_STRING("{:s}: Failed to read registration size (idx#{:d}/{:d})."), GetRecordName(), count, rsize);
					return false;
				}

				name.reserve(nsze);
				if (nsze != a_intfc->ReadRecordData(name.data(), nsze))
				{
					logger::warn(FMT_STRING("{:s}: Failed to read registration data (idx#{:d}/{:d})."), GetRecordName(), count, rsize);
					return false;
				}

				_events.emplace(a_intfc->ResolveHandle(hndl).value_or(hndl), name.c_str());
				count++;
			}

			logger::info(FMT_STRING("{:s}: Loaded {:d}/{:d} event registrations."), GetRecordName(), count, rsize);
			return true;
		}

		void RegistrationMapBase::Clear()
		{
			if (auto GameVM{ RE::GameVM::GetSingleton() }; GameVM)
			{
				for (auto& _event : _events)
				{
					GameVM->handlePolicy.ReleaseHandle(_event.first);
				}
			}

			_events.clear();
		}

		void RegistrationMapBase::Register(const RE::BSScript::Object& a_object)
		{
			auto GameVM{ RE::GameVM::GetSingleton() };
			if (!a_object.type || a_object.GetHandle() == GameVM->handlePolicy.EmptyHandle())
			{
				GameVM->impl->PostError("Cannot register unbound script to recieve pipboy light events"sv, 0, RE::BSScript::ErrorLogger::Severity::kError);
				return;
			}

			const RE::BSAutoLock locker{ _lock };
			auto result = _events.emplace(a_object.GetHandle(), a_object.type->name);
			if (result.second)
			{
				GameVM->handlePolicy.PersistHandle(a_object.GetHandle());
			}
		}

		void RegistrationMapBase::Unregister(const RE::BSScript::Object& a_object)
		{
			auto GameVM{ RE::GameVM::GetSingleton() };
			if (a_object.GetHandle() != GameVM->handlePolicy.EmptyHandle())
			{
				GameVM->handlePolicy.ReleaseHandle(a_object.GetHandle());
			}

			const RE::BSAutoLock locker{ _lock };
			_events.erase(a_object.GetHandle());
		}
	}
}
