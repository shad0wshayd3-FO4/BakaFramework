#include "Scripts/ObScript/Help.h"

namespace ObScript::Help
{
	namespace IMPL
	{
		bool Contains(std::string_view a_str, std::string_view a_sub)
		{
			if (a_str.empty())
			{
				return false;
			}

			if (a_sub.empty())
			{
				return true;
			}

			auto it = std::search(
				a_str.begin(),
				a_str.end(),
				a_sub.begin(),
				a_sub.end(),
				[](char a_lhs, char a_rhs)
				{
					return std::tolower(a_lhs) == std::tolower(a_rhs);
				});
			return it != a_str.end();
		}

		void Print(std::string_view a_msg)
		{
			if (auto ConsoleLog = RE::ConsoleLog::GetSingleton())
			{
				ConsoleLog->PrintLine(a_msg.data());
			}
		}
	}

	namespace FORM
	{
		struct output_t
		{
			bool operator<(const output_t& a_rhs) const
			{
				if (code != a_rhs.code)
					return code < a_rhs.code;
				if (edid != a_rhs.edid)
					return edid < a_rhs.edid;
				return form < a_rhs.form;
			}

			std::string_view edid;
			std::string_view name;
			std::string_view type;
			std::uint32_t    code{ 0 };
			std::uint32_t    form{ 0 };
		};
		std::vector<output_t> FormsToPrint;

		void Match(const std::string_view& a_name, RE::TESForm* a_form)
		{
			switch (a_form->GetFormType())
			{
			case RE::ENUM_FORM_ID::kGLOB:
				return;
			case RE::ENUM_FORM_ID::kCELL:
				if (auto cell = a_form->As<RE::TESObjectCELL>();
					cell && cell->IsExterior())
				{
					return;
				}
				break;
			default:
				break;
			}

			std::string_view edid = a_form->GetFormEditorID();
			std::string_view name = RE::TESFullName::GetFullName(*a_form);

			if (IMPL::Contains(edid, a_name) || IMPL::Contains(name, a_name))
			{
				output_t output;
				output.edid = edid;
				output.name = name;
				output.type = a_form->GetFormTypeString();
				output.code = a_form->GetFormCode();
				output.form = a_form->GetFormID();
				FormsToPrint.emplace_back(output);
			}
		}

		void Print()
		{
			std::sort(FormsToPrint.begin(), FormsToPrint.end());
			for (auto& iter : FormsToPrint)
			{
				std::stringstream ss;
				ss << std::format("{}: {} ({:08X})"sv, iter.type, iter.edid, iter.form);
				if (!iter.name.empty())
					ss << std::format(" '{}'"sv, iter.name);
				IMPL::Print(ss.str());
			}
		}

		namespace NONE
		{
			void Print(const std::string_view& a_name, const RE::ENUM_FORM_ID& a_type = RE::ENUM_FORM_ID::kNONE)
			{
				auto [map, lock] = RE::TESForm::GetAllForms();
				for (auto& [formID, iter] : *map)
				{
					if (a_type != RE::ENUM_FORM_ID::kNONE)
						if (a_type != iter->GetFormType())
							continue;

					FORM::Match(a_name, iter);
				}

				FORM::Print();
			}
		}

		namespace TYPE
		{
			void Print(const std::string_view& a_name, const RE::ENUM_FORM_ID& a_type)
			{
				if (auto dataHandler = RE::TESDataHandler::GetSingleton())
				{
					for (auto iter : dataHandler->formArrays[std::to_underlying(a_type)])
					{
						FORM::Match(a_name, iter);
					}
				}

				FORM::Print();
			}
		}

		namespace CELL
		{
			struct exterior_t
			{
				bool operator<(const exterior_t& a_rhs) const
				{
					if (cidx != a_rhs.cidx)
						return cidx < a_rhs.cidx;
					return edid < a_rhs.edid;
				}

				std::string   edid;
				std::string   name;
				std::uint32_t cidx{ 0 };
			};
			std::vector<exterior_t> ExteriorCells;

