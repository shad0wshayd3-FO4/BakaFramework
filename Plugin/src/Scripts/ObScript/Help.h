#pragma once

namespace ObScript
{
	class Help
	{
	public:
		static void Install()
		{
			const auto functions = RE::SCRIPT_FUNCTION::GetConsoleFunctions();
			const auto it = std::find_if(
				functions.begin(),
				functions.end(),
				[&](auto&& a_elem)
				{
					return _stricmp(a_elem.functionName, "Help") == 0;
				});

			if (it != functions.end())
			{
				static std::array params{
					RE::SCRIPT_PARAMETER{ "matchstring (optional)", RE::SCRIPT_PARAM_TYPE::kChar, true },
					RE::SCRIPT_PARAMETER{ "filter (optional)", RE::SCRIPT_PARAM_TYPE::kInt, true },
					RE::SCRIPT_PARAMETER{ "form type (optional)", RE::SCRIPT_PARAM_TYPE::kChar, true },
				};

				*it = RE::SCRIPT_FUNCTION{ "Help", "", it->output };
				it->helpString = "Show this help.";
				it->paramCount = static_cast<std::uint16_t>(params.size());
				it->parameters = params.data();
				it->executeFunction = Execute;

				logger::debug("Registered Help."sv);
			}
			else
			{
				logger::debug("Failed to register Help."sv);
			}
		}

		static void ClearCellMap()
		{
			m_CellMap.clear();
		}

	private:
		class detail
		{
		public:
			static void SortFormArray(std::vector<RE::TESForm*>& a_forms)
			{
				std::sort(
					a_forms.begin(),
					a_forms.end(),
					[](const RE::TESForm* a_lhs, const RE::TESForm* a_rhs)
					{
						auto FormEnumString = RE::TESForm::GetFormEnumString();
						auto L_FORM = FormEnumString[a_lhs->formType.underlying()].formCode;
						auto R_FORM = FormEnumString[a_rhs->formType.underlying()].formCode;
						if (L_FORM != R_FORM)
						{
							return L_FORM < R_FORM;
						}

						auto L_EDID = a_lhs->GetFormEditorID();
						auto R_EDID = a_rhs->GetFormEditorID();
						auto C_EDID = _stricmp(L_EDID, R_EDID);
						if (C_EDID != 0)
						{
							return C_EDID < 0;
						}

						return a_lhs->formID < a_rhs->formID;
					});
			}

			static bool strempty(const std::string_view& a_string)
			{
				return a_string.empty();
			}

			static bool strvicmp(const std::string_view& a_string, const std::string_view& a_matchString)
			{
				auto it = std::search(
					a_string.begin(),
					a_string.end(),
					a_matchString.begin(),
					a_matchString.end(),
					[](char a_s, char a_m)
					{ return std::tolower(a_s) == std::tolower(a_m); });
				return it != a_string.end();
			}
		};

		static bool Execute(
			const RE::SCRIPT_PARAMETER* a_parameters,
			const char* a_compiledParams,
			RE::TESObjectREFR* a_refObject,
			RE::TESObjectREFR* a_container,
			RE::Script* a_script,
			RE::ScriptLocals* a_scriptLocals,
			float&,
			std::uint32_t& a_offset)
		{
			m_MatchString[0] = '\0';
			m_FormTFilter[0] = '\0';
			m_ExtCellHeader = false;
			m_Forms.clear();

			std::uint32_t idx{ 0 };
			RE::Script::ParseParameters(
				a_parameters,
				a_compiledParams,
				a_offset,
				a_refObject,
				a_container,
				a_script,
				a_scriptLocals,
				m_MatchString,
				&idx,
				m_FormTFilter);

			if (detail::strempty(m_MatchString))
			{
				ShowHelp_Usage();
				return true;
			}

			switch (idx)
			{
				case 0:
					ShowHelp_Funcs();
					ShowHelp_Settings();
					ShowHelp_Globs();
					ShowHelp_Forms();
					ShowHelp_Usage();
					break;

				case 1:
					ShowHelp_Funcs();
					ShowHelp_Usage();
					break;

				case 2:
					ShowHelp_Settings();
					ShowHelp_Usage();
					break;

				case 3:
					ShowHelp_Globs();
					ShowHelp_Usage();
					break;

				case 4:
					ShowHelp_Forms();
					ShowHelp_Usage();
					break;

				default:
					ShowHelp_Usage();
					break;
			}

			return true;
		}

