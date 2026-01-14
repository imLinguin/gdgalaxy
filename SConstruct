#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.

addon_path = "addons/gdgalaxy"
galaxy_lib_path = "sdk/Libraries"
arch_path = "osx"
libs = ["Galaxy64" if env["arch"] == "x86_64" else "Galaxy"]
if env["platform"] == "macos":
    arch_path = "osx"
    libs = ["Galaxy"]
elif env["platform"] == "windows":
    arch_path = "win64" if env["arch"] == "x86_64" else "win32"
elif env["platform"] == "linux":
    arch_path = "/linux64" if env["arch"] == "x86_64" else "/linux32"

if env["CC"] == "cl":
    env.Append(LINKFLAGS=["Galaxy64.lib" if env["arch"] == "x86_64" else "Galaxy.lib"])
else:
    env.Append(LIBS=libs)
    
env.Append(LIBPATH=[galaxy_lib_path + "/" + arch_path])
env.Append(CPPPATH=["src/", "sdk/Include"])

sources = Glob("src/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "{}/libgdgalaxy.{}.{}.framework/libgdgalaxy.{}.{}".format(
            addon_path + "/" + arch_path, env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "{}/libgdgalaxy{}{}".format(addon_path + "/" + arch_path, env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
