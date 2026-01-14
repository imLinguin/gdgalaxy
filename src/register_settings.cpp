#include "register_settings.h"

using namespace godot;

void GalaxyProjectSettings::register_settings() {
    ProjectSettings *settings = ProjectSettings::get_singleton();

    if(!settings->has_setting("galaxy/initialization/client_id")) {
        settings->set("galaxy/initialization/client_id", "");
    }
    if(!settings->has_setting("galaxy/initialization/client_secret")) {
        settings->set("galaxy/initialization/client_secret", "");
    }
     if(!settings->has_setting("galaxy/initialization/init_timeout")) {
        settings->set("galaxy/initialization/init_timeout", 5);
    }
}

String GalaxyProjectSettings::get_client_id() {
    Variant clientId = ProjectSettings::get_singleton()->get_setting_with_override("galaxy/initialization/client_id");
    return clientId;
}

String GalaxyProjectSettings::get_client_secret() {
    Variant clientSecret = ProjectSettings::get_singleton()->get_setting_with_override("galaxy/initialization/client_secret");
    return clientSecret;
}

uint32_t GalaxyProjectSettings::get_init_timeout() {
    return ProjectSettings::get_singleton()->get_setting_with_override("galaxy/initialization/init_timeout");
}