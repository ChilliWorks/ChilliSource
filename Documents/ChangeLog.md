ChilliSource Change Log
========================

Version 1.5.2, 2016-03-18
-------------------------
* Fixed: FileSystem::GetDirectoryPath() now correctly creates parent paths recursively on Windows. This fixes the issue where the Documents/ directory would not be created on start up in certain circumstances.

Version 1.5.1, 2015-08-14
-------------------------
* Added: A new primitive shape model factory. This can be used to create boxes and planes in code.
* Added: A new primitive shape entity factory. This creates new entities using shapes generated through the primitive shape model factory.
* Added: Support for "multi-dex" applications on Android. This is needed if your application exceeds to 65k method limit.
* Added: A new way of handling JNI callbacks from Java to Native on Android. The method should now provide a BoxedPointer, which can be used to retreive a pointer to the target native object
* Improved: The Change Over Lifetime particle affector now supports 'intermediate colours', meaning it can fade through a series of different colours over the lifetime
* Improved: It is now possible to get the expected size and current progress of a download using the Http Request System
* Improved: Reimplemented Http Request on Android. It will no longer block the background task queue if several requests are made at the same time.
* Improved: It is now possible to get the jobject from a JavaClass.
* Fixed: Row and column major ordering is now the correct way round in a Grid Layout
* Fixed: Materials used by the Canvas Renderer and now correctly relinquished at the end of each frame, fixing the issue where the textures could not be manually released
* Fixed: An issue with the minus operator in all math vectors where the vector itself was modified rather than a copy, leading the following returning true: b = -a; return (a == b);
* Fixed: Vector2::Angle now returns an absolute angle to be consistent with how Vector3::Angle() works
* Fixed: A crash bug that would occur under certain conditions when using Application::Quit() on Android
* Fixed: System dialogue boxes now correctly display on Windows
* Fixed: The main thead id is now correctly updated during the OnDestroy() life cycle event on Android
* Fixed: An issue where pushing an Apk Expansion file to device would fail becuase a directory doesn't exist.
* Fixed: A crash related to WebView in Android release builds.
* Fixed: An issue with clear() in the Java class DynamicByteBuffer

Version 1.5.0, 2015-07-10
-------------------------
* Added: The android build pipeline now includes gradle tasks for packaging resources in the Apk Expansion file and pushing it to device.
* Added: The APK expansion downloader screen can now be overridden by the user.
* Added: Implemented a new, much easier to use system for handling JNI calls on Android. Native to Java calls are now all handled through JavaClass or JavaStaticClass. These include a number of sanity checks and provide more intuitive error messages. Java to Native still works as before.
* Added: Implemented JavaSystem, similar to JavaClass, but specifically for andriod java classes which extend System.
* Added: The android java class FileUtils now includes separate methods for reading and writing binary and text files.
* Added: FileSystem now includes methods for checking if a directory exists in the cached or package DLC locations.
* Added: The java tools ZipUtils now includes the ability to zip the contents of a directory.
* Added: A new java tool for cross-platform zipping of directories.
* Removed: The previous android expansion system has been removed.
* Removed: The android Power Manager is now redundant and has been removed.
* Removed: The GetAbsolutePathToFile() and GetAbsolutePathToDirectory() methods in FileSystem are no longer relevant with changes made to the system a while ago, and therefore have been removed.
* Improved: Updated the android projects to Android Studio 1.2. This included updating to gradle 1.2.3, build tools version 22.0.1 and sdk version 22.
* Improved: Support for Android Expansion files has been greatly improved. Google Play builds will now be automatically downloaded on startup if not already present. This occurs prior to any game code being executed so the user no longer need to handle the case where game assets do not yet exist. The downloaded expansion file is no longer unpacked meaning the app will be much smaller on disc, and a number of possible complications are avoided. A default downloader screen is provided by the engine.
* Improved: The android file system implementation has been significantly overhauled. Google Play flavoured builds now refer to the main APK expansion file rather than the APK when the Package, ChilliSource or DLC storage locations are requested. Amazon builds still use the APK.
* Improved: Significantly tidied up and improved the android gradle build scripts. 
* Improved: Restructured the Android backend.
* Improved: Renamed the android java class NativeInterface and changed it so that it follows the same lifecycle rules as systems in the native side of the engine.
* Improved: Renamexd JavaInterfaceUtils to JavaUtils.
* Improved: Simplified the creation and destruction of file streams. Calls to FileSystem::CreateFileStream() will now either return a valid ready to use stream, or null if it failed. A stream is no longer manually closed, instead it will be automatically cleaned up when it goes out of scope.
* Improved: The LVL key for Android Google Play builds should now be provided once in App.cpp, rather than being provided to each system that needs it in different ways.
* Improved: It is now possible to get access to the DrawableDef in a UI::DrawableComponent.
* Improved: Refactored and improved the android java StringUtils class.
* Improved: Refactored the android java class IQueryableInterface.
* Improved: The TaggedPathResolver now returns the input path if the path doesn't exist. This means requests to ResourcePool will print more helpful error messages.
* Improved: Refactored the android VideoPlayer backend so that it uses JavaSystem instead of JavaInterface. 
* Improved: The android manifest builder has been updated to reflect the changes to the Android project structure. Added new option for specifying a custom APK expansion downloader view.
* Improved: The project generator has been updated to include the new Android project structure changes.
* Fixed: A number of inconsistencies and bugs in the Android and Windows FileSystem have been fixed.
* Fixed: A FileStream referring to the Package, ChilliSource or packaged DLC storage locations on Android no longer allows write operations.
* Fixed: A bug where tagged asset paths were incorrectly handled for CkBank resources has been resolved.
* Deprecated: JavaInterface and _JavaInteface are both now deprecated and will be removed in the near future.

