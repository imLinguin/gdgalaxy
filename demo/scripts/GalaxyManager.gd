extends Node

func on_auth_success():
	print("Success")
	GDGalaxy.RequestUserStatsAndAchievements()
	
	
func on_auth_failure(reason: GDGalaxy.AuthFailureReason):
	print("Failed ", reason)
	
func on_auth_lost():
	print("Auth lost")
	
func on_stats(userId: int):
	print("Stats received ", userId)

func _init():
	GDGalaxy.Init()
	
	GDGalaxy.on_auth_success.connect(on_auth_success)
	GDGalaxy.on_auth_failure.connect(on_auth_failure)
	GDGalaxy.on_auth_lost.connect(on_auth_lost)
	GDGalaxy.on_user_stats_and_achievements_retrieve_success.connect(on_stats)
	
	GDGalaxy.SignInGalaxy()

func _process(_delta: float):
	GDGalaxy.ProcessData()

func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		GDGalaxy.Shutdown()
		get_tree().quit()
