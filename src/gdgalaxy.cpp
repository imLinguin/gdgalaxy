#include "gdgalaxy.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GDGalaxy *GDGalaxy::singleton = nullptr;

GDGalaxy::GDGalaxy() {
	is_init_called = false;
	is_init_success = false;
	singleton = this;
}
GDGalaxy::~GDGalaxy() {
	if (is_init_called) Shutdown();
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
	is_init_called = true;
	if (galaxy::api::GetError()) {
		UtilityFunctions::printerr(galaxy::api::GetError()->GetMsg());
	} else {
		is_init_success = true;
	}
}

void GDGalaxy::Shutdown() {
	galaxy::api::Shutdown();
	is_init_called = false;
}

void GDGalaxy::ProcessData() {
	galaxy::api::ProcessData();
}

void GDGalaxy::SignInGalaxy(){
	uint32_t timeout = GalaxyProjectSettings::get_init_timeout();
	galaxy::api::User()->SignInGalaxy(false, timeout, this);
}

String GDGalaxy::GetCurrentGameLanguage(galaxy::api::ProductID product) {
	const char* language = galaxy::api::Apps()->GetCurrentGameLanguage();
	return String(language);
}

String GDGalaxy::GetCurrentGameLanguageCode(galaxy::api::ProductID product) {
	const char* language = galaxy::api::Apps()->GetCurrentGameLanguageCode();
	return String(language);
}

bool GDGalaxy::IsDlcInstalled(galaxy::api::ProductID product) {
	return galaxy::api::Apps()->IsDlcInstalled(product);
}

void GDGalaxy::IsDlcOwned(galaxy::api::ProductID product) {
	galaxy::api::Apps()->IsDlcOwned(product, this);
}

void GDGalaxy::RequestUserStatsAndAchievements() {
	galaxy::api::Stats()->RequestUserStatsAndAchievements(galaxy::api::GalaxyID(), this);
}

int32_t GDGalaxy::GetStatInt(String name) {
	return galaxy::api::Stats()->GetStatInt(name.utf8());
}

float GDGalaxy::GetStatFloat(String name) {
	return galaxy::api::Stats()->GetStatFloat(name.utf8());
}

void GDGalaxy::SetStatInt(String name, int32_t value) {
	galaxy::api::Stats()->SetStatInt(name.utf8(), value);
}

void GDGalaxy::SetStatFloat(String name, float value) {
	galaxy::api::Stats()->SetStatFloat(name.utf8(), value);
}

void GDGalaxy::UpdateAvgRateStat(String name, float countThisSession, double sessionLength) {
	galaxy::api::Stats()->UpdateAvgRateStat(name.utf8(), countThisSession, sessionLength);
}

Dictionary GDGalaxy::GetAchievement(String name) {
	Dictionary achievement;
	bool isUnlocked = false;
	uint32_t unlockTime = 0;

	galaxy::api::Stats()->GetAchievement(name.utf8(), isUnlocked, unlockTime);

	if (galaxy::api::GetError()) {
		UtilityFunctions::printerr(galaxy::api::GetError()->GetMsg());
	}

	achievement["success"] = galaxy::api::GetError() == nullptr;
	achievement["isUnlocked"] = isUnlocked;
	achievement["unlockTime"] = unlockTime;

	return achievement;
}

void GDGalaxy::SetAchievement(String name) {
	galaxy::api::Stats()->SetAchievement(name.utf8());
}

void GDGalaxy::ClearAchievement(String name) {
	galaxy::api::Stats()->ClearAchievement(name.utf8());
}

void GDGalaxy::StoreStatsAndAchievements() {
	galaxy::api::Stats()->StoreStatsAndAchievements(this);
}

void GDGalaxy::ResetStatsAndAchievements(){
	galaxy::api::Stats()->ResetStatsAndAchievements(this);
}

String GDGalaxy::GetAchievementDisplayName(String name){
	const char* displayName = galaxy::api::Stats()-> GetAchievementDisplayName(name.utf8());
	return String(displayName);
}

