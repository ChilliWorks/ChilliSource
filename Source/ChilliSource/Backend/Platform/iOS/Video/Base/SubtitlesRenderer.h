//
//  SubtitlesRenderer.h
//  moFlow
//
//  Created by Ian Copland on 22/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_
#define _MOFLOW_PLATFORM_IOS_VIDEO_SUBTITLESRENDERER_H_

#include <ChilliSource/ChilliSource.h>
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
    ChilliSource::Video::SubtitlesSPtr mpSubtitles;
    ChilliSource::iOS::CVideoPlayerActivity* mpVideoPlayer;
    std::unordered_map<ChilliSource::Video::Subtitles::SubtitlePtr, UITextView*> maTextViewMap;
    std::vector<ChilliSource::Video::Subtitles::SubtitlePtr> maSubtitlesToRemove;
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
-(id) InitWithVideoPlayer:(ChilliSource::iOS::CVideoPlayerActivity*)inpVideoPlayer View:(UIView*)inpView StorageLocation:(ChilliSource::Core::StorageLocation)ineStorageLocation Filename:(std::string)instrFilename;
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
-(void) AddTextView:(const ChilliSource::Video::Subtitles::SubtitlePtr&)inpSubtitle;
//--------------------------------------------------------
/// Update Text View
///
/// Updates the given text view.
///
/// @param The text view.
/// @param the subtitle.
//--------------------------------------------------------
-(void) UpdateTextView:(UITextView*)inpTextView Subtitle:(const ChilliSource::Video::Subtitles::SubtitlePtr&)inpSubtitle Time:(TimeIntervalMs)inTimeMS;
//--------------------------------------------------------
/// Remove Text View
///
/// Queues the text view paired with the given subtitle
/// for removal.
///
/// @param The subtitle.
//--------------------------------------------------------
-(void) RemoveTextView:(const ChilliSource::Video::Subtitles::SubtitlePtr&)inpSubtitle;
//--------------------------------------------------------
/// Set Alignment
///
/// Sets the alignment of the text in the given text view.
///
/// @param the UITextView.
/// @param The alignment achor.
//--------------------------------------------------------
-(void) SetAlignment:(UITextView*)inpView WithAnchor:(ChilliSource::Core::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Text Alignment From Anchor
///
/// Converts from a moFlow alignment anchor to NSText
/// alignment.
///
/// @param The alignment achor.
/// @return the NSTextAlignment.
//--------------------------------------------------------
-(NSTextAlignment) TextAlignmentFromAnchor:(ChilliSource::Core::AlignmentAnchor)ineAnchor;
//--------------------------------------------------------
/// Calculate Text Box Rect
///
/// @return the rectangle in which the text box should
/// appear on screen.
//--------------------------------------------------------
-(CGRect) CalculateTextBoxRect:(const ChilliSource::Core::Rectangle&)inRelativeBounds;
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