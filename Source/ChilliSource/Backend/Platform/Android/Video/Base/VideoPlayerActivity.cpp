/*
 *  VideoPlayerActivity.h
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Video/Base/VideoPlayerActivity.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Video/Base/VideoPlayerJavaInterface.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Video/Base/Subtitles.h>

namespace ChilliSource
{
    namespace Android
    {
        //--------------------------------------------------------------
        /// Constructor
        //--------------------------------------------------------------
        VideoPlayerActivity::VideoPlayerActivity()
        	: mCurrentSubtitleTimeMS(0)
        {
        	//get the media player java interface or create it if it doesn't yet exist.
        	mpVideoPlayerJavaInterface = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<VideoPlayerJavaInterface>();
        	if (mpVideoPlayerJavaInterface == nullptr)
        	{
        		mpVideoPlayerJavaInterface = VideoPlayerJavaInterfaceSPtr(new VideoPlayerJavaInterface());
        		JavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpVideoPlayerJavaInterface);
        	}
        }
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool VideoPlayerActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == Video::VideoPlayerActivity::InterfaceID;
		}
        //--------------------------------------------------------------
        /// Present
        //--------------------------------------------------------------
        void VideoPlayerActivity::Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour)
        {
        	//calculate the storage location and full filename.
        	bool bIsPackage;
        	std::string strFilename;
        	if (ineLocation == Core::StorageLocation::k_package)
        	{
        		bIsPackage = true;
        		strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(Core::StorageLocation::k_package, instrFileName);
        	}
        	else if (ineLocation == Core::StorageLocation::k_DLC)
			{
        		if (Core::Application::Get()->GetFileSystem()->DoesFileExistInCachedDLC(instrFileName) == true)
        		{
        			bIsPackage = false;
        			strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + instrFileName;
        		}
        		else
        		{
        			bIsPackage = true;
        			strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToFile(Core::StorageLocation::k_package, Core::Application::Get()->GetFileSystem()->GetPackageDLCPath() + instrFileName);
        		}
			}
        	else
        	{
        		bIsPackage = false;
        		strFilename = Core::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(ineLocation) + instrFileName;
        	}

        	//start the video
        	OnVideoDismissedDelegate dismissedDelegate(Core::MakeDelegate(this, &VideoPlayerActivity::OnVideoDismissed));
        	OnVideoStoppedDelegate stoppedDelegate(Core::MakeDelegate(this, &VideoPlayerActivity::OnVideoStopped));
        	mpVideoPlayerJavaInterface->Present(bIsPackage, strFilename, inbCanDismissWithTap, inBackgroundColour, dismissedDelegate, stoppedDelegate);
        }
        //--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void VideoPlayerActivity::PresentWithSubtitles(Core::StorageLocation ineVideoLocation, const std::string& instrVideoFilename,
														const Video::SubtitlesCSPtr& in_subtitles,
														bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour)
		{
			mpSubtitles = in_subtitles;
			mpVideoPlayerJavaInterface->SetUpdateSubtitlesDelegate(Core::MakeDelegate(this, &VideoPlayerActivity::OnUpdateSubtitles));
			Present(ineVideoLocation, instrVideoFilename, inbCanDismissWithTap, inBackgroundColour);
		}
        //--------------------------------------------------------------
        /// Is Playing
        //--------------------------------------------------------------
        bool VideoPlayerActivity::IsPlaying() const
        {
        	return mpVideoPlayerJavaInterface->IsPlaying();
        }
        //--------------------------------------------------------------
        /// Get Duration
        //--------------------------------------------------------------
        f32 VideoPlayerActivity::GetDuration() const
        {
        	return mpVideoPlayerJavaInterface->GetDuration();
        }
        //--------------------------------------------------------------
        /// Get Time
        //--------------------------------------------------------------
        f32 VideoPlayerActivity::GetTime() const
		{
        	return mpVideoPlayerJavaInterface->GetTime();
		}
        //--------------------------------------------------------------
        /// Dismiss
        //--------------------------------------------------------------
        void VideoPlayerActivity::Dismiss()
        {
        	return mpVideoPlayerJavaInterface->Dismiss();
        }
		//---------------------------------------------------------------
		/// On Video Dismissed
		//---------------------------------------------------------------
		void VideoPlayerActivity::OnVideoDismissed()
		{
			//This is being called on either the video player UI thread or
			//The video player surface thread. Its is also more than likely
			//being called at a time when the moFlow activity is not active
			//so we are going to fire off a main thread task to ensure the
			//playback complete event occurs on a thread and at a time the
			//user would expect.

			Core::Task<> task(this, &VideoPlayerActivity::VideoDismissedTask);
			Core::TaskScheduler::ScheduleMainThreadTask(task);
		}
        //---------------------------------------------------------------
        /// On Video Stopped
        //---------------------------------------------------------------
        void VideoPlayerActivity::OnVideoStopped()
        {
        	//This is being called on either the video player UI thread or
        	//The video player surface thread. Its is also more than likely
        	//being called at a time when the moFlow activity is not active
        	//so we are going to fire off a main thread task to ensure the
        	//playback complete event occurs on a thread and at a time the
        	//user would expect.

        	Core::Task<> task(this, &VideoPlayerActivity::VideoStoppedTask);
			Core::TaskScheduler::ScheduleMainThreadTask(task);
        }
		//---------------------------------------------------------------
		/// Video Dismissed Task
		//---------------------------------------------------------------
		void VideoPlayerActivity::VideoDismissedTask()
		{
			mOnDismissedEvent.NotifyConnections();
		}
		//---------------------------------------------------------------
		/// Video Stopped Task
		//---------------------------------------------------------------
		void VideoPlayerActivity::VideoStoppedTask()
		{
			mOnPlaybackCompleteEvent.NotifyConnections();
			mpSubtitles.reset();
		}
		//---------------------------------------------------------------
		/// Update Subtitles
		//---------------------------------------------------------------
		void VideoPlayerActivity::OnUpdateSubtitles()
		{
			//only update if the position in the video has changed.
			f32 fPosition = GetTime();
			TimeIntervalMs currentTimeMS = (TimeIntervalMs)(fPosition * 1000.0f);
			if (mCurrentSubtitleTimeMS != currentTimeMS)
			{
				mCurrentSubtitleTimeMS = currentTimeMS;

				//get the current subtitles
				auto pSubtitleArray = mpSubtitles->GetSubtitlesAtTime(mCurrentSubtitleTimeMS);

				//add any new subtitles
				for (auto it = pSubtitleArray.begin(); it != pSubtitleArray.end(); ++it)
				{
					auto mapEntry = maSubtitleMap.find(*it);
					if (mapEntry == maSubtitleMap.end())
					{
						Core::UTF8String strText = Core::LocalisedText::GetText((*it)->m_textId);
						const Video::Subtitles::Style* pStyle = mpSubtitles->GetStyleWithName((*it)->m_styleName);
						s64 lwSubtitleID = mpVideoPlayerJavaInterface->CreateSubtitle(strText, pStyle->m_fontName, pStyle->m_fontSize, Rendering::StringFromAlignmentAnchor(pStyle->m_alignment), pStyle->m_bounds.vOrigin.x, pStyle->m_bounds.vOrigin.y, pStyle->m_bounds.vSize.x, pStyle->m_bounds.vSize.y);
						mpVideoPlayerJavaInterface->SetSubtitleColour(lwSubtitleID, 0.0f, 0.0f, 0.0f, 0.0f);
						maSubtitleMap.insert(std::make_pair(*it, lwSubtitleID));
					}
				}

				//update the current text views
				for (auto it = maSubtitleMap.begin(); it != maSubtitleMap.end(); ++it)
				{
					UpdateSubtitle(it->first, it->second, mCurrentSubtitleTimeMS);
				}

				//removes any text views that are no longer needed.
				for (auto it = maSubtitlesToRemove.begin(); it != maSubtitlesToRemove.end(); ++it)
				{
					auto mapEntry = maSubtitleMap.find(*it);
					if (mapEntry != maSubtitleMap.end())
					{
						mpVideoPlayerJavaInterface->RemoveSubtitle(mapEntry->second);
						maSubtitleMap.erase(mapEntry);
					}
				}
				maSubtitlesToRemove.clear();
			}
		}
		//---------------------------------------------------------------
		/// Update Subtitle
		//---------------------------------------------------------------
		void VideoPlayerActivity::UpdateSubtitle(const Video::Subtitles::Subtitle* inpSubtitle, s64 inlwSubtitleID, TimeIntervalMs inTimeMS)
		{
			const Video::Subtitles::Style* pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->m_styleName);

			f32 fFade = 0.0f;
			s64 lwRelativeTime = ((s64)inTimeMS) - ((s64)inpSubtitle->m_startTimeMS);
			s64 lwDisplayTime = ((s64)inpSubtitle->m_endTimeMS) - ((s64)inpSubtitle->m_startTimeMS);

			//subtitle should not be displayed yet so remove
			if (lwRelativeTime < 0)
			{
				maSubtitlesToRemove.push_back(inpSubtitle);
			}

			//fading in
			else if (lwRelativeTime < pStyle->m_fadeTimeMS)
			{
				fFade = ((f32)lwRelativeTime) / ((f32)pStyle->m_fadeTimeMS);
			}

			//active
			else if (lwRelativeTime < lwDisplayTime - pStyle->m_fadeTimeMS)
			{
				fFade = 1.0f;
			}

			//fading out
			else if (lwRelativeTime < lwDisplayTime)
			{
				fFade = 1.0f - (((f32)lwRelativeTime - (lwDisplayTime - pStyle->m_fadeTimeMS)) / ((f32)pStyle->m_fadeTimeMS));
			}

			//should no longer be displayed so remove
			else if (lwRelativeTime >= lwDisplayTime)
			{
				maSubtitlesToRemove.push_back(inpSubtitle);
			}

			mpVideoPlayerJavaInterface->SetSubtitleColour(inlwSubtitleID, pStyle->m_colour.r, pStyle->m_colour.g, pStyle->m_colour.b, fFade * pStyle->m_colour.a);
		}
    }
}
