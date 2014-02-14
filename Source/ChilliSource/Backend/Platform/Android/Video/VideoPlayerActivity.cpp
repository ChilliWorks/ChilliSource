/*
 *  VideoPlayerActivity.h
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/Video/VideoPlayerActivity.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/VideoPlayerJavaInterface.h>
#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Main/Screen.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Main/ApplicationEvents.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Locale/LocalisedText.h>
#include <ChilliSource/Video/Main/Subtitles.h>
#include <ChilliSource/Video/Main/SubtitlesManager.h>
#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>

namespace moFlo
{
    namespace AndroidPlatform
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
			return inID == Video::IVideoPlayerActivity::InterfaceID;
		}
        //--------------------------------------------------------------
        /// Present
        //--------------------------------------------------------------
        void CVideoPlayerActivity::Present(Core::STORAGE_LOCATION ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour)
        {
        	//calculate the storage location and full filename.
        	bool bIsPackage;
        	std::string strFilename;
        	if (ineLocation == Core::SL_PACKAGE)
        	{
        		bIsPackage = true;
        		strFilename = Core::CApplication::GetFileSystemPtr()->GetDirectoryForPackageFile(instrFileName);
        	}
        	else if (ineLocation == Core::SL_DLC)
			{
        		if (Core::CApplication::GetFileSystemPtr()->DoesFileExistInCachedDLC(instrFileName) == true)
        		{
        			bIsPackage = false;
        			strFilename = Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(Core::SL_DLC) + instrFileName;
        		}
        		else
        		{
        			bIsPackage = true;
        			strFilename = Core::CApplication::GetFileSystemPtr()->GetDirectoryForPackageFile(Core::CApplication::GetFileSystemPtr()->GetPackageDLCDirectory() + instrFileName);
        		}
			}
        	else
        	{
        		bIsPackage = false;
        		strFilename = Core::CApplication::GetFileSystemPtr()->GetStorageLocationDirectory(ineLocation) + instrFileName;
        	}

        	//start the video
        	OnVideoDismissedDelegate dismissedDelegate(this, &CVideoPlayerActivity::OnVideoDismissed);
        	OnVideoStoppedDelegate stoppedDelegate(this, &CVideoPlayerActivity::OnVideoStopped);
        	mpVideoPlayerJavaInterface->Present(bIsPackage, strFilename, inbCanDismissWithTap, inBackgroundColour, dismissedDelegate, stoppedDelegate);
        }
        //--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void CVideoPlayerActivity::PresentWithSubtitles(Core::STORAGE_LOCATION ineVideoLocation, const std::string& instrVideoFilename,
														Core::STORAGE_LOCATION ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
														bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour)
		{
			mpSubtitles = LOAD_RESOURCE(moFlo::Video::CSubtitles, ineSubtitlesLocation, instrSubtitlesFilename);
			mpVideoPlayerJavaInterface->SetUpdateSubtitlesDelegate(OnUpdateSubtitlesDelegate(this, &CVideoPlayerActivity::OnUpdateSubtitles));
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

			Task0 task(this, &CVideoPlayerActivity::VideoDismissedTask);
			CTaskScheduler::ScheduleMainThreadTask(task);
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

			Task0 task(this, &CVideoPlayerActivity::VideoStoppedTask);
			CTaskScheduler::ScheduleMainThreadTask(task);
        }
		//---------------------------------------------------------------
		/// Video Dismissed Task
		//---------------------------------------------------------------
		void CVideoPlayerActivity::VideoDismissedTask()
		{
			mOnDismissedEvent.Invoke();
		}
		//---------------------------------------------------------------
		/// Video Stopped Task
		//---------------------------------------------------------------
		void CVideoPlayerActivity::VideoStoppedTask()
		{
			mOnPlaybackCompleteEvent.Invoke();
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
				DYNAMIC_ARRAY<moFlo::Video::CSubtitles::SubtitlePtr> pSubtitleArray = mpSubtitles->GetSubtitlesAtTime(mCurrentSubtitleTimeMS);

				//add any new subtitles
				for (DYNAMIC_ARRAY<moFlo::Video::CSubtitles::SubtitlePtr>::iterator it = pSubtitleArray.begin(); it != pSubtitleArray.end(); ++it)
				{
					HASH_MAP<moFlo::Video::CSubtitles::SubtitlePtr, s64>::iterator mapEntry = maSubtitleMap.find(*it);
					if (mapEntry == maSubtitleMap.end())
					{
						moFlo::UTF8String strText = moFlo::Core::CLocalisedText::GetText((*it)->strTextID);
						moFlo::Video::CSubtitles::StylePtr pStyle = mpSubtitles->GetStyleWithName((*it)->strStyleName);
						s64 lwSubtitleID = mpVideoPlayerJavaInterface->CreateSubtitle(strText, pStyle->strFontName,pStyle->udwFontSize, Core::StringFromAlignmentAnchor(pStyle->eAlignment), pStyle->Bounds.vOrigin.x, pStyle->Bounds.vOrigin.y, pStyle->Bounds.vSize.x, pStyle->Bounds.vSize.y);
						mpVideoPlayerJavaInterface->SetSubtitleColour(lwSubtitleID, 0.0f, 0.0f, 0.0f, 0.0f);
						maSubtitleMap.insert(std::pair<moFlo::Video::CSubtitles::SubtitlePtr, s64>(*it, lwSubtitleID));
					}
				}

				//update the current text views
				for (HASH_MAP<moFlo::Video::CSubtitles::SubtitlePtr, s64>::iterator it = maSubtitleMap.begin(); it != maSubtitleMap.end(); ++it)
				{
					UpdateSubtitle(it->first, it->second, mCurrentSubtitleTimeMS);
				}

				//removes any text views that are no longer needed.
				for (DYNAMIC_ARRAY<moFlo::Video::CSubtitles::SubtitlePtr>::iterator it = maSubtitlesToRemove.begin(); it != maSubtitlesToRemove.end(); ++it)
				{
					HASH_MAP<moFlo::Video::CSubtitles::SubtitlePtr, s64>::iterator mapEntry = maSubtitleMap.find(*it);
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
		void CVideoPlayerActivity::UpdateSubtitle(const moFlo::Video::CSubtitles::SubtitlePtr& inpSubtitle, s64 inlwSubtitleID, TimeIntervalMs inTimeMS)
		{
			moFlo::Video::CSubtitles::StylePtr pStyle = mpSubtitles->GetStyleWithName(inpSubtitle->strStyleName);

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
