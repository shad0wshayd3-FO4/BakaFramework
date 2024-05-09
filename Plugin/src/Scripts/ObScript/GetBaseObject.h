#pragma once

namespace ObScript
{
    class GetBaseObject
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
                    return _stricmp(a_elem.functionName, "IgnoreRenderPass") == 0;
                });

            if (it != functions.end())
            {
                *it = RE::SCRIPT_FUNCTION{ LONG_NAME.data(), SHORT_NAME.data(), it->output };
                it->helpString = HelpString().data();
                it->referenceFunction = true;
                it->executeFunction = Execute;

                DEBUG("Registered GetBaseObject."sv);
            }
            else
            {
                DEBUG("Failed to register GetBaseObject."sv);
            }
        }

    private:
        static bool Execute(
            const RE::SCRIPT_PARAMETER*,
            const char*,
            RE::TESObjectREFR* a_refObject,
            RE::TESObjectREFR*,
            RE::Script*,
            RE::ScriptLocals*,
            float&,
            std::uint32_t&)
        {
            if (!a_refObject)
            {
                return true;
            }

            auto base = a_refObject->data.objectReference;
            if (!base)
            {
                return true;
            }

            auto result = fmt::format("GetBaseObject >> {:08X}"sv, base->formID);
            RE::ConsoleLog::GetSingleton()->PrintLine(result.data());
            return true;
        }

        [[nodiscard]] static const std::string& HelpString()
        {
            static auto help = []()
            {
                std::string buf;
                buf += "Print the FormID of an ObjectReference's base Form."sv;
                return buf;
            }();
            return help;
        }

        static constexpr auto LONG_NAME = "GetBaseObject"sv;
        static constexpr auto SHORT_NAME = "gbo"sv;
    };
}
