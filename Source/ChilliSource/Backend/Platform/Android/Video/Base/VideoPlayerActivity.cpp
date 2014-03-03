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
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Video/Base/SubtitlesManager.h>

namespace ChilliSource
{
    namespace Android
    {
        //--------------------------------------------------------------
        /// Constructor
        //--------------------------------------------------------------
        CVideoPlayerActivity::CVideoPlayerActivity()
        	: mCurrentSubtitleTimeMS(0)
        {
        	//get the media player java interface or create it if it doesn't yet exist.
        	mpVideoPlayerJavaInterface = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CVideoPlayerJavaInterface>();
        	if (mpVideoPlayerJavaInterface == NULL)
        	{
        		mpVideoPlayerJavaInterface = VideoPlayerJavaInterfacePtr(new CVideoPlayerJavaInterface());
        		CJavaInterfaceManager::GetSingletonPtr()->AddJavaInterface(mpVideoPlayerJavaInterface);
        	}
        }
		//--------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------
		bool CVideoPlayerActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == Video::VideoPlayerActivity::InterfaceID;
		}
        //--------------------------------------------------------------
        /// Present
        //--------------------------------------------------------------
        void CVideoPlayerActivity::Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour)
        {
        	//calculate the storage location and full filename.
        	bool bIsPackage;
        	std::string strFilename;
        	if (ineLocation == Core::StorageLocation::k_package)
        	{
        		bIsPackage = true;
        		strFilename = Core::Application::GetFileSystemPtr()->GetDirectoryForPackageFile(instrFileName);
        	}
        	else if (ineLocation == Core::StorageLocation::k_DLC)
			{
        		if (Core::Application::GetFileSystemPtr()->DoesFileExistInCachedDLC(instrFileName) == true)
        		{
        			bIsPackage = false;
        			strFilename = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrFileName;
        		}
        		else
        		{
        			bIsPackage = true;
        			strFilename = Core::Application::GetFileSystemPtr()->GetDirectoryForPackageFile(Core::Application::GetFileSystemPtr()->GetPackageDLCDirectory() + instrFileName);
        		}
			}
        	else
        	{
        		bIsPackage = false;
        		strFilename = Core::Application::GetFileSystemPtr()->GetStorageLocationDirectory(ineLocation) + instrFileName;
        	}

        	//start the video
        	OnVideoDismissedDelegate dismissedDelegate(Core::MakeDelegate(this, &CVideoPlayerActivity::OnVideoDismissed));
        	OnVideoStoppedDelegate stoppedDelegate(Core::MakeDelegate(this, &CVideoPlayerActivity::OnVideoStopped));
        	mpVideoPlayerJavaInterface->Present(bIsPackage, strFilename, inbCanDismissWithTap, inBackgroundColour, dismissedDelegate, stoppedDelegate);
        }
        //--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void CVideoPlayerActivity::PresentWithSubtitles(Core::StorageLocation ineVideoLocation, const std::string& instrVideoFilename,
														Core::StorageLocation ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
														bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour)
		{
			mpSubtitles = LOAD_RESOURCE(Video::Subtitles, ineSubtitlesLocation, instrSubtitlesFilename);
			mpVideoPlayerJavaInterface->SetUpdateSubtitlesDelegate(Core::MakeDelegate(this, &CVideoPlayerActivity::OnUpdateSubtitles));
			Present(ineVideoLocation, instrVideoFilename, inbCanDismissWithTap, inBackgroundColour);
		}
        //--------------------------------------------------------------
        /// Is Playing
        //--------------------------------------------------------------
        bool CVideoPlayerActivity::IsPlaying() const
        {
        	return mpVideoPlayerJavaInterface->IsPlaying();
        }
        //--------------------------------------------------------------
        /// Get Duration
        //--------------------------------------------------------------
        f32 CVideoPlayerActivity::GetDuration() const
        {
        	return mpVideoPlayerJavaInterface->GetDuration();
        }
        //--------------------------------------------------------------
        /// Get Time
        //--------------------------------------------------------------
        f32 CVideoPlayerActivity::GetTime() const
		{
        	return mpVideoPlayerJavaInterface->GetTime();
		}
        //--------------------------------------------------------------
        /// Dismiss
        //--------------------------------------------------------------
        void CVideoPlayerActivity::Dismiss()
        {
        	return mpVideoPlayerJavaInterface->Dismiss();
        }
		//---------------------------------------------------------------
		/// On Video Dismissed
		//---------------------------------------------------------------
		void CVideoPlayerActivity::OnVideoDismissed()
		{
			//This is being called on either the video player UI thread or
			//The video player surface thread. Its is also more than likely
			//being called at a time when the moFlow activity is not active
			//so we are going to fire off a main thread task to ensure the
			//playback complete event occurs on a thread and at a time the
			//user would expect.

			Core::Task<> task(this, &CVideoPlayerActivity::VideoDismissedTask);
			Core::TaskScheduler::ScheduleMainThreadTask(task);
		}
        //---------------------------------------------------------------
        /// On Video Stopped
        //---------------------------------------------------------------
        void CVideoPlayerActivity::OnVideoStopped()
        {
        	//This is being called on either the video player UI thread or
        	//The video player surface thread. Its is also more than likely
        	//being called at a time when the moFlow activity is not active
        	//so we are going to fire off a main thread task to ensure the
        	//playback complete event occurs on a thread and at a time the
        	//user would expect.

        	Core::Task<> task(this, &CVideoPlayerActivity::VideoStoppedTask);
			Core::TaskScheduler::ScheduleMainThreadTask(task);
        }
		//---------------------------------------------------------------
		/// Video Dismissed Task
		//---------------------------------------------------------------
		void CVideoPlayerActivity::VideoDismissedTask()
		{
			mOnDismissedEvent.NotifyConnections();
		}
		//---------------------------------------------------------------
		/// Video Stopped Task
		//---------------------------------------------------------------
		void CVideoPlayerActivity::VideoStoppedTask()
		{
			mOnPlaybackCompleteEvent.NotifyConnections();
			mpSubtitles.reset();
		}
		//---------------------------------------------------------------
		/// Update Subtitles
		//---------------------------------------------------------------
		void CVideoPlayerActivity::OnUpdateSubtitles()
		{
			//only update if the position in the video has changed.
			f32 fPosition = GetTime();
			TimeIntervalMs currentTimeMS = (TimeIntervalMs)(fPosition * 1000.0f);
			if (mCurrentSubtitleTimeMS != currentTimeMS)
			{
				mCurrentSubtitleTimeMS = currentTimeMS;

				//get the current subtitles
				std::vector<Video::Subtitles::SubtitlePtr> pSubtitleArray = mpSubtitles->GetSubtitlesAtTime(mCurrentSubtitleTimeMS);

				//add any new subtitles
				for (std::vector<Video::Subtitles::SubtitlePtr>::iterator it = pSubtitleArray.begin(); it != pSubtitleArray.end(); ++it)
				{
					std::unordered_map<Video::Subtitles::SubtitlePtr, s64>::iterator mapEntry = maSubtitleMap.find(*it);
					if (mapEntry == maSubtitleMap.end())
					{
						Core::UTF8String strText = Core::LocalisedText::GetText((*it)->strTextID);
						Video::Subtitles::StylePtr pStyle = mpSubtitles->GetStyleWithName((*it)->strStyleName);
						s64 lwSubtitleID = mpVideoPlayerJavaInterface->CreateSubtitle(strText, pStyle->strFontName,pStyle->udwFontSize, Rendering::StringFromAlignmentAnchor(pStyle->eAlignment), pStyle->Bounds.vOrigin.x, pStyle->Bounds.vOrigin.y, pStyle->Bounds.vSize.x, pStyle->Bounds.vSize.y);
						mpVideoPlayerJavaInterface->SetSubtitleColour(lwSubtitleID, 0.0f, 0.0f, 0.0f, 0.0f);
						maSubtitleMap.insert(std::pair<Video::Subtitles::SubtitlePtr, s64>(*it, lwSubtitleID));
					}
				}

				//update the current text views
				for (std::unordered_map<Video::Subtitles::SubtitlePtr, s64>::iterator it = maSubtitleMap.begin(); it != maSubtitleMap.end(); ++it)
				{
					UpdateSubtitle(it->first, it->second, mCurrentSubtitleTimeMS);
				}

				//removes any text views that are no longer needed.
				for (std::vector<Video::Subtitles::SubtitlePtr>::iterator it = maSubtitlesToRemove.begin(); it != maSubtitlesToRemove.end(); ++it)
				{
					std::unordered_map<Video::Subtitles::SubtitlePtr, s64>::iterator mapEntry = maSubtitleMap.find(*it);
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
		void CVideoPlayerActivity::UpdateSubtitle(const Video::Subtitles::SubtitlePtr& inpSubtitle, s64 inlwSubtitleID, TimeIntervalMs inTimeMS)
		{
			Video::Subtitles::StylePtr pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->strStyleName);

			f32 fFade = 0.0f;
			s64 lwRelativeTime = ((s64)inTimeMS) - ((s64)inpSubtitle->StartTimeMS);
			s64 lwDisplayTime = ((s64)inpSubtitle->EndTimeMS) - ((s64)inpSubtitle->StartTimeMS);

			//subtitle should not be displayed yet so remove
			if (lwRelativeTime < 0)
			{
				maSubtitlesToRemove.push_back(inpSubtitle);
			}

			//fading in
			else if (lwRelativeTime < pStyle->FadeTimeMS)
			{
				fFade = ((f32)lwRelativeTime) / ((f32)pStyle->FadeTimeMS);
			}

			//active
			else if (lwRelativeTime < lwDisplayTime - pStyle->FadeTimeMS)
			{
				fFade = 1.0f;
			}

			//fading out
			else if (lwRelativeTime < lwDisplayTime)
			{
				fFade = 1.0f - (((f32)lwRelativeTime - (lwDisplayTime - pStyle->FadeTimeMS)) / ((f32)pStyle->FadeTimeMS));
			}

			//should no longer be displayed so remove
			else if (lwRelativeTime >= lwDisplayTime)
			{
				maSubtitlesToRemove.push_back(inpSubtitle);
			}

			mpVideoPlayerJavaInterface->SetSubtitleColour(inlwSubtitleID, pStyle->Colour.r, pStyle->Colour.g, pStyle->Colour.b, fFade * pStyle->Colour.a);
		}
    }
}
