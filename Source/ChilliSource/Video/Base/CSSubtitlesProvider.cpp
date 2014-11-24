//
//  CSSubtitlesProvider.cpp
//  Chilli Source
//  Created by Ian Copland 21/02/2013.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2013 Tag Games Limited
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

#include <ChilliSource/Video/Base/CSSubtitlesProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/Json/JsonUtils.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Video/Base/Subtitles.h>

#include <json/json.h>

namespace ChilliSource
{
	namespace Video
	{
        namespace
        {
            //----------------------------------------------------------------
            // Constants
            //----------------------------------------------------------------
            const std::string k_CSSubtitlesExtension("mosubtitles");
            const std::string k_tagVersionNumber = "VersionNumber";
            const std::string k_tagStyles = "Styles";
            const std::string k_tagSubtitles = "Subtitles";
            const std::string k_tagStyleName = "Name";
            const std::string k_tagStyleFont = "Font";
            const std::string k_tagStyleFontSize = "FontSize";
            const std::string k_tagStyleFontColour = "FontColour";
            const std::string k_tagStyleFadeTime = "FadeTime";
            const std::string k_tagStyleAlignment = "Alignment";
            const std::string k_tagStyleBounds = "Bounds";
            const std::string k_tagStyleBoundsTop = "Top";
            const std::string k_tagStyleBoundsBottom = "Bottom";
            const std::string k_tagStyleBoundsLeft = "Left";
            const std::string k_tagStyleBoundsRight = "Right";
            const std::string k_tagSubtitlestyle = "Style";
            const std::string k_tagSubtitlestartTime = "StartTime";
            const std::string k_tagSubtitleEndTime = "EndTime";
            const std::string k_tagLocalisedText = "LocalisedText";
            const std::string k_tagSubtitleTextID = "LocalisedTextID";
            const std::string k_defaultFont = "Arial";
            const std::string k_defaultColour = "1.0 1.0 1.0 1.0";
            const std::string k_defaultAlignment = "MiddleCentre";
            const u32 k_defaultFontSize = 18;
            const TimeIntervalMs k_defaultFadeTimeMS = 250;
            const f32 k_defaultTop = 0.0f;
            const f32 k_defaultLeft = 0.0f;
            const f32 k_defaultRight = 1.0f;
            const f32 k_defaultBottom = 1.0f;
        }
        