		static void ShowHelp_Funcs_Print(RE::SCRIPT_FUNCTION a_function)
		{
			auto name = a_function.functionName;
			auto nick = a_function.shortName;
			auto help = a_function.helpString;

			if (!name)
			{
				return;
			}

			if (detail::strempty(name))
			{
				return;
			}

			std::string match;
			if (help && !detail::strempty(help))
			{
				if (nick && !detail::strempty(nick))
				{
					match = fmt::format(FMT_STRING("{:s} ({:s}) -> {:s}"sv), name, nick, help);
				}
				else
				{
					match = fmt::format(FMT_STRING("{:s} -> {:s}"sv), name, help);
				}
			}
			else
			{
				if (nick && !detail::strempty(nick))
				{
					match = fmt::format(FMT_STRING("{:s} ({:s})"sv), name, nick);
				}
				else
				{
					match = fmt::format(FMT_STRING("{:s}"sv), name);
				}
			}

			RE::ConsoleLog::GetSingleton()->PrintLine(match.data());
		}

		static void ShowHelp_Funcs_Match(RE::SCRIPT_FUNCTION a_function)
		{
			if (detail::strempty(m_MatchString))
			{
				ShowHelp_Funcs_Print(a_function);
				return;
			}

			auto name = a_function.functionName;
			auto nick = a_function.shortName;
			auto help = a_function.helpString;

			if ((name && detail::strvicmp(name, m_MatchString)) || (nick && detail::strvicmp(nick, m_MatchString)) || (help && detail::strvicmp(help, m_MatchString)))
			{
				ShowHelp_Funcs_Print(a_function);
			}
		}

		static void ShowHelp_Funcs()
		{
			RE::ConsoleLog::GetSingleton()->PrintLine("----CONSOLE COMMANDS--------------------");
			for (auto& iter : RE::SCRIPT_FUNCTION::GetConsoleFunctions())
			{
				ShowHelp_Funcs_Match(iter);
			}

			RE::ConsoleLog::GetSingleton()->PrintLine("----SCRIPT FUNCTIONS--------------------");
			for (auto& iter : RE::SCRIPT_FUNCTION::GetScriptFunctions())
			{
				ShowHelp_Funcs_Match(iter);
			}
		}

		static void ShowHelp_Settings_Print(RE::Setting* a_setting)
		{
			std::string match;
			switch (a_setting->GetType())
			{
				case RE::Setting::SETTING_TYPE::kBinary:
					match = fmt::format(
						FMT_STRING("{:s} = {:s}"sv),
						a_setting->GetKey(),
						a_setting->GetBinary());
					break;

				case RE::Setting::SETTING_TYPE::kChar:
					match = fmt::format(
						FMT_STRING("{:s} = {:d}"sv),
						a_setting->GetKey(),
						a_setting->GetChar());
					break;

				case RE::Setting::SETTING_TYPE::kUChar:
					match = fmt::format(
						FMT_STRING("{:s} = {:d}"sv),
						a_setting->GetKey(),
						a_setting->GetUChar());
					break;

				case RE::Setting::SETTING_TYPE::kInt:
					match = fmt::format(
						FMT_STRING("{:s} = {:d}"sv),
						a_setting->GetKey(),
						a_setting->GetInt());
					break;

				case RE::Setting::SETTING_TYPE::kUInt:
					match = fmt::format(
						FMT_STRING("{:s} = {:d}"sv),
						a_setting->GetKey(),
						a_setting->GetUInt());
					break;

				case RE::Setting::SETTING_TYPE::kFloat:
					match = fmt::format(
						FMT_STRING("{:s} = {:0.2f}"sv),
						a_setting->GetKey(),
						a_setting->GetFloat());
					break;

				case RE::Setting::SETTING_TYPE::kString:
					match = fmt::format(
						FMT_STRING("{:s} = {:s}"sv),
						a_setting->GetKey(),
						a_setting->GetString());
					break;

				case RE::Setting::SETTING_TYPE::kRGB:
					{
						auto color = a_setting->GetRGB();
						match = fmt::format(
							FMT_STRING("{:s} = R:{:d} G:{:d} B:{:d}"sv),
							a_setting->GetKey(),
							color[0],
							color[1],
							color[2]);
					}
					break;

				case RE::Setting::SETTING_TYPE::kRGBA:
					{
						auto color = a_setting->GetRGBA();
						match = fmt::format(
							FMT_STRING("{:s} = R:{:d} G:{:d} B:{:d} A:{:d}"sv),
							a_setting->GetKey(),
							color[0],
							color[1],
							color[2],
							color[3]);
					}
					break;

				default:
					match = fmt::format(
						FMT_STRING("{:s} = <UNKNOWN>"sv),
						a_setting->GetKey());
					break;
			}

			RE::ConsoleLog::GetSingleton()->PrintLine(match.data());
		}

