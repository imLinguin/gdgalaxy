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

galaxy_lib_path = "sdk/Libraries"
if env["platform"] == "macos":
    galaxy_lib_path += "/osx"
    libs = ["libGalaxy.dylib", "libGalaxyPeer64.dylib"]
elif env["platform"] == "windows":
    galaxy_lib_path += "/win64" if env["arch"] == "x86_64" else "/win32"
    libs = ["Galaxy64.dll" if env["arch"] == "x86_64" else "Galaxy.dll"]
elif env["platform"] == "linux":
    galaxy_lib_path += "/linux64" if env["arch"] == "x86_64" else "/linux32"
    libs = ["libGalaxy64.so" if env["arch"] == "x86_64" else "libGalaxy.so"]

libs = [lib.replace(".dll", "") for lib in libs]

env.Append(LIBPATH=[galaxy_lib_path])
env.Append(CPPPATH=["src/", "sdk/Include"])
env.Append(LIBS=libs)

sources = Glob("src/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libgdgalaxy.{}.{}.framework/libgdgalaxy.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "demo/bin/libgdgalaxy{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
