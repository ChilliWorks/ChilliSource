# ChilliSource: Getting Started on Raspberry Pi

Exciting news! You can now create games for the Raspberry Pi using ChilliSource. This tutorial will walk you through how to compile and deploy your game and some caveats to note when using ChilliSource for Pi development.

Please let us know of any issues that you come across or any features you would like to see.

## Known Issues

It is worth noting that this is an experimental branch, due to this and the reduced ability of the Pi's GPU, there are some features of ChilliSource that cannot be used on the Pi. Some of the missing features will be added as part of the continued development of the Pi backend, others will only be added if and when they are supported by the 3rd party libraries we use or by the Pi hardware itself.

**Hardware and Version**

We have tested on a Raspberry Pi 3 model B running Raspbian "Jessie". It may work on other Raspberry Pi models and OS versions but we have yet to test.

**In Development**

- HTTP System
- Text Entry System
- Framebuffer MSAA

**Waiting on Support**

- Shadows: *The Pi GPU does not support the GL depth texture extension that we require for shadows.*

- Cricket Audio: *Cricket currently have no library for Raspberry Pi (although they do have a Linux library). Feel free to make requests to them for RPi support. In the meantime you can always plug in a different audio library or use OpenAL*

- System Dialogue Boxes: *Unlike on Windows the Pi display does not work in conjunction with the window manager and therefore always renders on top of the OS. Any displayed dialogue boxes would be obscured. We are waiting on better integration between the VideoCore window and X11.*

- Video rendering: *This has not been looked into yet but the worry is that we will encounter the same issues as with dialogue boxes.*

- Web View: *This has not been looked into yet but the worry is that we will encounter the same issues as with dialogue boxes.*

## Requirements

- Raspberry Pi 3, Model B
- [Raspbian "Jessie"](https://www.raspberrypi.org/downloads/raspbian/)
- [Ninja build](https://ninja-build.org/)

## Pi Setup

### GPU Memory

By default the Pi allocates the GPU 64 MB of memory. This is enough for games with few textures but anything more advances will require additional resources. We recommend allocating at least 256 MB.

The allocated memory can be changed in *Preferences > Raspberry Pi Configuration > Performance > GPU Memory*.

### Ninja

Ninja is a cross platform build system that we use to create ChilliSource applications. You can download binaries [here](https://ninja-build.org/) or alternatively install it through a package manager. On the Raspberry Pi Ninja can be installed by opening a terminal and typing: "sudo apt-get install ninja-build".

### Compiler and Linker

By default we use g++ to compile and link. This is usually preinstalled on your Raspberry Pi.

## Compiling

The CSTest and CSPong sample projects come with a build script that you can use to compile your game (also the CSProjectGenerator will generate the build script for new projects). By default the build script is setup to build on the Raspberry Pi itself. Simply add *build.py* to */Projects/RPi/* and then navigate to that directory in terminal and type "python build.py debug" to start building (note, you can replace "debug" with "release"). This will generate an exe and required assets in an Output folder in the same directory.

If you want to clean the build type "python build.py debug|release clean".

By default the build process uses the built in g++ compiler and linker. If you want to change to another compiler (e.g. clang) change the paths at the top of *build.py*.

NOTE: Make sure you have installed Ninja prior to running the build script.

### Cross Compiling

Compiling on the Pi can be tedious so it is probably worth setting up a cross compile toolchain that allows you to compile the exe on a different machine and simply copy the *Output* folder to the Pi.

We recommend downloading prebuilt toolchains like the following:

- [Windows](http://gnutoolchains.com/raspberry/tutorial/)
- [Mac OS](http://www.jaredwolff.com/blog/cross-compiling-on-mac-osx-for-raspberry-pi/)

Once you have downloaded the toolchains simply change the CC, AR and LD variables at the top of *build.py* to point to the cross compiler/linker instead.

You will also need to pull libraries from the Pi onto your compiling machine so that the linker can link correctly (e.g. X11, GLES, etc). Libraries should be added into a folder called *CrossLibs* in the same directory as *build.py*. The following libraries are required to cross compile:

- libbcm_host.so
- libEGL.so
- libGLESv2.so
- libvchiq_arm.so
- libvcos.so
- libX11.so.6.3.0
- libXau.so.6.0.0
- libxcb-xkb.so.1.0.0
- libxcb.so.1.1.0
- libXdmcp.so.6.0.0
- libxkbcommon-x11.so.0.0.0
- libxkbcommon.so.0.0.0

NOTES:
- You can "sudo apt-get install locate" to help you find the libraries on the Pi. Once installed simply type "locate LibName".
- If you are compiling on Windows using the downloaded Ninja binary you will need to add Ninja to the PATH environment variable.
- The default build script limits the number of jobs that Ninja will do in parallel to two (if you don't restrict Ninja it will crash the Pi). If you are cross compiling then feel free to remove this restriction to improve build times.

## App Configuration

You will need to edit your App.config JSON file to add the following to the main root:

```
"RPi": {
  "CursorType": "NonSystem",
  "WindowDisplayMode": "Windowed"
}
```

The display mode instructs ChilliSource whether to start your game "Windowed" or whether to start it "Fullscreen".

NOTE: If you start the game fullscreen make sure you have some way to call CS::Application::Get()->Quit() as the window will have no close button.

The cursor type is required if you want to see a mouse cursor. Due to the way the hardware accelerated display window works it always renders on top of the OS and therefore obscures the system mouse cursor. Setting the cursor type to "NonSystem" will render a software cursor (NOTE: The cursor is just a widget and can be configured to suit your game). If you want to hide the cursor completely just set the cursor type to "None".

## Running your Game

If you want to see debug output from your application you should run it from terminal by navigating to the location of your app in terminal and then typing ./AppName to execute. If you wish to run your app without debug output simply double-click the icon.

NOTE: If you cannot run the app due to "Permission denied" you may need to make it executable by typing "chmod +x AppName" into a terminal.

## Debugging

If you build a debug build then it will contain debug symbols. You can debug the app on the Pi using gdb (provided you are using the g++/gcc compiler). For example if you want to grab the stack trace of a crash run the app in the following way:

```
gdb ./AppName
catch throw
run
```

Once the crash is triggered type:

```
bt
```

This will display the call stack.
