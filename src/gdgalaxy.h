#pragma once

#include <godot_cpp/classes/object.hpp>
#include <galaxy/GalaxyApi.h>

#include "register_settings.h"

namespace godot {

class GDGalaxy : public Object,
    galaxy::api::GlobalAuthListener {
    GDCLASS(GDGalaxy, Object)

protected:
    static void _bind_methods();
    static GDGalaxy *singleton;
    
public:
    GDGalaxy();
    static GDGalaxy *get_singleton();
    ~GDGalaxy();
    GDGalaxy(const GDGalaxy&) = delete;

    // galaxy::api
    void Init();
    void ProcessData();
    void Shutdown();

    // galaxy::api::User
    void SignInGalaxy();

    // galaxy::api::IAuthListener
    void OnAuthSuccess();
    void OnAuthFailure(galaxy::api::IAuthListener::FailureReason reason);
    void OnAuthLost();
};

}