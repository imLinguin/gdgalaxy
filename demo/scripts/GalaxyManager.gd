extends Control

@onready var sign_in_status: Label = %"SignIn status"
@onready var init_status: Label = %"Init status"

@onready var init_button: Button = %"Init Button"
@onready var sign_in_galaxy: Button = %SignInGalaxy
@onready var logged_on_status: Label = %"LoggedOn status"

var initialized = false

func _ready():
	init_button.pressed.connect(init_pressed)
	sign_in_galaxy.pressed.connect(sign_in_galaxy_pressed)
	
	GDGalaxy.on_auth_success.connect(on_auth_success)
	GDGalaxy.on_auth_failure.connect(on_auth_failure)
	GDGalaxy.on_auth_lost.connect(on_auth_lost)
	GDGalaxy.on_user_stats_and_achievements_retrieve_success.connect(on_stats)

func _process(_delta: float):
	GDGalaxy.ProcessData()
	
func init_pressed():
	if initialized: return
	var success = GDGalaxy.Init()
	initialized = success;
	init_status.text = "Init: " + yes_or_no(success)
	
func sign_in_galaxy_pressed():
	GDGalaxy.SignInGalaxy()

func on_auth_failure(reason: GDGalaxy.AuthFailureReason):
	var map = ['UNDEFINED',
'GALAXY_SERVICE_NOT_AVAILABLE',
'GALAXY_SERVICE_NOT_SIGNED_IN',
'CONNECTION_FAILURE',
'NO_LICENSE',
'INVALID_CREDENTIALS',
'GALAXY_NOT_INITIALIZED',
'EXTERNAL_SERVICE_FAILURE',
]
	print('Auth failed ', reason)
	sign_in_status.text = 'SignedIn: '  + yes_or_no(GDGalaxy.SignedIn()) + ' ' + map[reason]
	logged_on_status.text = 'IsLoggedOn: ' + yes_or_no(GDGalaxy.IsLoggedOn())

func on_auth_lost():
	print('Auth lost')
	sign_in_status.text = 'SignedIn: ' + yes_or_no(GDGalaxy.SignedIn()) + ' (AuthLost)'
	logged_on_status.text = 'IsLoggedOn: ' + yes_or_no(GDGalaxy.IsLoggedOn()) + ' (AuthLost)'

func on_auth_success():
	print('Successful login')
	sign_in_status.text = 'SignedIn: ' + yes_or_no(GDGalaxy.SignedIn())
	logged_on_status.text = 'IsLoggedOn: ' + yes_or_no(GDGalaxy.IsLoggedOn())
	
func on_stats(userId: int):
	print("Stats received ", userId)
	
func yes_or_no(val):
	return "Yes" if val else "No"