		static void ShowHelp_Settings_Match(RE::Setting* a_setting)
		{
			if (detail::strempty(m_MatchString))
			{
				ShowHelp_Settings_Print(a_setting);
				return;
			}

			auto name = a_setting->GetKey();
			if (!name.empty() && detail::strvicmp(name, m_MatchString))
			{
				ShowHelp_Settings_Print(a_setting);
			}
		}

		static void ShowHelp_Settings()
		{
			RE::ConsoleLog::GetSingleton()->PrintLine("----GAME SETTINGS-----------------------");
			if (auto GameSettingCollection = RE::GameSettingCollection::GetSingleton())
			{
				for (auto& iter : GameSettingCollection->settings)
				{
					ShowHelp_Settings_Match(iter.second);
				}
			}

			RE::ConsoleLog::GetSingleton()->PrintLine("----INI SETTINGS------------------------");
			if (auto INISettingCollection = RE::INISettingCollection::GetSingleton())
			{
				if (auto INIPrefSettingCollection = RE::INIPrefSettingCollection::GetSingleton())
				{
					for (auto& iter : INISettingCollection->settings)
					{
						if (auto it = std::find_if(
								INIPrefSettingCollection->settings.begin(),
								INIPrefSettingCollection->settings.end(),
								[&](auto&& a_elem)
								{
									return iter->GetKey() == a_elem->GetKey();
								});
						    it != INIPrefSettingCollection->settings.end())
						{
							ShowHelp_Settings_Match(*it);
							continue;
						}

						ShowHelp_Settings_Match(iter);
					}
				}
			}
		}

		static void ShowHelp_Globs_Print(RE::TESGlobal* a_global)
		{
			auto edid = a_global->GetFormEditorID();
			if (!edid)
			{
				return;
			}

			if (detail::strempty(edid))
			{
				return;
			}

			auto match = fmt::format(FMT_STRING("{:s} = {:0.2f}"sv), edid, a_global->value);
			RE::ConsoleLog::GetSingleton()->PrintLine(match.data());
		}

		static void ShowHelp_Globs_Match(RE::TESGlobal* a_global)
		{
			if (detail::strempty(m_MatchString))
			{
				ShowHelp_Globs_Print(a_global);
				return;
			}

			auto edid = a_global->GetFormEditorID();
			if (edid && detail::strvicmp(edid, m_MatchString))
			{
				ShowHelp_Globs_Print(a_global);
			}
		}

		static void ShowHelp_Globs()
		{
			RE::ConsoleLog::GetSingleton()->PrintLine("----GLOBAL VARIABLES--------------------");
			if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
			{
				for (auto& iter : TESDataHandler->GetFormArray<RE::TESGlobal>())
				{
					ShowHelp_Globs_Match(iter);
				}
			}
		}

		static void ShowHelp_Forms_Print()
		{
			detail::SortFormArray(m_Forms);

			auto FormEnumString = RE::TESForm::GetFormEnumString();
			for (auto& iter : m_Forms)
			{
				auto form = FormEnumString[iter->formType.underlying()].formString;
				auto edid = iter->GetFormEditorID();
				auto name = RE::TESFullName::GetFullName(*iter);

				auto match = fmt::format(FMT_STRING("{:s}: {:s} ({:08X}) '{:s}'"sv), form, edid, iter->formID, name);
				RE::ConsoleLog::GetSingleton()->PrintLine(match.data());
			}
		}

		static void ShowHelp_Forms_Match(RE::TESForm* a_form)
		{
			switch (a_form->GetFormType())
			{
				case RE::ENUM_FORM_ID::kGLOB:
					break;

				case RE::ENUM_FORM_ID::kCELL:
					{
						auto cell = a_form->As<RE::TESObjectCELL>();
						if (cell && cell->IsExterior())
						{
							break;
						}
					}

				default:
					{
						auto edid = a_form->GetFormEditorID();
						auto name = RE::TESFullName::GetFullName(*a_form);

						if ((edid && detail::strvicmp(edid, m_MatchString)) || (!name.empty() && detail::strvicmp(name, m_MatchString)))
						{
							m_Forms.emplace_back(a_form);
						}
					}
					break;
			}
		}