Version 1.4.5, 2015-06-26
-------------------------
* Added: Additional data on IAPs can now be retreived through the Android and iOS backends for IAPSystem.
* Fixed: Application and State no longer log a warning if a system couldn't be found using GetSystem<>()
* Fixed: Fixed a number of fatal errors which were incorrectly reported in the PNG To CSImage tool.
* Fixed: The Android build process will now correctly halt if there are compiler errors during the native build.

Version 1.4.4, 2015-06-05
-------------------------
* Fixed: A crash when removing Entities with children from the scene.
* Fixed: Child widgets are now correctly removed from their parent when the parent is deleted.
* Fixed: An issue that would result in auto-scaling text not rendering.

Version 1.4.3, 2015-05-21
-------------------------
* Removed: The GUI model and animation converter it had a number of issues and was windows specific. It will be replaced with a cross platform equivalent at some point in the future, but for now the command line tools should be used.
* Fixed: A label which contains icons in the text will no longer crash if label is too small to fit one of the icons.
* Fixed: Icons in labels are now correctly sized if the icon is from a texture altas.
* Fixed: SetWorldTransform() in Transform now works correctly.
* Fixed: An issue where Widgets could receive incorrect input events when moving.
* Fixed: The Cricket Audio system now correctly works with assets in the DLC storage location.
* Fixed: The Cricket Audio system, Android Video Player, Android WebView, and Android Email Composer now all work correctly with tagged assets.
* Fixed: An issue in the Content Management System where some packages would re-downloaded unnecessarily.

Version 1.4.2, 2015-05-01
-------------------------
* Added: Label now supports auto sizing to ensure text will fit inside the label bounds
* Fixed: Issues with resolution on iPhone 6 Plus
* Fixed: Changing resolution on windows in code now correctly fires the On Resolution Changed event
* Fixed: It is no longer possible to try and run in fullscreen mode with an invalid resolution on Windows
* Fixed: The ChilliSource android project no longer runs proguard, which was causing issues in projects which contain their own Java source
* Fixed: Animated Collada files exported from Blender should now be correctly converted by the collada conversion tools

