#pragma once

#include <stdint.h>
#include <string>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

    class GalaxyProjectSettings {
    public:
        static void register_settings();
        
        static String get_client_id();
        static String get_client_secret();
        static uint32_t get_init_timeout();
    };
}