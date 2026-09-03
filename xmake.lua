-- include subprojects
includes("lib/commonlibf4")

-- set project constants
set_project("BakaFramework")
set_version("4.1.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set configs
set_config("commonlib_ini", true)

-- define targets
target("BakaFramework")
    add_deps("BakaFramework.papyrus", { order = true })

    add_rules("commonlibf4.plugin", {
        name = "BakaFramework",
        author = "shad0wshayd3"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- add extra files
    add_extrafiles(".clang-format")

    -- add install files
    add_installfiles("res/(**)")

target("BakaFramework.papyrus")
    add_rules("commonlibf4.papyrus", {
        install = "BakaFramework"
    })

    add_extrafiles("res/Scripts/Source/Baka/(**.psc)")