        CS_DEFINE_NAMEDTYPE(CSSubtitlesProvider);
        //-------------------------------------------------------
        //-------------------------------------------------------
        CSSubtitlesProviderUPtr CSSubtitlesProvider::Create()
        {
        	return CSSubtitlesProviderUPtr(new CSSubtitlesProvider());
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool CSSubtitlesProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSSubtitlesProvider::InterfaceID;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        Core::InterfaceIDType CSSubtitlesProvider::GetResourceType() const
        {
            return Subtitles::InterfaceID;
        }
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool CSSubtitlesProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_CSSubtitlesExtension;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		void CSSubtitlesProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
            SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(out_resource);
            LoadSubtitles(in_storageLocation, in_filePath, nullptr, pSubtitles);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		void CSSubtitlesProvider::CreateResourceFromFileAsync(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
			SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(out_resource);
			
			//Load model as task
			auto task = std::bind(&CSSubtitlesProvider::LoadSubtitles, this, in_storageLocation, in_filePath, in_delegate, pSubtitles);
			Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		void CSSubtitlesProvider::LoadSubtitles(Core::StorageLocation in_storageLocation, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const SubtitlesSPtr& out_resource) const
        {
            //read the JSON
            Json::Value root;
            if (Core::JsonUtils::ReadJson(in_storageLocation, in_filePath, root) == false)
            {
                CS_LOG_ERROR("Subtitles file '" + in_filePath + "' could not be read.");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
                    Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            //get the version number
            u32 udwVersionNumber = root.get(k_tagVersionNumber, 0).asUInt();
            if (udwVersionNumber != 1)
            {
                CS_LOG_ERROR("Subtitles file '" + in_filePath + "' has version number '" + Core::ToString(udwVersionNumber) + "'. Only version 1 is supported.");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            //get the localised text
            if (root.isMember(k_tagLocalisedText) == false)
            {
                CS_LOG_ERROR("Subtitles file '" + in_filePath + "' must have localised text resource");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            Json::Value localisedTextJson(root[k_tagLocalisedText]);
            Core::StorageLocation textLocation = Core::ParseStorageLocation(localisedTextJson.get("Location", "Package").asString());
            
            auto localisedText = Core::Application::Get()->GetResourcePool()->LoadResource<Core::LocalisedText>(textLocation, localisedTextJson.get("Path", "").asString());
            if (localisedText == nullptr)
            {
                CS_LOG_ERROR("Subtitles file '" + in_filePath + "' must have localised text resource");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            out_resource->SetLocalisedText(localisedText);
            
            //get the styles
            if (root.isMember(k_tagStyles) == true)
            {
                for (Json::ValueIterator it = root[k_tagStyles].begin(); it != root[k_tagStyles].end(); ++it)
                {
                    Subtitles::StyleCUPtr pStyle = LoadStyle((*it));
                    if (pStyle != nullptr)
                    {
                        out_resource->AddStyle(std::move(pStyle));
                    }
                    else
                    {
                        CS_LOG_ERROR("Subtitles file '" + in_filePath + "' failed to load.");
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        if(in_delegate != nullptr)
                        {
							Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                        }
                        return;
                    }
                }
            }
            else
            {
                CS_LOG_ERROR("Subtitles file '" + in_filePath + "' does not have styles.");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            //get the subtitles
            if (root.isMember(k_tagSubtitles) == true)
            {
                for (Json::ValueIterator it = root[k_tagSubtitles].begin(); it != root[k_tagSubtitles].end(); ++it)
                {
                    Subtitles::SubtitleCUPtr pSubtitle = LoadSubtitle((*it));
                    if (pSubtitle != nullptr)
                    {
                        out_resource->AddSubtitle(std::move(pSubtitle));
                    }
                    else
                    {
                        CS_LOG_ERROR("Subtitles file '" + in_filePath + "' failed to load.");
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        if(in_delegate != nullptr)
                        {
							Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                        }
                        return;
                    }
                }
            }
            else
            {
                CS_LOG_ERROR("Subtitles file '" + in_filePath + "' does not have subtitles.");
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
            
            if(in_delegate != nullptr)
            {
				Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
            }
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        Subtitles::StyleCUPtr CSSubtitlesProvider::LoadStyle(const Json::Value& in_styleJSON) const
        {
            Subtitles::StyleUPtr pStyle(new Subtitles::Style());
            
            //name
            pStyle->m_name = in_styleJSON.get(k_tagStyleName,"").asString();
            if(pStyle->m_name == "")
            {
                CS_LOG_ERROR("Subtitle style must have a name.");
                return Subtitles::StyleUPtr();
            }
            
            pStyle->m_fontName = in_styleJSON.get(k_tagStyleFont, k_defaultFont).asString();
            pStyle->m_fontSize = in_styleJSON.get(k_tagStyleFontSize, k_defaultFontSize).asUInt();
            pStyle->m_colour = Core::ParseColour(in_styleJSON.get(k_tagStyleFontColour, k_defaultColour).asString());
            pStyle->m_fadeTimeMS = ParseTime(in_styleJSON.get(k_tagStyleFadeTime, (s32)k_defaultFadeTimeMS).asString());
            pStyle->m_alignment = Rendering::ParseAlignmentAnchor(in_styleJSON.get(k_tagStyleAlignment, k_defaultAlignment).asString());
            pStyle->m_bounds = LoadBounds(in_styleJSON.get(k_tagStyleBounds, ""));
            
            return Subtitles::StyleUPtr(std::move(pStyle));
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        Subtitles::SubtitleCUPtr CSSubtitlesProvider::LoadSubtitle(const Json::Value& in_subtitleJSON) const
        {
            Subtitles::SubtitleUPtr pSubtitle(new Subtitles::Subtitle());
            
            //style
            pSubtitle->m_styleName = in_subtitleJSON.get(k_tagSubtitlestyle, "").asString();
            if (pSubtitle->m_styleName == "")
            {
            	CS_LOG_ERROR("Subtitle must have a style.");
            	return Subtitles::SubtitleCUPtr();
            }

            //text id
            pSubtitle->m_localisedTextId = in_subtitleJSON.get(k_tagSubtitleTextID, "").asString();
            if (pSubtitle->m_localisedTextId == "")
            {
                CS_LOG_ERROR("Subtitle must have a text ID.");
                return Subtitles::SubtitleCUPtr();
            }
            
            pSubtitle->m_startTimeMS = ParseTime(in_subtitleJSON.get(k_tagSubtitlestartTime, "0").asString());
            pSubtitle->m_endTimeMS = ParseTime(in_subtitleJSON.get(k_tagSubtitleEndTime, "0").asString());

            return Subtitles::SubtitleCUPtr(std::move(pSubtitle));
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        Core::Rectangle CSSubtitlesProvider::LoadBounds(const Json::Value& in_boundsJSON) const
        {
        	f32 fTop = (f32)in_boundsJSON.get(k_tagStyleBoundsTop, k_defaultTop).asDouble();
        	f32 fBottom = (f32)in_boundsJSON.get(k_tagStyleBoundsBottom, k_defaultBottom).asDouble();
        	f32 fLeft = (f32)in_boundsJSON.get(k_tagStyleBoundsLeft, k_defaultLeft).asDouble();
        	f32 fRight = (f32)in_boundsJSON.get(k_tagStyleBoundsRight, k_defaultRight).asDouble();
            
            return Core::Rectangle(CSCore::Vector2(fLeft + (fRight - fLeft) * 0.5f, fBottom + (fTop - fBottom) * 0.5f), CSCore::Vector2(fRight - fLeft, fBottom - fTop));
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        TimeIntervalMs CSSubtitlesProvider::ParseTime(const std::string& in_time) const
        {
            u32 udwHours, udwMinutes, udwSeconds, udwMilliseconds;

            CS_SSCANF(in_time.c_str(), "%d:%d:%d:%d", &udwHours, &udwMinutes, &udwSeconds, &udwMilliseconds);

            TimeIntervalMs outputMS = ((TimeIntervalMs)udwHours) * 60 * 60 * 1000 + ((TimeIntervalMs)udwMinutes) * 60 * 1000 + ((TimeIntervalMs)udwSeconds) * 1000 + ((TimeIntervalMs)udwMilliseconds);
            return outputMS;
        }
	}
}
