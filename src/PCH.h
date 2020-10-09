#pragma once

#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include <spdlog/sinks/basic_file_sink.h>

#include "AutoTOML.hpp"

#define DLLEXPORT __declspec(dllexport)

namespace logger
{
	using namespace F4SE::log;
}

namespace stl
{
	using F4SE::stl::report_and_fail;
	using F4SE::stl::span;
}

using namespace std::literals;

#include "version.h"

#include "Settings.h"