String GDGalaxy::GetAchievementDescription(String name){
	const char* description = galaxy::api::Stats()-> GetAchievementDescription(name.utf8());
	return String(description);
}

bool GDGalaxy::IsAchievementVisible(String name){
	return galaxy::api::Stats()->IsAchievementVisible(name.utf8());
}

bool GDGalaxy::IsAchievementVisibleWhileLocked(String name){
	return galaxy::api::Stats()->IsAchievementVisibleWhileLocked(name.utf8());
}

void GDGalaxy::RequestLeaderboards(){
	galaxy::api::Stats()->RequestLeaderboards(this);
	if (galaxy::api::GetError()) {
		UtilityFunctions::printerr(galaxy::api::GetError()->GetMsg());
	}
}

String GDGalaxy::GetLeaderboardDisplayName(String name) {
	const char* displayName = galaxy::api::Stats()->GetLeaderboardDisplayName(name.utf8());
	return String(displayName);
}

uint8_t GDGalaxy::GetLeaderboardSortMethod(String name) {
	return galaxy::api::Stats()->GetLeaderboardSortMethod(name.utf8());
}

uint8_t GDGalaxy::GetLeaderboardDisplayType(String name) {
	return galaxy::api::Stats()->GetLeaderboardDisplayType(name.utf8());
}

void GDGalaxy::RequestLeaderboardEntriesGlobal(String name, uint32_t rangeStart, uint32_t rangeEnd) {
	galaxy::api::Stats()->RequestLeaderboardEntriesGlobal(name.utf8(), rangeStart, rangeEnd, this);
}

void GDGalaxy::RequestLeaderboardEntriesArroundUser(String name, uint32_t countBefore, uint32_t countAfter, int64_t userId) {
	galaxy::api::Stats()->RequestLeaderboardEntriesAroundUser(name.utf8(), countBefore, countAfter, galaxy::api::GalaxyID(userId), this);
}

void GDGalaxy::RequestLeaderboardEntriesForUsers(String name, const PackedInt64Array userIds) {
	std::vector<galaxy::api::GalaxyID> ids;
	ids.reserve(userIds.size());
	for (int i = 0; i < userIds.size(); i++) {
		ids.push_back(galaxy::api::GalaxyID(userIds[i]));
	}
	galaxy::api::Stats()->RequestLeaderboardEntriesForUsers(name.utf8(), ids.data(), userIds.size(), this);
}

Dictionary GDGalaxy::GetRequestedLeaderboardEntry(uint32_t index) {
	Dictionary leaderboardEntry;
	uint32_t rank;
	int32_t score;
	galaxy::api::GalaxyID userID;

	galaxy::api::Stats()->GetRequestedLeaderboardEntry(index, rank, score, userID);

	leaderboardEntry["success"] = galaxy::api::GetError() == nullptr;
	leaderboardEntry["rank"] = rank;
	leaderboardEntry["score"] = score;
	leaderboardEntry["userID"] = userID.ToUint64();
	return leaderboardEntry;
}

Dictionary GDGalaxy::GetRequestedLeaderboardEntryWithDetails(uint32_t index) {
	Dictionary leaderboardEntry;
	uint32_t rank = 0;
	int32_t score = 0;
	uint32_t detailsSize = 0;
	galaxy::api::GalaxyID userID;
	PackedByteArray array;

	galaxy::api::Stats()->GetRequestedLeaderboardEntryWithDetails(index, rank, score, nullptr, 0, detailsSize, userID);

	leaderboardEntry["success"] = galaxy::api::GetError() == nullptr;
	leaderboardEntry["rank"] = rank;
	leaderboardEntry["score"] = score;
	leaderboardEntry["userID"] = userID.ToUint64();
	if (galaxy::api::GetError()) {
		leaderboardEntry["details"] = array;
		return leaderboardEntry;
	}
	array.resize(detailsSize);
	galaxy::api::Stats()->GetRequestedLeaderboardEntryWithDetails(index, rank, score, (void*)array.ptr(), detailsSize, detailsSize, userID);
	leaderboardEntry["success"] = galaxy::api::GetError() == nullptr;
	leaderboardEntry["details"] = array;
	return leaderboardEntry;
}