		static void ShowHelp_Forms()
		{
			RE::ConsoleLog::GetSingleton()->PrintLine("----OTHER FORMS-------------------------");
			if (auto TESDataHandler = RE::TESDataHandler::GetSingleton())
			{
				auto formType = RE::TESForm::GetFormTypeFromString(m_FormTFilter);
				if (formType == RE::ENUM_FORM_ID::kGLOB)
				{
					return;
				}

				if (formType != RE::ENUM_FORM_ID::kNONE && formType != RE::ENUM_FORM_ID::kCELL)
				{
					auto& forms = TESDataHandler->formArrays[stl::to_underlying(formType)];
					for (auto iter : forms)
					{
						ShowHelp_Forms_Match(iter);
					}
				}
				else
				{
					auto [forms, lock] = RE::TESForm::GetAllForms();
					for (auto& iter : *forms)
					{
						if (formType == RE::ENUM_FORM_ID::kCELL && iter.second->formType != RE::ENUM_FORM_ID::kCELL)
						{
							continue;
						}

						ShowHelp_Forms_Match(iter.second);
					}
				}

				if (m_Forms.size())
				{
					ShowHelp_Forms_Print();
				}

				if (formType == RE::ENUM_FORM_ID::kNONE || formType == RE::ENUM_FORM_ID::kCELL)
				{
					ShowHelp_Cells();
				}
			}
		}

		static void ShowHelp_Cells_Print(const std::string_view& a_edid, const std::string_view& a_fileName)
		{
			if (!m_ExtCellHeader)
			{
				RE::ConsoleLog::GetSingleton()->PrintLine("----EXTERIOR CELLS----------------------");
				m_ExtCellHeader = true;
			}

			if (!detail::strempty(a_fileName))
			{
				auto match = fmt::format(FMT_STRING("{:s} CELL: {:s}"sv), a_fileName, a_edid);
				RE::ConsoleLog::GetSingleton()->PrintLine(match.data());
			}
			else
			{
				auto match = fmt::format(FMT_STRING("CELL: {:s}"sv), a_edid);
				RE::ConsoleLog::GetSingleton()->PrintLine(match.data());
			}
		}

		static void ShowHelp_Cells_Match(RE::TESFile* a_file)
		{
			if (!a_file->OpenTES(RE::NiFile::OpenMode::kReadOnly, false))
			{
				logger::warn(FMT_STRING("failed to open file: {:s}"sv), a_file->filename);
				return;
			}

			do
			{
				if (a_file->currentform.form == 'LLEC')
				{
					char edid[512]{ '\0' };
					bool gotEDID{ false };

					std::uint16_t data{ 0 };
					bool gotDATA{ false };

					std::uint32_t cidx{ 0 };
					cidx += a_file->compileIndex << 24;
					cidx += a_file->smallFileCompileIndex << 12;

					do
					{
						switch (a_file->GetTESChunk())
						{
							case 'DIDE':
								gotEDID = a_file->GetChunkData(edid, a_file->actualChunkSize);
								if (gotEDID && gotDATA && ((data & 1) == 0))
								{
									m_CellMap.insert_or_assign(std::make_pair(cidx, edid), a_file->filename);
									continue;
								}
								break;

							case 'ATAD':
								gotDATA = a_file->GetChunkData(&data, a_file->actualChunkSize);
								if (gotEDID && gotDATA && ((data & 1) == 0))
								{
									m_CellMap.insert_or_assign(std::make_pair(cidx, edid), a_file->filename);
									continue;
								}
								break;

							default:
								break;
						}
					}
					while (a_file->NextChunk());
				}
			}
			while (a_file->NextForm(true));

			if (!a_file->CloseTES(false))
			{
				logger::warn(FMT_STRING("failed to close file: {:s}"sv), a_file->filename);
			}
		}

		static void ShowHelp_Cells_Build()
		{
			auto TESDataHandler = RE::TESDataHandler::GetSingleton();
			for (auto iter : TESDataHandler->compiledFileCollection.files)
			{
				ShowHelp_Cells_Match(iter);
			}

			for (auto iter : TESDataHandler->compiledFileCollection.smallFiles)
			{
				ShowHelp_Cells_Match(iter);
			}
		}

		static void ShowHelp_Cells()
		{
			if (m_CellMap.empty())
			{
				ShowHelp_Cells_Build();
			}

			for (auto& iter : m_CellMap)
			{
				if (detail::strvicmp(iter.first.second, m_MatchString))
				{
					ShowHelp_Cells_Print(iter.first.second, iter.second);
				}
			}
		}

		static void ShowHelp_Usage()
		{
			RE::ConsoleLog::GetSingleton()->PrintLine("usage: help <matchstring> <filter> <form type>");
			RE::ConsoleLog::GetSingleton()->PrintLine("filters: 0-all 1-functions, 2-settings, 3-globals, 4-other forms");
			RE::ConsoleLog::GetSingleton()->PrintLine("form type is 4 characters and is ignored unless the filter is 4.");
		}

	protected:
		inline static char m_MatchString[512];
		inline static char m_FormTFilter[512];
		inline static bool m_ExtCellHeader{ false };

		inline static std::vector<RE::TESForm*> m_Forms;
		inline static std::map<std::pair<std::uint32_t, const std::string>, std::string_view> m_CellMap;
	};
}
