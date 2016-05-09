//
//  VideoPlayer.cpp
//  Chilli Source
//  Created by Ian Copland on 10/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Video/Base/VideoPlayer.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/File/FileSystem.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/File/TaggedFilePathResolver.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Video/Base/Subtitles.h>

extern "C"
{
	//------------------------------------------------------------------------------
	/// Called from java whenever subtitles are to be updated. This will occur every
	/// frame a video containing subtitles is running.
	///
	/// @author Ian Copland
	///
	/// @param in_env - The jni environment.
	/// @param in_this - The java object calling the function.
	//------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_video_VideoPlayer_onUpdateSubtitles(JNIEnv* in_env, jobject in_this);
	//------------------------------------------------------------------------------
    /// Called from java when the video has stopped.
    ///
    /// @author Ian Copland
    ///
    /// @param in_env - The jni environment.
    /// @param in_this - The java object calling the function.
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_video_VideoPlayer_onVideoComplete(JNIEnv* in_env, jobject in_this);
}

CSBackend::Android::VideoPlayer* g_activeVideoPlayer = nullptr;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_video_VideoPlayer_onUpdateSubtitles(JNIEnv* in_env, jobject in_this)
{
	CS_ASSERT(g_activeVideoPlayer != nullptr, "No video player active!");

	ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
	{
		g_activeVideoPlayer->OnUpdateSubtitles();
	});
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_video_VideoPlayer_onVideoComplete(JNIEnv* in_env, jobject in_this)
{
	CS_ASSERT(g_activeVideoPlayer != nullptr, "No video player active!");

	ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext&)
	{
		g_activeVideoPlayer->OnVideoComplete();
	});
}

namespace CSBackend
{
    namespace Android
    {
    	namespace
    	{
			//------------------------------------------------------------------------------
			/// Calculate the offset and size of the given size within the zip.
			///
			/// @author Ian Copland
			///
			/// @param in_storageLocation - The storage location.
			/// @param in_filePath - The path to the file in the zip.
			/// @param out_filePath - [Out] The output file path.
			/// @param out_fileOffset - [Out] The output file offset.
			/// @param out_fileLength - [Out] The output file length.
			//------------------------------------------------------------------------------
    		void CalcZippedVideoInfo(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, std::string& out_filePath, s32& out_fileOffset, s32& out_fileLength)
    		{
    			auto fileSystem = ChilliSource::Application::Get()->GetFileSystem()->Cast<FileSystem>();
    			CS_ASSERT(fileSystem != nullptr, "Could not cast to android file system.");

    			FileSystem::ZippedFileInfo zippedFileInfo;
				if (fileSystem->TryGetZippedFileInfo(in_storageLocation, in_filePath, zippedFileInfo) == false)
				{
					CS_LOG_FATAL("Couldn't get video file '" + in_filePath + "' from zip.");
				}

				CS_ASSERT(zippedFileInfo.m_size > 0, "Cannot stream a zero size video file: " + in_filePath);
				CS_ASSERT(zippedFileInfo.m_isCompressed == false && zippedFileInfo.m_size == zippedFileInfo.m_uncompressedSize, "Cannot stream video file '" + in_filePath +
					"' becuase it is compressed inside Apk or Apk expansion file.");

				out_filePath = fileSystem->GetZipFilePath();
				out_fileOffset = zippedFileInfo.m_offset;
				out_fileLength = zippedFileInfo.m_size;
    		}
    	}

