#pragma once

#include <godot_cpp/classes/object.hpp>

namespace godot {

class GDGalaxy : public Object {
    GDCLASS(GDGalaxy, Object)

private:



protected:
    static void _bind_methods();
    static GDGalaxy *singleton;

public:
    static GDGalaxy *get_singleton();
    GDGalaxy();
    ~GDGalaxy();
    GDGalaxy(const GDGalaxy&) = delete;
};

}