			void Match(RE::TESFile* a_file)
			{
				if (!a_file->OpenTES(RE::NiFile::OpenMode::kReadOnly, false))
				{
					return;
				}

				do
				{
					if (a_file->currentform.form == 'LLEC')
					{
						std::uint32_t cidx{ 0 };
						cidx += a_file->compileIndex << 24;
						cidx += a_file->smallFileCompileIndex << 12;

						std::uint16_t data{ 0 };
						bool          dataFound{ false };
						char          edid[512]{ '\0' };
						bool          edidFound{ false };

						do
						{
							const auto size = a_file->actualChunkSize;
							switch (a_file->GetTESChunk())
							{
							case 'ATAD':
								dataFound = a_file->GetChunkData(&data, size);
								break;
							case 'DIDE':
								edidFound = a_file->GetChunkData(edid, size);
								break;
							default:
								break;
							}

							if (edidFound && dataFound && ((data & 1) == 0))
							{
								if (cidx > a_file->currentform.formID)
									break;

								exterior_t output;
								output.edid = edid;
								output.name = a_file->GetFilename();
								output.cidx = cidx;
								ExteriorCells.emplace_back(output);
								break;
							}
						}
						while (a_file->NextChunk());
					}
				}
				while (a_file->NextForm(true));
				(void)a_file->CloseTES(false);
			}

			void Print(const std::string_view& a_name)
			{
				IMPL::Print("----EXTERIOR CELLS----------------------"sv);

				std::sort(ExteriorCells.begin(), ExteriorCells.end());
				for (auto& iter : ExteriorCells)
				{
					if (IMPL::Contains(iter.edid, a_name))
					{
						std::stringstream ss;
						if (!iter.name.empty())
							ss << std::format("{} "sv, iter.name);
						ss << std::format("CELL: {}"sv, iter.edid);

						IMPL::Print(ss.str());
					}
				}
			}

			void Build()
			{
				ExteriorCells.clear();

				if (auto dataHandler = RE::TESDataHandler::GetSingleton())
				{
					for (auto iter : dataHandler->compiledFileCollection.files)
					{
						Match(iter);
					}

					for (auto iter : dataHandler->compiledFileCollection.smallFiles)
					{
						Match(iter);
					}
				}
			}
		}

		void Print(const std::string_view& a_name, RE::ENUM_FORM_ID a_type)
		{
			FormsToPrint.clear();

			IMPL::Print("----OTHER FORMS-------------------------"sv);
			switch (a_type)
			{
			case RE::ENUM_FORM_ID::kNONE:
				NONE::Print(a_name);
				CELL::Print(a_name);
				return;
			case RE::ENUM_FORM_ID::kGLOB:
				// GLOB::Print(a_name);
				return;
			case RE::ENUM_FORM_ID::kCELL:
				NONE::Print(a_name, a_type);
				CELL::Print(a_name);
				return;
			default:
				TYPE::Print(a_name, a_type);
				return;
			}
		}
	}

	namespace FUNC
	{
		void Match(const std::string_view& a_name, RE::SCRIPT_FUNCTION a_function)
		{
			std::string_view name = a_function.functionName;
			if (name.empty())
			{
				return;
			}

			std::string_view nick = a_function.shortName;
			std::string_view help = a_function.helpString;
			if (IMPL::Contains(name, a_name) || IMPL::Contains(nick, a_name) || IMPL::Contains(help, a_name))
			{
				std::stringstream ss;
				ss << name;

				if (!nick.empty())
					ss << std::format(" ({})"sv, nick);
				if (!help.empty())
					ss << std::format(" > {}"sv, help);

				IMPL::Print(ss.str());
			}
		}