Version 1.4.1, 2015-04-17
-------------------------
* Improved: Updated the ChangeLog.md to correct markdown syntax
* Fixed: The project generator no longer fails if there is a ".git" directory in the ChilliSource directory
* Fixed: Windows builds no longer fail during the asset copy python script if there is a space in the path name
* Fixed: The view frustum is now correctly updated when the screen resolution changes. This fixed an issue with on screen objects being culled

Version 1.4.0, 2015-04-03
-------------------------
* Added: Support for Android Studio. All projects created by the Project Generator now provide an Android Studio project instead of Eclipse ADT.
* Added: Support for 64-bit builds to Windows and iOS projects. 64-bit Android projects will be coming soon!
* Added: Double2, Double3, and Double4 math vector types.
* Removed: Support for Eclipse ADT builds. Eclipse is no longer officially supported as an Android development IDE.
* Improved: The widget hierarchy is now accessible through Canvas.
* Improved: DrawableDef no longer requires colour and UVs in their constructor.
* Improved: Restructured the Android backend to mirror the structure used for different product flavours in Android Studio.
* Improved: It is now much easier to build Kindle builds.
* Fixed: Sprite anchors now work correctly.
* Fixed: Text now correctly changes colour when the parent widget's colour changes.
* Fixed: Colour can now be pre-multiplied/divided/subtracted/added by a scalar.
* Fixed: A compiler error on Windows when using the Math::Random::Generate() default parameters.
* Fixed: CkBank resources are now correctly cleaned up if they still exist during the Application::OnDestroy lifecycle event.
* Fixed: Projects created by the Project Generator on Windows can now be correctly built in XCode.
* Fixed: IAP will no longer fail on Android if there are more than 20 available products.
* Fixed: A compiler error on Windows caused by the lack of virtual destructor in the Event interfaces.
* Fixed: The project generator no longer leaves git remnants in generated projects.

Version 1.3.3, 2015-02-24
-------------------------
* Labels now support inclusion of Icons in text.
* Improved the Facebook Post system and updated to the latest version of the Facebook SDK on both iOS and Android. 
* Facebook on Android is now an Android Extension. To use it add CS_ANDROIDEXTENSION_FACEBOOK to the pre-processor definitions in Android.mk and import the Facebook library project into eclipse. This can be found in the engine in the Libraries/Android/Projects/ directory.
* The Google Play Services android library project is now included in the engine, and can be found in the Libraries/Android/Projects/ directory. This should be used instead of copying a new version into the workspace.
* The CkAudioPlayer now has separate setters for music and effect volume.
* Fixed Android compiler errors when building on a linux machine. This was caused by in-correct case in include paths.
* Change errant log when failing to open file in JsonUtils.

Version 1.3.2, 2015-01-23
-------------------------
* Added new tool which provides a cross platform interface for the Cricket Audio cktool.
* Made a number of improvements to projects generated by the Project Generator including: improved default asset pipeline, iOS 8 launch images and icons and fixes.
* Fix: Solved an issue with md5 and sha1 file hashing on Android.
* Fix: RTT can be used without supplying a UI canvas.
* Fix: It's now possible to have a null drawable on a ui widget.

Version 1.3.1, 2015-01-09
-------------------------
* Added convenience methods to Widget Factory for building all the in-built widget types.
* Added a creation method for creating Particle Effect Components to the Render Component Factory.
* Fixed a crash bug on Android.

Version 1.3.0, 2015-01-09
-------------------------
* Added the new UI system.
* Added a new particle system.
* Added a Cricket audio system.
* Added a new property map for containing heterogeneous types.
* Added new dynamic_array which is based on std::dynarray which was originally proposed in the C++14 standard, but was later removed to be re-added at a later date.
* Added Random utilty class for thread-safe random number generation.
* Added new Json utility class, which provides convenience methods for working with Json.
* Improved the Tween API.
* Removed several features that have been superceeded by improved implementations: The old GUI system, the old particle system, MathUtils::FRand() and Utils::ReadJson().
* Removed the reflection functionality as it was incomplete and only used by the old GUI system.
* Removed Debug Stats as it relied heavily on the old GUI system. This will be re-implemented in the new system in the not too distant future.

