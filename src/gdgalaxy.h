#pragma once

#include <galaxy/GalaxyApi.h>
#include <godot_cpp/classes/object.hpp>

#include "register_settings.h"

namespace godot {

class GDGalaxy : public Object,
                 galaxy::api::IAuthListener,
                 galaxy::api::IIsDlcOwnedListener,
                 galaxy::api::IUserStatsAndAchievementsRetrieveListener,
                 galaxy::api::IStatsAndAchievementsStoreListener,
                 galaxy::api::ILeaderboardRetrieveListener,
                 galaxy::api::ILeaderboardsRetrieveListener,
                 galaxy::api::ILeaderboardEntriesRetrieveListener,
                 galaxy::api::IUserTimePlayedRetrieveListener,
                 galaxy::api::ILeaderboardScoreUpdateListener {
  GDCLASS(GDGalaxy, Object)

private:
  bool is_init_called;
  bool is_init_success;

protected:
  static void _bind_methods();
  static GDGalaxy *singleton;

public:
  GDGalaxy();
  static GDGalaxy *get_singleton();
  ~GDGalaxy();
  GDGalaxy(const GDGalaxy &) = delete;

  // galaxy::api
  void Init();
  void ProcessData();
  void Shutdown();

  // galaxy::api::User
  void SignInGalaxy();

  // galaxy::api::Apps
  String GetCurrentGameLanguage(galaxy::api::ProductID product = 0);
  String GetCurrentGameLanguageCode(galaxy::api::ProductID product = 0);
  bool IsDlcInstalled(galaxy::api::ProductID product);
  void IsDlcOwned(galaxy::api::ProductID product);

  // galaxy::api::Stats
  void RequestUserStatsAndAchievements();
  int32_t GetStatInt(String name);
  float GetStatFloat(String name);
  void SetStatInt(String name, int32_t value);
  void SetStatFloat(String name, float value);
  void UpdateAvgRateStat(String name, float countThisSession, double sessionLength);
  
  Dictionary GetAchievement(String name);
  void SetAchievement(String name);
  void ClearAchievement(String name);
  void StoreStatsAndAchievements();
  void ResetStatsAndAchievements();
  String GetAchievementDisplayName(String name);
  String GetAchievementDescription(String name);
  bool IsAchievementVisible(String name);
  bool IsAchievementVisibleWhileLocked(String name);
  
  void RequestLeaderboards();
  String GetLeaderboardDisplayName(String name);
  uint8_t GetLeaderboardSortMethod(String name);
  uint8_t GetLeaderboardDisplayType(String name);
  void RequestLeaderboardEntriesGlobal(String name, uint32_t rangeStart, uint32_t rangeEnd);
  void RequestLeaderboardEntriesArroundUser(String name, uint32_t countBefore, uint32_t countAfter, int64_t userId = 0);
  void RequestLeaderboardEntriesForUsers(String name, const PackedInt64Array userIds);
  Dictionary GetRequestedLeaderboardEntry(uint32_t index);
  Dictionary GetRequestedLeaderboardEntryWithDetails(uint32_t index);
  void SetLeaderboardScore(String name, int32_t score, bool forceUpdate=false);
  void SetLeaderboardScoreWithDetails(String name, int32_t score, PackedByteArray details, bool forceUpdate = false);
  uint32_t GetLeaderboardEntryCount(String name);
  void FindLeaderboard(String name);
  void FindOrCreateLeaderboard(String name, String displayName, uint8_t sortMethod, uint8_t displayType);
  void RequestUserTimePlayed(int64_t userID = 0);
  uint32_t GetUserTimePlayed(int64_t userID = 0);

  // galaxy::api::IAuthListener
  void OnAuthSuccess() override;
  void OnAuthFailure(galaxy::api::IAuthListener::FailureReason reason) override;
  void OnAuthLost() override;
  // galaxy::api::IIsDlcOwnedListener
  void OnDlcCheckSuccess(galaxy::api::ProductID productId,
                         bool isOwned) override;
  void OnDlcCheckFailure(
      galaxy::api::ProductID productId,
      galaxy::api::IIsDlcOwnedListener::FailureReason reason) override;
  // galaxy::api::IUserStatsAndAchievementsRetrieveListener
  void OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) override;
  void OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason reason) override;
  // galaxy::api::IStatsAndAchievementsStoreListener
  void OnUserStatsAndAchievementsStoreSuccess() override;
  void OnUserStatsAndAchievementsStoreFailure(galaxy::api::IStatsAndAchievementsStoreListener::FailureReason reason) override;
  // galaxy::api::ILeaderboardRetrieveListener
  void OnLeaderboardRetrieveSuccess(const char* name) override;
  void OnLeaderboardRetrieveFailure(const char* name, galaxy::api::ILeaderboardRetrieveListener::FailureReason reason) override;
  // galaxy::api::IUserTimePlayedRetrieveListener
  void OnUserTimePlayedRetrieveSuccess(galaxy::api::GalaxyID userID) override;
  void OnUserTimePlayedRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserTimePlayedRetrieveListener::FailureReason reason) override;
  // galaxy::api::ILeaderboardsRetrieveListener
  void OnLeaderboardsRetrieveSuccess() override;
  void OnLeaderboardsRetrieveFailure(galaxy::api::ILeaderboardsRetrieveListener::FailureReason reason) override;
  // galaxy::api::ILeaderboardEntriesRetrieveListener
  void OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t count) override;
  void OnLeaderboardEntriesRetrieveFailure(const char* name, galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason reason) override;
  // galaxy::api::ILeaderboardScoreUpdateListener
  void OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank) override;
  void OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, galaxy::api::ILeaderboardScoreUpdateListener::FailureReason reason) override;
  
};

} // namespace godot