#include "register_types.h"

#include "gdgalaxy.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static GDGalaxy *Galaxy = nullptr;

void initialize_galaxy_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}

    Galaxy = memnew(GDGalaxy);
    Engine::get_singleton()->register_singleton("Galaxy", GDGalaxy::get_singleton());
	GDREGISTER_CLASS(GDGalaxy);
}

void uninitialize_galaxy_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_CORE) {
		return;
	}
    Engine::get_singleton()->unregister_singleton("Galaxy");
    memdelete(Galaxy);
}

extern "C" {
GDExtensionBool GDE_EXPORT gdgalaxy_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_galaxy_module);
	init_obj.register_terminator(uninitialize_galaxy_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

	return init_obj.init();
}
}