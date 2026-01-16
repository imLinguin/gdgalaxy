<p align="center"><img width="128" src="assets/icon.png" /></p>
<h1 align="center">GDGalaxy</h1>
<p align="center">
GDExtension arround Galaxy SDK.<br/>
This enables usage of GOG Galaxy SDK in Godot games without need to recompile the engine.
</p>

## Usage

Supported Godot version: 4.1+  
GOG SDK overview: https://docs.gog.com/sdk/  
Detailed SDK documentation: https://docs.gog.com/galaxyapi/  

Supported platforms:
- Linux - x86_32, x86_64
- Mac - arm64, x86_64
- Windows - x86_32, x86_64

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

### Initial setup

Make sure to first setup your project with clientId and clientSecret values you get from GOG.

`Project > Project Settings > Galaxy > Initialization`

<img width="1481" height="876" alt="image" src="https://github.com/user-attachments/assets/149066d1-fa8d-4d39-b3c9-96ebf843e12a" />

### Code example

In order to connect with Galaxy you should call `Init()` and `SignInGalaxy()`  
note that in case `SignInGalaxy()` fails - `on_auth_failure` signal is emitted, the game should proceed as normal 
Make sure to call `GDGalaxy.ProcessData()` every frame.

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

Build:

```sh
scons platform=<yourplatform>
```

Supported platforms are `linux` `windows` and `macos`
