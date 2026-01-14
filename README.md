# GDGalaxy

GDExtension arround Galaxy SDK.

## Usage

Godot 4.1+

WIP

## Contributing

Building the extension locally requires you to be able to access https://devportal.gog.com/
in order to link the extension against libraries and to provide header files.

Place both `Include` and binaries into appropriate directories in `Libraries`

Right now SConstruct is configured to output the extension library directly into the demo Godot project, and doesnt yet copy needed libraries there, as well as the .gdextension manifest.

Build:

```sh
scons platform=<yourplatform>
```

Supported platforms are `linux` `windows` and `macos`