void GDGalaxy::SetLeaderboardScore(String name, int32_t score, bool forceUpdate) {
	galaxy::api::Stats()->SetLeaderboardScore(name.utf8(), score, forceUpdate, this);
}

void GDGalaxy::SetLeaderboardScoreWithDetails(String name, int32_t score, PackedByteArray details, bool forceUpdate){
	galaxy::api::Stats()->SetLeaderboardScoreWithDetails(name.utf8(), score, (void*)details.ptr(), details.size(), forceUpdate, this);
}

uint32_t GDGalaxy::GetLeaderboardEntryCount(String name){
	return galaxy::api::Stats()->GetLeaderboardEntryCount(name.utf8());
}

void GDGalaxy::FindLeaderboard(String name) {
	galaxy::api::Stats()->FindLeaderboard(name.utf8(), this);
}

void GDGalaxy::FindOrCreateLeaderboard(String name, String displayName, uint8_t sortMethod, uint8_t displayType) {
	galaxy::api::LeaderboardSortMethod leaderboardSortMethod = static_cast<galaxy::api::LeaderboardSortMethod>(sortMethod);
	galaxy::api::LeaderboardDisplayType leaderboardDisplayType = static_cast<galaxy::api::LeaderboardDisplayType>(sortMethod);
	galaxy::api::Stats()->FindOrCreateLeaderboard(name.utf8(), displayName.utf8(), leaderboardSortMethod, leaderboardDisplayType, this);
}

void GDGalaxy::RequestUserTimePlayed(int64_t userID) {
	galaxy::api::Stats()->RequestUserTimePlayed(galaxy::api::GalaxyID(userID), this);
}

uint32_t GDGalaxy::GetUserTimePlayed(int64_t userID) {
	return galaxy::api::Stats()->GetUserTimePlayed(galaxy::api::GalaxyID(userID));
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

void GDGalaxy::OnDlcCheckSuccess(galaxy::api::ProductID productId, bool isOwned) {
	emit_signal("on_dlc_check_success", productId, isOwned);
}

void GDGalaxy::OnDlcCheckFailure(galaxy::api::ProductID productId, galaxy::api::IIsDlcOwnedListener::FailureReason reason) {
	emit_signal("on_dlc_check_failure", productId, reason);
}

void GDGalaxy::OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) {
	emit_signal("on_user_stats_and_achievements_retrieve_success", userID.ToUint64());	
}

void GDGalaxy::OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason reason) {
	emit_signal("on_user_stats_and_achievements_retrieve_failure", userID.ToUint64(), reason);	
}

void GDGalaxy::OnUserStatsAndAchievementsStoreSuccess() {
	emit_signal("on_stats_and_achievements_store_success");
}

void GDGalaxy::OnUserStatsAndAchievementsStoreFailure(galaxy::api::IStatsAndAchievementsStoreListener::FailureReason reason) {
	emit_signal("on_stats_and_achievements_store_failure", reason);
}

void GDGalaxy::OnLeaderboardsRetrieveSuccess() {
	emit_signal("on_leaderboards_retrieve_success");
}

void GDGalaxy::OnLeaderboardsRetrieveFailure(galaxy::api::ILeaderboardsRetrieveListener::FailureReason reason) {
	emit_signal("on_leaderboards_retrieve_failure", reason);
}

void GDGalaxy::OnLeaderboardRetrieveSuccess(const char* name) {
	emit_signal("on_leaderboard_retrieve_success", String(name));
}

void GDGalaxy::OnLeaderboardRetrieveFailure(const char* name, galaxy::api::ILeaderboardRetrieveListener::FailureReason reason) {
	emit_signal("on_leaderboard_retrieve_failure", String(name), reason);
}

