add_rules("mode.debug", "mode.release")

set_languages("cxx17")

add_requires("raylib")

target("OptiChemin")
  set_kind("binary")
  add_files("src/*.cpp")
  --add_files("src/viz/*.cpp")
  add_includedirs("include")
  add_packages("raylib")
  add_rules("plugin.compile_commands.autoupdate")
  set_rundir("$(projectdir)") -- set the run directory to project root