    	CS_DEFINE_NAMEDTYPE(VideoPlayer);
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool VideoPlayer::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == ChilliSource::VideoPlayer::InterfaceID || in_interfaceId == VideoPlayer::InterfaceID);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        void VideoPlayer::Present(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, VideoCompleteDelegate::Connection&& in_delegateConnection, bool in_dismissWithTap,
        		const ChilliSource::Colour& in_backgroundColour)
        {
        	CS_ASSERT(g_activeVideoPlayer == nullptr, "A video player is already active!");
        	CS_ASSERT(m_isPlaying == false, "Cannot present a video while one is already playing.");

        	m_isPlaying = true;
			g_activeVideoPlayer = this;

            m_completionDelegateConnection = std::move(in_delegateConnection);

            auto fileSystem = ChilliSource::Application::Get()->GetFileSystem();
            auto taggedFilePath = ChilliSource::Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(in_storageLocation, in_filePath);

			bool inApk = false;
			std::string absFilePath = absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + taggedFilePath;
			s32 fileOffset = -1;
			s32 fileLength = -1;
            if (in_storageLocation == ChilliSource::StorageLocation::k_package || in_storageLocation == ChilliSource::StorageLocation::k_package)
            {
#if defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)
				CalcZippedVideoInfo(in_storageLocation, taggedFilePath, absFilePath, fileOffset, fileLength);
#elif defined(CS_ANDROIDFLAVOUR_AMAZON)
				inApk = true;
#else
				CS_LOG_FATAL("This Android Flavour cannot play videos from this storage location.");
#endif
            }
            else if (in_storageLocation == ChilliSource::StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(taggedFilePath) == false)
            {
#if defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)
				CalcZippedVideoInfo(in_storageLocation, taggedFilePath, absFilePath, fileOffset, fileLength);
#elif defined(CS_ANDROIDFLAVOUR_AMAZON)
				inApk = true;
#else
				CS_LOG_FATAL("This Android Flavour cannot play videos from this storage location.");
#endif
            }

			jstring jAbsFilePath = JavaUtils::CreateJStringFromSTDString(absFilePath);

			m_javaSystem->CallVoidMethod("present", inApk, jAbsFilePath, fileOffset, fileLength, in_dismissWithTap, (m_subtitles != nullptr), in_backgroundColour.r, in_backgroundColour.g,
			in_backgroundColour.b, in_backgroundColour.a);

			JavaUtils::DeleteLocalRef(jAbsFilePath);
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void VideoPlayer::PresentWithSubtitles(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::SubtitlesCSPtr& in_subtitles, VideoCompleteDelegate::Connection&& in_delegateConnection,
                bool in_dismissWithTap, const ChilliSource::Colour& in_backgroundColour)
		{
			m_subtitles = in_subtitles;
			Present(in_storageLocation, in_filePath, std::move(in_delegateConnection), in_dismissWithTap, in_backgroundColour);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        void VideoPlayer::OnInit()
        {
			JavaClassDef classDef("com/chilliworks/chillisource/video/VideoPlayer");
			classDef.AddMethod("present", "(ZLjava/lang/String;IIZZFFFF)V");
			classDef.AddMethod("getPlaybackPosition", "()F");
			classDef.AddMethod("createSubtitle", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;FFFF)J");
			classDef.AddMethod("setSubtitleColour", "(JFFFF)V");
			classDef.AddMethod("removeSubtitle", "(J)V");

        	m_javaSystem = JavaSystemUPtr(new JavaSystem(classDef));
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        void VideoPlayer::OnVideoComplete()
        {
        	m_subtitles.reset();
        	m_isPlaying = false;
			g_activeVideoPlayer = nullptr;

            if (m_completionDelegateConnection != nullptr)
            {
                auto delegateConnection = std::move(m_completionDelegateConnection);
                m_completionDelegateConnection = nullptr;
                delegateConnection->Call();
            }
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void VideoPlayer::OnUpdateSubtitles()
		{
			//only update if the position in the video has changed.
			f32 position = m_javaSystem->CallFloatMethod("getPlaybackPosition");
			TimeIntervalMs currentTimeMS = (TimeIntervalMs)(position * 1000.0f);
			if (m_currentSubtitleTimeMS != currentTimeMS)
			{
				m_currentSubtitleTimeMS = currentTimeMS;

				//get the current subtitles
				auto subtitleArray = m_subtitles->GetSubtitlesAtTime(m_currentSubtitleTimeMS);
				auto localisedText = m_subtitles->GetLocalisedText().get();

				//add any new subtitles
				for (auto it = subtitleArray.begin(); it != subtitleArray.end(); ++it)
				{
					auto mapEntry = m_subtitleMap.find(*it);
					if (mapEntry == m_subtitleMap.end())
					{
						const std::string& text = localisedText->GetText((*it)->m_localisedTextId);
						const ChilliSource::Subtitles::Style* style = m_subtitles->GetStyleWithName((*it)->m_styleName);
						auto alignment = ChilliSource::StringFromAlignmentAnchor(style->m_alignment);

						jstring jText = JavaUtils::CreateJStringFromSTDString(text);
						jstring jFontName = JavaUtils::CreateJStringFromSTDString(style->m_fontName);
						jstring jAlignment = JavaUtils::CreateJStringFromSTDString(alignment);

						s64 subtitleID = m_javaSystem->CallLongMethod("createSubtitle", jText, jFontName, style->m_fontSize, jAlignment, style->m_bounds.vOrigin.x, style->m_bounds.vOrigin.y, style->m_bounds.vSize.x, style->m_bounds.vSize.y);
						m_javaSystem->CallVoidMethod("setSubtitleColour", subtitleID, 0.0f, 0.0f, 0.0f, 0.0f);

						JavaUtils::DeleteLocalRef(jAlignment);
						JavaUtils::DeleteLocalRef(jFontName);
						JavaUtils::DeleteLocalRef(jText);

						m_subtitleMap.insert(std::make_pair(*it, subtitleID));
					}
				}

				//update the current text views
				for (auto it = m_subtitleMap.begin(); it != m_subtitleMap.end(); ++it)
				{
					UpdateSubtitle(it->first, it->second, m_currentSubtitleTimeMS);
				}

				//removes any text views that are no longer needed.
				for (auto it = m_subtitlesToRemove.begin(); it != m_subtitlesToRemove.end(); ++it)
				{
					auto mapEntry = m_subtitleMap.find(*it);
					if (mapEntry != m_subtitleMap.end())
					{
						m_javaSystem->CallVoidMethod("removeSubtitle", mapEntry->second);
						m_subtitleMap.erase(mapEntry);
					}
				}
				m_subtitlesToRemove.clear();
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void VideoPlayer::UpdateSubtitle(const ChilliSource::Subtitles::Subtitle* in_subtitle, s64 in_subtitleID, TimeIntervalMs in_timeMS)
		{
			const ChilliSource::Subtitles::Style* style = m_subtitles->GetStyleWithName(in_subtitle->m_styleName);

			f32 fade = 0.0f;
			s64 relativeTime = ((s64)in_timeMS) - ((s64)in_subtitle->m_startTimeMS);
			s64 displayTime = ((s64)in_subtitle->m_endTimeMS) - ((s64)in_subtitle->m_startTimeMS);

			//subtitle should not be displayed yet so remove
			if (relativeTime < 0)
			{
				m_subtitlesToRemove.push_back(in_subtitle);
			}

			//fading in
			else if (relativeTime < style->m_fadeTimeMS)
			{
				fade = ((f32)relativeTime) / ((f32)style->m_fadeTimeMS);
			}

			//active
			else if (relativeTime < displayTime - style->m_fadeTimeMS)
			{
				fade = 1.0f;
			}

			//fading out
			else if (relativeTime < displayTime)
			{
				fade = 1.0f - (((f32)relativeTime - (displayTime - style->m_fadeTimeMS)) / ((f32)style->m_fadeTimeMS));
			}

			//should no longer be displayed so remove
			else if (relativeTime >= displayTime)
			{
				m_subtitlesToRemove.push_back(in_subtitle);
			}

			m_javaSystem->CallVoidMethod("setSubtitleColour", in_subtitleID, style->m_colour.r, style->m_colour.g, style->m_colour.b, fade * style->m_colour.a);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
        void VideoPlayer::OnDestroy()
        {
        	m_javaSystem.reset();
        }
    }
}

#endif