Version 1.2.1, 2014-11-17
-------------------------
* Models exported from Blender can now be converted to CSModel format using the ColladaToCSModel tool.
* Improved the HTTP Request System API. Also replaced the iOS HTTP Request System internals with NSURLConnection. This resulted in removal of GetBytesRead() from the HTTP interface.
* Updated iOS Facebook SDK.
* ContentDownloader now allows a custom hashing delegate to be provided rather than being explicitly MD5.
* ContentDownloader now defaults to using SHA-1 hash.
* Fixed a bug in the font maker tool which resulted in different characters that were identical being rendered incorrectly.
* Fixed a bug that prevented the font maker tool runnning on windows.
* Fixed a bug with state system that could result in resume/suspend being called out of order causing an assertion.
* Fix for MakeConnectableDelegate which was crashing due to implicit move.

Version 1.2.0, 2014-11-07
-------------------------
* Replaced the existing Font Builder tool with a new version. Unlike the previous tool this can be run on Windows, OSX and Linux, and fonts can be generated from command line. Fonts can also have a number of effects applied including outlines, glow and drop shadows.
* Font rendering has been significantly improved. This involved updating the font format so any existing fonts will need to be rebuilt. 
* CSModelExport is now correctly working again.
* Updated the android backend for Android 5.0.
* Updated the android backed to use the latest NDK build pipeline.

Version 1.1.5, 2014-11-03
-------------------------
* Fixed a windows specific crash on closing an application.

Version 1.1.4, 2014-10-24
-------------------------
* Changed the Application::GetSystems() method so that it returns a vector of systems rather than taking a vector of systems as an output parameter.
* Added a GetSystems() method to State.
* Added constant versions of system getters to Application and State.
* Changed WorkerQueue to concurrent_blocking_queue and changed its API to ensure it is thread-safe. This solves the thread-safety issues with the TaskScheduler.
* Fixed issue with missing includes in the Vector classes and StandardMacros.h.

Version 1.1.3, 2014-10-20
-------------------------
* Project generator will now error if the output directory is inside ChilliSource. This was causing recursive file copy issues when copying ChilliSource to the project directory.

Version 1.1.2, 2014-10-16
-------------------------
* Added CSProjectGenerator, a tool for generating new ChilliSource project.

Version 1.1.1, 2014-10-10
-------------------------
* All App Systems now correctly receive life cycle events prior to States.
* Simplified the command line interface for the ColladaToCSModel and ColladaToCSAnim tools and re-added the ability to change coordinate system handedness.
* Updated the package name in all java code from com.chillisource.* to com.chilliworks.chillisource.*
* Fixed a bug in Colour::Clamp() where by default alpha was clamping between 1 and 1 rather than 0 and 1.
* Swapped the parameters in Timer::OpenConnection() to be more lambda friendly.
* Fixed typos in the documentation for the CSAtlas Builder.

Version 1.1.0, 2014-09-26: 
-------------------------
* Replaced the gesture system.
* UDIDs on Android are now generated from the Google Play Services Advertising Id when building for Google Play. This requires the inclusion of the Goole Play Services library in all Android projects.
* Adding accessor to Tween for getting the parametric T value
* The GUI System no longer allows consumption of moved events.
* The Pointer System no longer has a filtered version of OnPointerMoved() as this event cannot be filtered.
* The engine is no longer built with CS_ENABLE_DEBUGSTATS enabled in debug mode. App projects should also remove this flag.
* Scene::Remove() is no longer public, entities must be removed from the scene using RemoveFromParent().
* Attempting to add a widget to the scene with a parent which is not in the scene will result in an assert. 
* Fixed bug in TexturePacker algorithm which caused padding to break for atlases and fonts.
* Fixed iOS 8 issues with Push and Local Notifications. 
* Added new concurrent vector which is thread-safe and allows additions and removals while iterating.
* Fixed an issue with Android context restoration.
* Fixed a bug where Scene::RemoveAllEntities() would assert if there was an entity with a parent in the scene.
