#include "gdgalaxy.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GDGalaxy *GDGalaxy::singleton = nullptr;

GDGalaxy::GDGalaxy() {
	singleton = this;
}
GDGalaxy::~GDGalaxy() {
	singleton = nullptr;
}

GDGalaxy *GDGalaxy::get_singleton() {
	return singleton;
}

void GDGalaxy::Init() {
	String clientId = GalaxyProjectSettings::get_client_id();
	String clientSecret = GalaxyProjectSettings::get_client_secret();

	if (clientId.is_empty()) {
        UtilityFunctions::printerr("Galaxy client id is not configured in project settings!\n");
        return;
    }
	if (clientSecret.is_empty()) {
        UtilityFunctions::printerr("Galaxy client secret is not configured in project settings!\n");
        return;
    }

	galaxy::api::Init({clientId.ascii(), clientSecret.ascii()});

	if (galaxy::api::GetError()) {
		UtilityFunctions::printerr(galaxy::api::GetError()->GetMsg());
	}
}

void GDGalaxy::Shutdown() {
	galaxy::api::Shutdown();
}

void GDGalaxy::ProcessData() {
	galaxy::api::ProcessData();
}

void GDGalaxy::SignInGalaxy(){
	uint32_t timeout = GalaxyProjectSettings::get_init_timeout();
	galaxy::api::User()->SignInGalaxy(false, timeout, this);
}

void GDGalaxy::OnAuthSuccess() {
	emit_signal("on_auth_success");
}

void GDGalaxy::OnAuthFailure(galaxy::api::IAuthListener::FailureReason reason) {
	emit_signal("on_auth_failure", reason);
}

void GDGalaxy::OnAuthLost() {
	emit_signal("on_auth_lost");
}

void GDGalaxy::_bind_methods() {
	ClassDB::bind_method(D_METHOD("Init"), &GDGalaxy::Init);
	ClassDB::bind_method(D_METHOD("ProcessData"), &GDGalaxy::ProcessData);
	ClassDB::bind_method(D_METHOD("Shutdown"), &GDGalaxy::Shutdown);
	ClassDB::bind_method(D_METHOD("SignInGalaxy"), &GDGalaxy::SignInGalaxy);

	ADD_SIGNAL(MethodInfo("on_auth_success"));
	ADD_SIGNAL(MethodInfo("on_auth_failure", PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("on_auth_lost"));
	
	//Auth
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_UNDEFINED", (int)galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_GALAXY_SERVICE_NOT_AVAILABLE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_SERVICE_NOT_AVAILABLE);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_CONNECTION_FAILURE);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_NO_LICENSE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_NO_LICENSE);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_INVALID_CREDENTIALS", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_INVALID_CREDENTIALS);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_GALAXY_NOT_INITIALIZED", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_NOT_INITIALIZED);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "FAILURE_REASON_EXTERNAL_SERVICE_FAILURE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_EXTERNAL_SERVICE_FAILURE);
}
