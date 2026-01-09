add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})

set_languages("cxx17")

target("OptiChemin")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_rules("plugin.compile_commands.autoupdate")
    set_rundir("$(projectdir)")

    -- CRITICAL: Optimization flags
    -- These MUST be inside the target or global scope, but the 'if' checks the mode.
    if is_mode("release") then
        set_optimize("fastest")
        set_policy("build.optimization.lto", true)
        set_strip("all")

        if is_plat("windows") then
            add_cxflags("/fp:fast", "/arch:AVX2", "/GL") 
        else
            add_cxflags("-march=native", "-ffast-math", "-funroll-loops")
        end
    end
    -- !!! END of the is_mode check !!!


task("benchmark")
    set_menu({
        usage = "xmake benchmark",
        description = "Switch to release mode, compile, and run."
    })
    on_run(function ()
        import("core.project.task")
        
        -- 1. Switch configuration to release
        print(">>> Switching to Release Mode...")
        os.exec("xmake f -m release")
        
        -- 2. Build and Run
        print(">>> Compiling and Running...")
        os.exec("xmake run")
    end)