void GDGalaxy::OnUserTimePlayedRetrieveSuccess(galaxy::api::GalaxyID userID) {
	emit_signal("on_user_time_played_retrieve_success", userID.ToUint64());
}

void GDGalaxy::OnUserTimePlayedRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserTimePlayedRetrieveListener::FailureReason reason) {
	emit_signal("on_user_time_played_retrieve_failure", userID.ToUint64(), reason);
}

void GDGalaxy::OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t count) {
	emit_signal("on_leaderboard_entries_retrieve_success", String(name));
}

void GDGalaxy::OnLeaderboardEntriesRetrieveFailure(const char* name, galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason reason) {
	emit_signal("on_leaderboard_entries_retrieve_failure", String(name), reason);
}

void GDGalaxy::OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank) {
	emit_signal("on_leaderboard_score_update_success", String(name), score, oldRank, newRank);
}

void GDGalaxy::OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, galaxy::api::ILeaderboardScoreUpdateListener::FailureReason reason) {
	emit_signal("on_leaderboard_score_update_failure", String(name), score, reason);
}

void GDGalaxy::_bind_methods() {
    // galaxy::api
	ClassDB::bind_method(D_METHOD("Init"), &GDGalaxy::Init);
	ClassDB::bind_method(D_METHOD("ProcessData"), &GDGalaxy::ProcessData);
	ClassDB::bind_method(D_METHOD("Shutdown"), &GDGalaxy::Shutdown);

    // galaxy::api::User
	ClassDB::bind_method(D_METHOD("SignInGalaxy"), &GDGalaxy::SignInGalaxy);

    // galaxy::api::Apps
	ClassDB::bind_method(D_METHOD("GetCurrentGameLanguage", "product_id"), &GDGalaxy::GetCurrentGameLanguage, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("GetCurrentGameLanguageCode", "product_id"), &GDGalaxy::GetCurrentGameLanguageCode, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("IsDlcInstalled", "product_id"), &GDGalaxy::IsDlcInstalled);

	// galaxy::api::Stats
	ClassDB::bind_method(D_METHOD("RequestUserStatsAndAchievements"), &GDGalaxy::RequestUserStatsAndAchievements);
	ClassDB::bind_method(D_METHOD("GetStatInt", "name"), &GDGalaxy::GetStatInt);
	ClassDB::bind_method(D_METHOD("GetStatFloat", "name"), &GDGalaxy::GetStatFloat);
	ClassDB::bind_method(D_METHOD("SetStatInt", "name", "value"), &GDGalaxy::SetStatInt);
	ClassDB::bind_method(D_METHOD("SetStatFloat", "name", "value"), &GDGalaxy::SetStatFloat);
	ClassDB::bind_method(D_METHOD("UpdateAvgRateStat", "name", "countThisSession", "sessionLength"), &GDGalaxy::UpdateAvgRateStat);
	ClassDB::bind_method(D_METHOD("GetAchievement", "name"), &GDGalaxy::GetAchievement);
	ClassDB::bind_method(D_METHOD("ClearAchievement", "name"), &GDGalaxy::ClearAchievement);
	ClassDB::bind_method(D_METHOD("StoreStatsAndAchievements"), &GDGalaxy::StoreStatsAndAchievements);
	ClassDB::bind_method(D_METHOD("ResetStatsAndAchievements"), &GDGalaxy::ResetStatsAndAchievements);
	ClassDB::bind_method(D_METHOD("GetAchievementDisplayName", "name"), &GDGalaxy::GetAchievementDisplayName);
	ClassDB::bind_method(D_METHOD("GetAchievementDescription", "name"), &GDGalaxy::GetAchievementDescription);
	ClassDB::bind_method(D_METHOD("IsAchievementVisible", "name"), &GDGalaxy::IsAchievementVisible);
	ClassDB::bind_method(D_METHOD("IsAchievementVisibleWhileLocked", "name"), &GDGalaxy::IsAchievementVisibleWhileLocked);

	ClassDB::bind_method(D_METHOD("RequestLeaderboards"), &GDGalaxy::RequestLeaderboards);
	ClassDB::bind_method(D_METHOD("GetLeaderboardDisplayName", "name"), &GDGalaxy::GetLeaderboardDisplayName);
	ClassDB::bind_method(D_METHOD("GetLeaderboardSortMethod", "name"), &GDGalaxy::GetLeaderboardSortMethod);
	ClassDB::bind_method(D_METHOD("GetLeaderboardDisplayType", "name"), &GDGalaxy::GetLeaderboardDisplayType);
	ClassDB::bind_method(D_METHOD("RequestLeaderboardEntriesGlobal", "name", "rangeStart", "rangeEnd"), &GDGalaxy::RequestLeaderboardEntriesGlobal);
	ClassDB::bind_method(D_METHOD("RequestLeaderboardEntriesArroundUser", "name", "countBefore", "countAfter", "userId"), &GDGalaxy::RequestLeaderboardEntriesArroundUser, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("RequestLeaderboardEntriesForUsers", "name", "userIds"), &GDGalaxy::RequestLeaderboardEntriesForUsers);
	ClassDB::bind_method(D_METHOD("GetRequestedLeaderboardEntry", "index"), &GDGalaxy::GetRequestedLeaderboardEntry);
	ClassDB::bind_method(D_METHOD("GetRequestedLeaderboardEntryWithDetails", "index"), &GDGalaxy::GetRequestedLeaderboardEntryWithDetails);
	ClassDB::bind_method(D_METHOD("SetLeaderboardScore", "name", "score", "forceUpdate"), &GDGalaxy::SetLeaderboardScore, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("SetLeaderboardScoreWithDetails", "name", "score", "details", "forceUpdate"), &GDGalaxy::SetLeaderboardScoreWithDetails, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("GetLeaderboardEntryCount", "name"), &GDGalaxy::GetLeaderboardEntryCount);
	ClassDB::bind_method(D_METHOD("FindLeaderboard", "name"), &GDGalaxy::FindLeaderboard);
	ClassDB::bind_method(D_METHOD("FindOrCreateLeaderboard", "name", "displayName", "sortMethod", "displayType"), &GDGalaxy::FindOrCreateLeaderboard);
	ClassDB::bind_method(D_METHOD("RequestUserTimePlayed", "userID"), &GDGalaxy::RequestUserTimePlayed);
	ClassDB::bind_method(D_METHOD("GetUserTimePlayed", "userID"), &GDGalaxy::GetUserTimePlayed);


 	// galaxy::api::IAuthListener
	ADD_SIGNAL(MethodInfo("on_auth_success"));
	ADD_SIGNAL(MethodInfo("on_auth_failure", PropertyInfo(Variant::INT, "reason")));
	ADD_SIGNAL(MethodInfo("on_auth_lost"));
  	
	// galaxy::api::IIsDlcOwnedListener
	ADD_SIGNAL(MethodInfo("on_dlc_check_success", PropertyInfo(Variant::INT, "productId"), PropertyInfo(Variant::BOOL, "isOwned")));
	ADD_SIGNAL(MethodInfo("on_dlc_check_failure", PropertyInfo(Variant::INT, "productId"), PropertyInfo(Variant::INT, "reason")));
  	// galaxy::api::IUserStatsAndAchievementsRetrieveListener
  	ADD_SIGNAL(MethodInfo("on_user_stats_and_achievements_retrieve_success", PropertyInfo(Variant::INT, "userID")));
  	ADD_SIGNAL(MethodInfo("on_user_stats_and_achievements_retrieve_failure", PropertyInfo(Variant::INT, "userID"), PropertyInfo(Variant::INT, "reason")));
	// galaxy::api::IStatsAndAchievementsStoreListener
  	ADD_SIGNAL(MethodInfo("on_stats_and_achievements_store_success"));
  	ADD_SIGNAL(MethodInfo("on_stats_and_achievements_store_failure", PropertyInfo(Variant::INT, "reason")));
    // galaxy::api::ILeaderboardsRetrieveListener
    ADD_SIGNAL(MethodInfo("on_leaderboards_retrieve_success"));
  	ADD_SIGNAL(MethodInfo("on_leaderboards_retrieve_failure", PropertyInfo(Variant::INT, "reason")));
	// galaxy::api::ILeaderboardRetrieveListener
	ADD_SIGNAL(MethodInfo("on_leaderboard_retrieve_success", PropertyInfo(Variant::STRING, "name")));
	ADD_SIGNAL(MethodInfo("on_leaderboard_retrieve_failure", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "reason")));
 	// galaxy::api::IUserTimePlayedRetrieveListener
	ADD_SIGNAL(MethodInfo("on_user_time_played_retrieve_success", PropertyInfo(Variant::INT, "userID")));
	ADD_SIGNAL(MethodInfo("on_user_time_played_retrieve_failure", PropertyInfo(Variant::INT, "userID"), PropertyInfo(Variant::INT, "reason")));
	// galaxy::api::ILeaderboardEntriesRetrieveListener
	ADD_SIGNAL(MethodInfo("on_leaderboard_entries_retrieve_success", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "entriesCount")));
  	ADD_SIGNAL(MethodInfo("on_leaderboard_entries_retrieve_failure", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "reason")));
    // galaxy::api::ILeaderboardScoreUpdateListener
	ADD_SIGNAL(MethodInfo("on_leaderboard_score_update_success", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::INT, "oldRank"), PropertyInfo(Variant::INT, "newRank")));
	ADD_SIGNAL(MethodInfo("on_leaderboard_score_update_failure", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::INT, "reason")));

    // galaxy::api::IAuthListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_UNDEFINED", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_GALAXY_SERVICE_NOT_AVAILABLE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_SERVICE_NOT_AVAILABLE);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_CONNECTION_FAILURE);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_NO_LICENSE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_NO_LICENSE);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_INVALID_CREDENTIALS", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_INVALID_CREDENTIALS);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_GALAXY_NOT_INITIALIZED", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_NOT_INITIALIZED);
	ClassDB::bind_integer_constant("GDGalaxy", "AuthFailureReason", "AUTH_FAILURE_REASON_EXTERNAL_SERVICE_FAILURE", galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_EXTERNAL_SERVICE_FAILURE);

    // galaxy::api::IIsDlcOwnedListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "IsDlcOwnedFailureReason", "IS_DLC_OWNED_FAILURE_REASON_UNDEFINED", galaxy::api::IIsDlcOwnedListener::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "IsDlcOwnedFailureReason", "IS_DLC_OWNED_FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN", galaxy::api::IIsDlcOwnedListener::FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN);
	ClassDB::bind_integer_constant("GDGalaxy", "IsDlcOwnedFailureReason", "IS_DLC_OWNED_FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::IIsDlcOwnedListener::FAILURE_REASON_CONNECTION_FAILURE);
	ClassDB::bind_integer_constant("GDGalaxy", "IsDlcOwnedFailureReason", "IS_DLC_OWNED_FAILURE_REASON_EXTERNAL_SERVICE_FAILURE", galaxy::api::IIsDlcOwnedListener::FAILURE_REASON_EXTERNAL_SERVICE_FAILURE);

  	// galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "UserStatsAndAchievementsRetrieveFailureReason", "ACHIEVEMENTS_RETRIEVE_FAILURE_REASON_UNDEFINED", galaxy::api::IUserStatsAndAchievementsRetrieveListener::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "UserStatsAndAchievementsRetrieveFailureReason", "ACHIEVEMENTS_RETRIEVE_FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::IUserStatsAndAchievementsRetrieveListener::FAILURE_REASON_CONNECTION_FAILURE);
	// galaxy::api::IStatsAndAchievementsStoreListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "StatsAndAchievementsStoreFailureReason", "ACHIEVEMENTS_STORE_FAILURE_REASON_UNDEFINED", galaxy::api::IStatsAndAchievementsStoreListener::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "StatsAndAchievementsStoreFailureReason", "ACHIEVEMENTS_STOREFAILURE_REASON_CONNECTION_FAILURE", galaxy::api::IStatsAndAchievementsStoreListener::FAILURE_REASON_CONNECTION_FAILURE);
	// galaxy::api::ILeaderboardsRetrieveListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardsRetrieveFailureReason", "LEADERBOARDS_RETRIEVE_FAILURE_REASON_UNDEFINED", galaxy::api::ILeaderboardsRetrieveListener::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardsRetrieveFailureReason", "LEADERBOARDS_RETRIEVE_FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::ILeaderboardsRetrieveListener::FAILURE_REASON_CONNECTION_FAILURE);
	// galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardEntriesRetrieveFailureReason", "LEADERBOARD_RETRIEVE_FAILURE_REASON_UNDEFINED", galaxy::api::ILeaderboardEntriesRetrieveListener::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardEntriesRetrieveFailureReason", "LEADERBOARD_RETRIEVE_FAILURE_REASON_NOT_FOUND", galaxy::api::ILeaderboardEntriesRetrieveListener::FAILURE_REASON_NOT_FOUND);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardEntriesRetrieveFailureReason", "LEADERBOARD_RETRIEVE_FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::ILeaderboardEntriesRetrieveListener::FAILURE_REASON_CONNECTION_FAILURE);
    // galaxy::api::ILeaderboardScoreUpdateListener::FailureReason
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardScoreUpdateFailureReason", "LEADERBOARD_SCORE_UPDATE_FAILURE_REASON_UNDEFINED", galaxy::api::ILeaderboardScoreUpdateListener::FAILURE_REASON_UNDEFINED);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardScoreUpdateFailureReason", "LEADERBOARD_SCORE_UPDATE_FAILURE_REASON_NO_IMPROVEMENT", galaxy::api::ILeaderboardScoreUpdateListener::FAILURE_REASON_NO_IMPROVEMENT);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardScoreUpdateFailureReason", "LEADERBOARD_SCORE_UPDATE_FAILURE_REASON_CONNECTION_FAILURE", galaxy::api::ILeaderboardScoreUpdateListener::FAILURE_REASON_CONNECTION_FAILURE);

	// galaxy::api::LeaderboardSortMethod
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardSortMethod", "LEADERBOARD_SORT_METHOD_NONE", galaxy::api::LeaderboardSortMethod::LEADERBOARD_SORT_METHOD_NONE);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardSortMethod", "LEADERBOARD_SORT_METHOD_ASCENDING", galaxy::api::LeaderboardSortMethod::LEADERBOARD_SORT_METHOD_ASCENDING);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardSortMethod", "LEADERBOARD_SORT_METHOD_DESCENDING", galaxy::api::LeaderboardSortMethod::LEADERBOARD_SORT_METHOD_DESCENDING);

	// galaxy::api::LeaderboardDisplayType
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardDisplayType", "LEADERBOARD_DISPLAY_TYPE_NONE", galaxy::api::LeaderboardDisplayType::LEADERBOARD_DISPLAY_TYPE_NONE);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardDisplayType", "LEADERBOARD_DISPLAY_TYPE_NUMBER", galaxy::api::LeaderboardDisplayType::LEADERBOARD_DISPLAY_TYPE_NUMBER);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardDisplayType", "LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS", galaxy::api::LeaderboardDisplayType::LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS);
	ClassDB::bind_integer_constant("GDGalaxy", "LeaderboardDisplayType", "LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS", galaxy::api::LeaderboardDisplayType::LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS);
}
