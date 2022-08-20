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
					RE::SCRIPT_PARAMETER{"matchstring (optional)", RE::SCRIPT_PARAM_TYPE::kChar, true},
					RE::SCRIPT_PARAMETER{ "filter (optional)",     RE::SCRIPT_PARAM_TYPE::kInt,  true},
					RE::SCRIPT_PARAMETER{ "form type (optional)",  RE::SCRIPT_PARAM_TYPE::kChar, true},
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
			static void ArrayQuickSortRecursive(RE::BSTArray<RE::TESForm*>& a_array, const RE::TESForm::FormSortFunc& a_functor, std::uint32_t a_lowIndex, std::uint32_t a_highIndex)
			{
				using func_t = decltype(&detail::ArrayQuickSortRecursive);
				REL::Relocation<func_t> func{ REL::ID(1361344) };
				return func(a_array, a_functor, a_lowIndex, a_highIndex);
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
					ShowHelp_Globs();
					ShowHelp_Forms();
					ShowHelp_Usage();
					break;

				case 1:
					ShowHelp_Funcs();
					ShowHelp_Usage();
					break;

				case 2:
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
					match = fmt::format(FMT_STRING("{:s} ({:s}) -> {:s}\n"), name, nick, help);
				}
				else
				{
					match = fmt::format(FMT_STRING("{:s} -> {:s}\n"), name, help);
				}
			}
			else
			{
				if (nick && !detail::strempty(nick))
				{
					match = fmt::format(FMT_STRING("{:s} ({:s})\n"), name, nick);
				}
				else
				{
					match = fmt::format(FMT_STRING("{:s}\n"), name);
				}
			}

			RE::ConsoleLog::GetSingleton()->AddString(match.data());
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
			RE::ConsoleLog::GetSingleton()->AddString("----CONSOLE COMMANDS--------------------\n");
			for (auto& iter : RE::SCRIPT_FUNCTION::GetConsoleFunctions())
			{
				// logger::info("cfunc {:s}", iter.functionName);
				ShowHelp_Funcs_Match(iter);
			}

			RE::ConsoleLog::GetSingleton()->AddString("----SCRIPT FUNCTIONS--------------------\n");
			for (auto& iter : RE::SCRIPT_FUNCTION::GetScriptFunctions())
			{
				// logger::info("sfunc {:s}", iter.functionName);
				ShowHelp_Funcs_Match(iter);
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

			auto match = fmt::format(FMT_STRING("{:s} = {:0.2f}\n"), edid, a_global->value);
			RE::ConsoleLog::GetSingleton()->AddString(match.data());
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
			RE::ConsoleLog::GetSingleton()->AddString("----GLOBAL VARIABLES--------------------\n");
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
			auto highIdx = m_Forms.size() - 1;
			auto functor = RE::TESForm::FormSortFunc();
			detail::ArrayQuickSortRecursive(m_Forms, functor, 0, highIdx);

			auto FormEnumString = RE::TESForm::GetFormEnumString();
			for (auto& iter : m_Forms)
			{
				auto form = FormEnumString[iter->formType.underlying()].formString;
				auto edid = iter->GetFormEditorID();
				auto name = RE::TESFullName::GetFullName(*iter);

				auto match = fmt::format(FMT_STRING("{:s}: {:s} ({:08X}) '{:s}'\n"), form, edid, iter->formID, name);
				RE::ConsoleLog::GetSingleton()->AddString(match.data());
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
			RE::ConsoleLog::GetSingleton()->AddString("----OTHER FORMS-------------------------\n");
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
				RE::ConsoleLog::GetSingleton()->AddString("----EXTERIOR CELLS----------------------\n");
				m_ExtCellHeader = true;
			}

			if (!detail::strempty(a_fileName))
			{
				auto match = fmt::format(FMT_STRING("{:s} CELL: {:s}\n"), a_fileName, a_edid);
				RE::ConsoleLog::GetSingleton()->AddString(match.data());
			}
			else
			{
				auto match = fmt::format(FMT_STRING("CELL: {:s}'\n"), a_edid);
				RE::ConsoleLog::GetSingleton()->AddString(match.data());
			}
		}

		static void ShowHelp_Cells_Match(RE::TESFile* a_file)
		{
			if (!a_file->OpenTES(RE::NiFile::OpenMode::kReadOnly, false))
			{
				logger::warn(FMT_STRING("failed to open file: {:s}"), a_file->filename);
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
								gotEDID = a_file->GetChunkData(edid);
								if (gotEDID && gotDATA && ((data & 1) == 0))
								{
									m_CellMap.insert_or_assign(
										std::make_pair(cidx, edid),
										a_file->filename);
									continue;
								}
								break;

							case 'ATAD':
								gotDATA = a_file->GetChunkData(&data, 0);
								if (gotEDID && gotDATA && ((data & 1) == 0))
								{
									m_CellMap.insert_or_assign(
										std::make_pair(cidx, edid),
										a_file->filename);
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
				logger::warn(FMT_STRING("failed to close file: {:s}"), a_file->filename);
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
			RE::ConsoleLog::GetSingleton()->AddString("usage: help <matchstring> <filter> <form type>\n");
			RE::ConsoleLog::GetSingleton()->AddString("filters: 0-all 1-functions, 2-settings, 3-globals, 4-other forms\n");
			RE::ConsoleLog::GetSingleton()->AddString("form type is 4 characters and is ignored unless the filter is 4.\n");
		}

	protected:
		inline static char m_MatchString[512];
		inline static char m_FormTFilter[512];
		inline static bool m_ExtCellHeader{ false };

		inline static RE::BSTArray<RE::TESForm*> m_Forms;
		inline static std::map<std::pair<std::uint32_t, const std::string>, std::string_view> m_CellMap;
	};
}
