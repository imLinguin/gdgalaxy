# GDGalaxy

GDExtension arround Galaxy SDK.  
This enables usage of GOG Galaxy SDK into Godot games without need to recompile it together with the engine.

## Usage

Supported Godot version: 4.1+  
GOG SDK overview: https://docs.gog.com/sdk/  
Detailed SDK documentation: https://docs.gog.com/galaxyapi/  

Currently covered interfaces are:
- IApps
    - Game language
    - DLC discovery
- IStats
    - Achievements
    - Statistics
    - Leaderboards

> [!NOTE]
> Need anything else? Create an issue and let me know.

### Example

In order to connect with Galaxy you should call `Init()` and `SignInGalaxy()`  
note that in case `SignInGalaxy()` fails - `on_auth_failure` signal is emitted, the game should proceed as normal 

```gd
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
```

## Contributing

Building the extension locally requires you to be able to access https://devportal.gog.com/
in order to link the extension against libraries and to provide header files.

Place binaries into appropriate directories in `Libraries` together with `Include` into `sdk` directory

Right now SConstruct is configured to output the extension library directly into the demo Godot project, and doesnt yet copy needed libraries there, as well as the .gdextension manifest.

Build:

```sh
scons platform=<yourplatform>
```

Supported platforms are `linux` `windows` and `macos`