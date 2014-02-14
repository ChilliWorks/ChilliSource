//
//  SubtitlesRenderer.h
//  moFlow
//
//  Created by Ian Copland on 22/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_
#define _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_

#include <ChilliSource/Video/ForwardDeclarations.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayerActivity.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


//========================================================
/// Subtitles Renderer
///
/// Renders subtitles on top of the video player.
//========================================================
@interface CSubtitlesRenderer : NSObject
{
    UIView* mpBaseView;
    CADisplayLink* mpDisplayLink;
    moFlo::Video::SubtitlesPtr mpSubtitles;
    moFlo::iOSPlatform::CVideoPlayerActivity* mpVideoPlayer;
    HASH_MAP<moFlo::Video::CSubtitles::SubtitlePtr, UITextView*> maTextViewMap;
    DYNAMIC_ARRAY<moFlo::Video::CSubtitles::SubtitlePtr> maSubtitlesToRemove;
    TimeIntervalMs mCurrentTimeMS;
}
//--------------------------------------------------------
/// init With Video Player
///
/// Initialises the subtitles renderer.
///
/// @param The view.
/// @param The subtitles storage location.
/// @param The subtitles filename.
//--------------------------------------------------------
-(id) InitWithVideoPlayer:(moFlo::iOSPlatform::CVideoPlayerActivity*)inpVideoPlayer View:(UIView*)inpView StorageLocation:(moFlo::Core::STORAGE_LOCATION)ineStorageLocation Filename:(std::string)instrFilename;
//--------------------------------------------------------
/// On Update
///
/// Called each frame.
//--------------------------------------------------------
-(void) OnUpdate;
//--------------------------------------------------------
/// Add Text View
///
/// Adds a new text view for the given subtitle.
///
/// @param the subtitle.
//--------------------------------------------------------
-(void) AddTextView:(const moFlo::Video::CSubtitles::SubtitlePtr&)inpSubtitle;
//--------------------------------------------------------
/// Update Text View
///
/// Updates the given text view.
///
/// @param The text view.
/// @param the subtitle.
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const moFlo::Video::CSubtitles::SubtitlePtr&)inpSubtitle Time:(TimeIntervalMs)inTimeMS;
//--------------------------------------------------------
/// Remove Text View
///
/// Queues the text view paired with the given subtitle
/// for removal.
///
/// @param The subtitle.
//--------------------------------------------------------
-(void) RemoveTextView:(const moFlo::Video::CSubtitles::SubtitlePtr&)inpSubtitle;
//--------------------------------------------------------
/// Set Alignment
///
/// Sets the alignment of the text in the given text view.
///
/// @param the UITextView.
/// @param The alignment achor.
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(moFlo::Core::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Text Alignment From Anchor
///
/// Converts from a moFlow alignment anchor to NSText
/// alignment.
///
/// @param The alignment achor.
/// @return the NSTextAlignment.
//--------------------------------------------------------
-(NSTextAlignment) TextAlignmentFromAnchor:(moFlo::Core::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Calculate Text Box Rect
///
/// @return the rectangle in which the text box should
/// appear on screen.
//--------------------------------------------------------
-(CGRect) CalculateTextBoxRect:(const moFlo::Core::Rectangle&)inRelativeBounds;
//--------------------------------------------------------
/// CleanUp
///
/// Cleans up any possible retainers, should be called before releasing
//--------------------------------------------------------
-(void) CleanUp;
//--------------------------------------------------------
/// Dealloc
///
/// destroys the listener
//--------------------------------------------------------
-(void) dealloc;
@end

#endif