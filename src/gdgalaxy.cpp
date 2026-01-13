#include "gdgalaxy.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;


GDGalaxy::GDGalaxy() {
	singleton = this;
}
GDGalaxy::~GDGalaxy() {
	singleton = nullptr;
}

GDGalaxy *GDGalaxy::get_singleton() {
	return singleton;
}

void GDGalaxy::_bind_methods() {}
