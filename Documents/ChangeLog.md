=== Chilli Source Change Log ===

Version 1.1.2 2014-10-16:
	- Added CSProjectGenerator, a tool for generating new Chilli Source project.

Version 1.1.1, 2014-10-10:
	- All App Systems now correctly receive life cycle events prior to States.
	- Simplified the command line interface for the ColladaToCSModel and ColladaToCSAnim tools and re-added the ability to change coordinate system handedness.
	- Updated the package name in all java code from com.chillisource.* to com.chilliworks.chillisource.*
	- Fixed a bug in Colour::Clamp() where by default alpha was clamping between 1 and 1 rather than 0 and 1.
	- Swapped the parameters in Timer::OpenConnection() to be more lambda friendly.
	- Fixed typos in the documentation for the CSAtlas Builder.

Version 1.1.0, 2014-09-26:  
 	- Replaced the gesture system.
 	- UDIDs on Android are now generated from the Google Play Services Advertising Id when building for Google Play. This requires the inclusion of the Goole Play Services library in all Android projects.
 	- Adding accessor to Tween for getting the parametric T value
 	- The GUI System no longer allows consumption of moved events.
 	- The Pointer System no longer has a filtered version of OnPointerMoved() as this event cannot be filtered.
 	- The engine is no longer built with CS_ENABLE_DEBUGSTATS enabled in debug mode. App projects should also remove this flag.
	- Scene::Remove() is no longer public, entities must be removed from the scene using RemoveFromParent().
	- Attempting to add a widget to the scene with a parent which is not in the scene will result in an assert. 
	- Fixed bug in TexturePacker algorithm which caused padding to break for atlases and fonts.
 	- Fixed iOS 8 issues with Push and Local Notifications. 
 	- Added new concurrent vector which is thread-safe and allows additions and removals while iterating.
 	- Fixed an issue with Android context restoration.
 	- Fixed a bug where Scene::RemoveAllEntities() would assert if there was an entity with a parent in the scene.
