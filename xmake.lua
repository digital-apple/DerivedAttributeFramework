set_xmakever("2.8.2")

includes("libraries/commonlibsse-ng")

set_project("DerivedAttributeFramework")
set_version("1.0.3")
set_license("GPL-3.0")

set_languages("c++23")
set_warnings("allextra")

set_policy("package.requires_lock", true)

set_config("skyrim_vr", false)

add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

add_requires("toml11")

target("DerivedAttributeFramework")
    add_deps("commonlibsse-ng")

    add_rules("commonlibsse-ng.plugin", {
        name = "DerivedAttributeFramework",
        author = "digital-apple",
        description = ""
    })

    add_files("source/**.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include", { public = true })
    set_pcxxheader("include/PCH.h")

    add_packages("toml11")

    add_extrafiles("release/**.toml")
	
    after_build(function(target)
        local copy = function(env, ext)
            for _, env in pairs(env:split(";")) do
                if os.exists(env) then
                    local plugins = path.join(env, ext, "SKSE/Plugins")
                    os.mkdir(plugins)
                    os.trycp(target:targetfile(), plugins)
                    os.trycp(target:symbolfile(), plugins)
                    os.trycp("$(projectdir)/release/*.toml", plugins)
                end
            end
        end
        if os.getenv("XSE_TES5_MODS_PATH") then
            copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
        elseif os.getenv("XSE_TES5_GAME_PATH") then
            copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
        end
    end)