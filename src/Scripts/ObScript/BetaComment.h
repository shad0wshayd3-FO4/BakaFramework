#pragma once

#include "Config/Config.h"

namespace ObScript::BetaComment
{
	/*
		%D (%R)		| Current Time
		%s			| File Name
		%D (%R)		| File Time
		%s			| Machine Name
		%08X		| FormID
		%s			| Form Name
		%s			| Cell Name (Int: %s, Ext: %s (%d,%d))
		%08X		| Cell FormID
		%.0f		| Form Pos X
		%.0f		| Form Pos Y
		%.0f		| Form Pos Z
		%.0f		| Form Ang X
		%.0f		| Form Ang Y
		%.0f		| Form Ang Z
		%.0f		| Camera Pos X
		%.0f		| Camera Pos Y
		%.0f		| Camera Pos Z
		%.0f		| Camera Ang X
		%.0f		| Camera Ang Y
		%.0f		| Camera Ang Z
		"%s"		| Comment
	*/

	namespace detail
	{
		std::string GetComputerName()
		{
			char          buffer[16]{ "\0" };
			std::uint32_t bufferSize = sizeof(buffer) / sizeof(char);
			if (REX::W32::GetComputerNameA(buffer, &bufferSize))
			{
				return buffer;
			}
			return "UNKNOWN"s;
		}

		auto GetFileTime(std::uint64_t a_time)
		{
			auto time = static_cast<std::time_t>(a_time / 10000000ULL - 11644473600ULL);
			return std::chrono::system_clock::from_time_t(time);
		}

		void PrintPositionInfo(std::stringstream& a_out, RE::NiPoint3 a_pos, RE::NiPoint3 a_ang)
		{
			a_out << std::format("{:.0f}\t{:.0f}\t{:.0f}\t{:.0f}\t{:.0f}\t{:.0f}\t", a_pos.x, a_pos.y, a_pos.z, a_ang.x, a_ang.y, a_ang.z);
		}
	}

	static bool Execute(
		[[maybe_unused]] const RE::SCRIPT_PARAMETER* a_parameters,
		[[maybe_unused]] const char*                 a_compiledParams,
		[[maybe_unused]] RE::TESObjectREFR*          a_refObject,
		[[maybe_unused]] RE::TESObjectREFR*          a_container,
		[[maybe_unused]] RE::Script*                 a_script,
		[[maybe_unused]] RE::ScriptLocals*           a_scriptLocals,
		[[maybe_unused]] float&                      a_result,
		[[maybe_unused]] std::uint32_t&              a_offset)
	{
		char a_comment[512]{ '\0' };
		RE::Script::ParseParameters(
			a_parameters,
			a_compiledParams,
			a_offset,
			a_refObject,
			a_container,
			a_script,
			a_scriptLocals,
			a_comment);

		std::string_view comment = a_comment;
		if (comment.empty())
		{
			return true;
		}

		RE::TESObjectREFR* refr{ a_refObject };
		if (!refr)
		{
			refr = RE::PlayerCharacter::GetSingleton();
			if (!refr)
			{
				return true;
			}

			RE::ConsoleLog::GetSingleton()->Log("No Ref Selected, Using Player for BetaComment"sv);
		}

		std::stringstream out;

		// Current Time
		auto time = std::chrono::system_clock::now();
		out << std::format("{:%m/%d/%y (%H:%M)}\t"sv, time);

		// File Modify Time
		if (auto file = refr->GetFile())
		{
			auto mod = detail::GetFileTime(file->fileInfo.modifyTime);
			out << std::format("{}\t{:%m/%d/%y (%H:%M)}\t"sv, file->GetFilename(), mod);
		}
		else
		{
			out << "\t\t"sv;
		}

		// Computer Name
		out << std::format("{}\t", detail::GetComputerName());

		// Form Info
		out << std::format("{:08X}\t{}\t", refr->GetFormID(), refr->GetFormEditorID());

		// Cell Info
		if (auto cell = refr->GetParentCell())
		{
			if (cell->IsInterior())
			{
				out << std::format("{}\t{:08X}\t", cell->GetFormEditorID(), cell->GetFormID());
			}
			else
			{
				out << std::format("{} ({:d},{:d})\t{:08X}\t",
					cell->worldSpace->GetFormEditorID(), cell->GetDataX(), cell->GetDataY(), cell->GetFormID());
			}
		}
		else
		{
			out << "\t\t"sv;
		}

		// Form Position Info
		detail::PrintPositionInfo(out, refr->data.location, refr->data.angle);

		// Camera Position Info
		if (auto root = RE::Main::WorldRootCamera())
		{
			RE::NiPoint3 angle;
			root->parent->world.rotate.ToEulerAnglesXYZ(angle);
			detail::PrintPositionInfo(out, root->parent->world.translate, angle);
		}
		else
		{
			out << "\t\t\t\t\t\t";
		}

		// Comment
		out << std::format("\"{}\"\r\n", comment);

		// Output to File
		std::ofstream output;
		output.open(
			Config::Patches::sBetaCommentFileName,
			std::ofstream::out | std::ofstream::app);
		output << out.str();
		output.close();

		return true;
	}

	inline void Install()
	{
		if (auto func = RE::SCRIPT_FUNCTION::LocateConsoleCommand("BetaComment"sv))
		{
			func->executeFunction = &Execute;
		}
	}
}