		void Print(const std::string_view& a_name)
		{
			IMPL::Print("----CONSOLE COMMANDS--------------------"sv);
			for (auto& command : RE::SCRIPT_FUNCTION::GetConsoleFunctions())
			{
				Match(a_name, command);
			}

			IMPL::Print("----SCRIPT FUNCTIONS--------------------"sv);
			for (auto& command : RE::SCRIPT_FUNCTION::GetScriptFunctions())
			{
				Match(a_name, command);
			}
		}
	}

	namespace GLOB
	{
		void Match(const std::string_view& a_name, RE::TESGlobal* a_global)
		{
			std::string_view editorID = a_global->GetFormEditorID();
			if (IMPL::Contains(editorID, a_name))
			{
				auto ss = std::format("{} = {:0.2f}"sv, editorID, a_global->value);
				IMPL::Print(ss);
			}
		}

		void Print(const std::string_view& a_name)
		{
			IMPL::Print("----GLOBAL VARIABLES--------------------"sv);
			if (auto dataHandler = RE::TESDataHandler::GetSingleton())
			{
				for (auto iter : dataHandler->GetFormArray<RE::TESGlobal>())
				{
					Match(a_name, iter);
				}
			}
		}
	}

	namespace GMST
	{
		void Match(const std::string_view& a_name, RE::Setting* a_setting)
		{
			std::string_view name = a_setting->GetKey();
			if (IMPL::Contains(name, a_name))
			{
				std::stringstream ss;
				ss << name;

				switch (a_setting->GetType())
				{
				case RE::Setting::SETTING_TYPE::kBinary:
					ss << std::format(" = {}"sv,
						a_setting->GetBinary());
					break;
				case RE::Setting::SETTING_TYPE::kChar:
					ss << std::format(" = {}"sv,
						a_setting->GetChar());
					break;
				case RE::Setting::SETTING_TYPE::kUChar:
					ss << std::format(" = {}"sv,
						a_setting->GetUChar());
					break;
				case RE::Setting::SETTING_TYPE::kInt:
					ss << std::format(" = {}"sv,
						a_setting->GetInt());
					break;
				case RE::Setting::SETTING_TYPE::kUInt:
					ss << std::format(" = {}"sv,
						a_setting->GetUInt());
					break;
				case RE::Setting::SETTING_TYPE::kFloat:
					ss << std::format(" = {:0.2f}"sv,
						a_setting->GetFloat());
					break;
				case RE::Setting::SETTING_TYPE::kString:
					ss << std::format(" = {}"sv,
						a_setting->GetString());
					break;
				case RE::Setting::SETTING_TYPE::kRGB:
					ss << std::format(" = {}"sv,
						a_setting->GetRGB());
					break;
				case RE::Setting::SETTING_TYPE::kRGBA:
					ss << std::format(" = {}"sv,
						a_setting->GetRGBA());
					break;
				default:
					ss << " = <UNKNOWN>"sv;
					break;
				}

				IMPL::Print(ss.str());
			}
		}

		void Print(const std::string_view& a_name)
		{
			IMPL::Print("----GAME SETTINGS-----------------------"sv);
			if (auto gmstSettings = RE::GameSettingCollection::GetSingleton())
			{
				for (auto& [name, setting] : gmstSettings->settings)
				{
					Match(a_name, setting);
				}
			}

			IMPL::Print("----INI SETTINGS------------------------"sv);
			if (auto mainSettings = RE::INISettingCollection::GetSingleton())
			{
				if (auto prefSettings = RE::INIPrefSettingCollection::GetSingleton())
				{
					for (auto mainSetting : mainSettings->settings)
					{
						auto prefSetting = prefSettings->GetSetting(mainSetting->GetKey());
						Match(a_name, prefSetting ? prefSetting : mainSetting);
					}
				}
			}
		}
	}

	namespace HELP
	{
		void Print()
		{
			IMPL::Print("usage: help <matchstring> <filter> <form type>"sv);
			IMPL::Print("filters: 0-all 1-functions, 2-settings, 3-globals, 4-other forms"sv);
			IMPL::Print("form type is 4 characters and is ignored unless the filter is 4."sv);
		}
	}
}
