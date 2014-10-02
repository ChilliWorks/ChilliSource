=== Chilli Source Change Log ===

Version 1.1.0, 2014-09-26:  
 	- Replaced the gesture system.
 	- UDIDs on Android are now generated from the Google Play Services Advertising Id when building for Google Play. This requires the inclusion of the Goole Play Services library in all Android projects.
 	- Adding accessor to Tween for getting the parametric T value
	- Scene::Remove() is no longer public, entities must be removed from the scene using RemoveFromParent(). 
	- Attempting to add a widget to the scene with a parent which is not in the scene will result in an assert. 
	- Fixed bug in TexturePacker algorithm which caused padding to break for atlases and fonts.
 	- Fixed iOS 8 issues with Push and Local Notifications. 
 	- Added new concurrent vector which is thread-safe and allows additions and removals while iterating.
 	- Fixed an issue with Android context restoration.
 	- Fixed a bug where Scene::RemoveAllEntities() would assert if there was an entity with a parent in the scene.
