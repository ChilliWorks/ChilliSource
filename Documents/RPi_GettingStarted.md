# ChilliSource: Getting Started on Raspberry Pi

Exciting news - you can now use ChilliSource to create both 2D and 3D games for the Raspberry Pi! ChilliSource brings the features of a professional game engine to a $30 computer the size of a credit card.

This tutorial will walk you through how to build a ChilliSource game for Raspberry Pi and covers the caveats of Pi development. Please let us know if you encounter any issues or if there are any new features you would like to see.

We have tested apps running on a Raspberry Pi 3 model B with Raspbian "Jessie" (we also have used libraries from "Jessie"). Apps may work on other Raspberry Pi models and OS versions but we have yet to test.

## Requirements

- Raspberry Pi 3, Model B
- [Raspbian "Jessie"](https://www.raspberrypi.org/downloads/raspbian/)
- [Ninja build](https://ninja-build.org/)
- Python 2.7+
- GNU g++

## Missing Features

This is an experimental platform and due to this, and the Pi hardware/software, there are some features of ChilliSource that cannot be used on the Pi. Some of the missing features will be added as part of the continued development of ChilliSource Pi, others will only be added if and when they are supported by the 3rd party libraries we use or by the Pi hardware itself.

- Framebuffer MSAA: In development.

- Shadows: *The Pi GPU does not support the GL depth texture extension that we require for shadows. So as with some Android devices shadows are disabled*

- Cricket Audio: *Cricket currently have no library for Raspberry Pi (although they do have a Linux library). Feel free to make requests to them for RPi support. In the meantime you can always plug in a different audio library or use OpenAL*

- System Dialogue Boxes: *Unlike on Windows the Pi display does not work in conjunction with the window manager and therefore always renders on top of the OS. Any displayed dialogue boxes would be obscured. We are waiting on better integration between the VideoCore window and X11.*

- Video rendering: *This has not been looked into yet but will probably encounter the same issues as with dialogue boxes.*

- Web View: *This has not been looked into yet but will probably encounter the same issues as with dialogue boxes.*

## Pi Setup

### GPU Memory

By default the Pi allocates the GPU 64 MB of memory. This is enough for games with few textures but anything more advances will require additional resources. We recommend allocating at least 256 MB.

The allocated memory can be changed in *Preferences > Raspberry Pi Configuration > Performance > GPU Memory*.

### Ninja

Ninja is a cross platform build system that we use to create ChilliSource applications. You can download binaries [here](https://ninja-build.org/) or alternatively install it through a package manager. On the Raspberry Pi Ninja can be installed by opening a terminal and typing: "sudo apt-get install ninja-build".

NOTE: For the build script to work, Ninja must be added to the path so that typing "ninja" into a terminal executes Ninja.

### Compiler and Linker

By default we use g++ to compile and link. This is usually preinstalled on your Raspberry Pi (as is Python which is required by the build script).

## Compiling

The build process uses the built in g++ compiler and linker. If you are creating a game from scratch using the project generator then you will already have the build scripts required for Raspberry Pi development. If you have an existing game that you want to build for the Pi then you can use the sample build script (*build.py*) from [CSTest](https://github.com/ChilliWorks/CSTest) or [CSPong](https://github.com/ChilliWorks/CSSamples).

By default the build script is setup to build on the Raspberry Pi itself so make sure your project is on the Pi. Navigate to */Projects/RPi/* in terminal and type "python build.py debug" to start building (you can replace "debug" with "release" to build in release mode). The script will generate an exe and the required assets in an Output folder in the same directory.

If you want to clean the build type "python build.py debug|release clean".

NOTE: Make sure you have installed Ninja prior to running the build script.

### Cross Compiling

Compiling on the Pi can be tedious so it is probably worth setting up a cross compile toolchain that allows you to compile the exe on a different machine and simply copy the *Output* folder to the Pi.

We recommend downloading one of the following prebuilt toolchains depending on your development platform:

- [Windows](http://sysprogs.com/files/gnutoolchains/raspberry/raspberry-gcc4.9.2-r4.exe)
- [Mac OS](https://www.jaredwolff.com/toolchains/rpi-xtools-201402102110.dmg.zip)
- [Linux](https://github.com/raspberrypi/tools)

Once you have downloaded the toolchains simply change the variables at the top of *build.py* to point to the cross compiler, archiver and linker instead.

NOTES:
- If you are compiling on Windows using the downloaded Ninja binary you will need to add Ninja to the PATH environment variable.
- The default build script limits the number of jobs that Ninja will do in parallel to two (if you don't restrict Ninja it will crash the Pi). If you are cross compiling then feel free to remove this restriction (by setting NUM_JOBS=None in *build.py*) to speed up build times.

### Libraries

At the moment all the libraries required for building to the Pi have been harvested from Raspbian "Jessie" itself. In the future these will be compiled from source as part of the toolchain allowing us to better support other Pi models.

## App Configuration

You will need to edit your App.config JSON file to add the following to the main root:

```
"RPi": {
  "CursorType": "NonSystem",
  "WindowDisplayMode": "Windowed"
}
```

The display mode tells ChilliSource whether to start your game "Windowed" or whether to start it "Fullscreen".

NOTE: If you start the game fullscreen make sure you have some way of calling CS::Application::Get()->Quit() as the window will have no close button.

The cursor type is required if you want to see a mouse cursor. The OpenGL ES display window always renders on top of the OS and therefore obscures the system mouse cursor. Setting the cursor type to "NonSystem" will render a software cursor (NOTE: The cursor is just a UI widget and can be configured to suit your game). If you want to hide the cursor completely just set the cursor type to "None".

## Running your Game

If you want to see debug output from your application you should run it from terminal by navigating to the location of your app in terminal and then typing "./AppName" to execute. If you wish to run your app without debug output simply double-click the icon.

NOTE: If you cannot run the app due to "Permission denied" you may need to make it executable by typing "chmod +x AppName" into a terminal.

## Debugging

You can debug "debug" builds on the Pi using gdb (provided you are using the g++/gcc compiler). You will probably want to read up on how to use gdb from the terminal; for example if you want to grab the stack trace of a crash run the app in the following way:

```
gdb ./AppName
catch throw
run
```

libCrypto will throw an exception (which is expected apparently) that will stop the debugger so you need to skip that:

```
signal SIGILL
```

Once the crash is triggered type:

```
bt
```

This will display the call stack.

## Gamepad/Controllers

As part of the new Raspberry Pi backend we've also added support for gamepads/controllers to both Raspberry Pi and Windows. Checkout the CSRunner sample project for a more in-depth example but here's the main code required:

```
auto gamepadSystem = CS::Application::Get()->GetSystem<CS::GamepadSystem>();
if(gamepadSystem != nullptr)
{
    gamepadSystem->SetDefaultActionMapping(k_moveGamepadAction, CS::GamepadAxis::k_y);
    gamepadSystem->SetActionMapping(k_moveGamepadAction, CS::GamepadMappings::PS4::k_name, CS::GamepadMappings::PS4::k_dpadY, true);
    gamepadSystem->SetActionMapping(k_moveGamepadAction, CS::GamepadMappings::PS4::k_name, CS::GamepadMappings::PS4::k_lStickY);

    m_gamepadAxisEventConnection = gamepadSystem->GetMappedAxisPositionChangedEvent().OpenConnection([=](const CS::Gamepad& gamepad, f64 timestamp, u32 actionId, f32 position) { //AXIS MOVED });
    m_gamepadButtonEventConnection = gamepadSystem->GetMappedButtonPressureChangedEvent().OpenConnection([=](const CS::Gamepad& gamepad, f64 timestamp, u32 actionId, f32 pressure) { //BUTTON PRESSURE CHANGED });
}
```

## Next Steps

There isn't much more to say other than have fun and let us know how you get on.
