//
//  CSSubtitlesProvider.cpp
//  Chilli Source
//
//  Created by Ian Copland 21/02/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

#include <ChilliSource/Video/Base/CSSubtitlesProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Video/Base/Subtitles.h>

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
            const std::string k_tagSubtitleTextID = "TextID";
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

		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool CSSubtitlesProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProviderOld::InterfaceID || in_interfaceId == CSSubtitlesProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool CSSubtitlesProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == Subtitles::InterfaceID);
		}
		//----------------------------------------------------------------
		//----------------------------------------------------------------
		bool CSSubtitlesProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
		{
			return (in_extension == k_CSSubtitlesExtension);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool CSSubtitlesProvider::CreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
		{
            SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(out_resource);
            
            LoadSubtitles(in_storageLocation, in_filePath, pSubtitles);
            
            return out_resource->IsLoaded();
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool CSSubtitlesProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
		{
			SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(out_resource);
			
			//Load model as task
            Core::Task<Core::StorageLocation, const std::string&, SubtitlesSPtr&> task(this, &CSSubtitlesProvider::LoadSubtitles, in_storageLocation, in_filePath, pSubtitles);
			Core::TaskScheduler::ScheduleTask(task);
			
			return true;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void CSSubtitlesProvider::LoadSubtitles(Core::StorageLocation in_storageLocation, const std::string& in_filePath, SubtitlesSPtr& out_resource) const
        {
            //read the JSON
            Json::Value root;
            ChilliSource::Core::Utils::ReadJson(in_storageLocation, in_filePath, &root);
            
            //get the version number
            u32 udwVersionNumber = root.get(k_tagVersionNumber, 0).asUInt();
            if (udwVersionNumber != 1)
            {
                CS_LOG_ERROR("MoSubtitles file '" + in_filePath + "' has version number '" + Core::ToString(udwVersionNumber) + "'. Only version 1 is supported.");
                out_resource->SetLoaded(false);
                return;
            }
            
            //get the styles
            if (root.isMember(k_tagStyles) == true)
            {
                for (Json::ValueIterator it = root[k_tagStyles].begin(); it != root[k_tagStyles].end(); ++it)
                {
                    Subtitles::StylePtr pStyle = LoadStyle((*it));
                    if (pStyle != nullptr)
                    {
                        out_resource->AddStyle(pStyle);
                    }
                    else
                    {
                        CS_LOG_ERROR("MoSubtitles file '" + in_filePath + "' failed to load.");
                        out_resource->SetLoaded(false);
                        return;
                    }
                }
            }
            else
            {
                CS_LOG_ERROR("MoSubtitles file '" + in_filePath + "' does not have styles.");
                out_resource->SetLoaded(false);
                return;
            }
            
            //get the subtitles
            if (root.isMember(k_tagSubtitles) == true)
            {
                for (Json::ValueIterator it = root[k_tagSubtitles].begin(); it != root[k_tagSubtitles].end(); ++it)
                {
                    Subtitles::SubtitlePtr pSubtitle = LoadSubtitle((*it));
                    if (pSubtitle != nullptr)
                    {
                        out_resource->AddSubtitle(pSubtitle);
                    }
                    else
                    {
                        CS_LOG_ERROR("MoSubtitles file '" + in_filePath + "' failed to load.");
                        out_resource->SetLoaded(false);
                        return;
                    }
                }
            }
            else
            {
                CS_LOG_ERROR("MoSubtitles file '" + in_filePath + "' does not have subtitles.");
                out_resource->SetLoaded(false);
                return;
            }
            
            out_resource->SetLoaded(true);
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        Subtitles::StylePtr CSSubtitlesProvider::LoadStyle(const Json::Value& in_styleJSON) const
        {
            Subtitles::StylePtr pStyle(new Subtitles::Style());
            
            //name
            pStyle->strName = in_styleJSON.get(k_tagStyleName,"").asString();
            if(pStyle->strName == "")
            {
                CS_LOG_ERROR("Subtitle style must have a name.");
                return Subtitles::StylePtr();
            }
            
            pStyle->strFontName = in_styleJSON.get(k_tagStyleFont, k_defaultFont).asString();
            pStyle->udwFontSize = in_styleJSON.get(k_tagStyleFontSize, k_defaultFontSize).asUInt();
            pStyle->Colour = Core::ParseColour(in_styleJSON.get(k_tagStyleFontColour, k_defaultColour).asString());
            pStyle->FadeTimeMS = ParseTime(in_styleJSON.get(k_tagStyleFadeTime, (s32)k_defaultFadeTimeMS).asString());
            pStyle->eAlignment = Rendering::AlignmentAnchorFromString(in_styleJSON.get(k_tagStyleAlignment, k_defaultAlignment).asString());
            pStyle->Bounds = LoadBounds(in_styleJSON.get(k_tagStyleBounds, ""));
            
            return pStyle;
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        Subtitles::SubtitlePtr CSSubtitlesProvider::LoadSubtitle(const Json::Value& in_subtitleJSON) const
        {
            Subtitles::SubtitlePtr pSubtitle(new Subtitles::Subtitle());
            
            //style
            pSubtitle->strStyleName = in_subtitleJSON.get(k_tagSubtitlestyle, "").asString();
            if (pSubtitle->strStyleName == "")
            {
            	CS_LOG_ERROR("Subtitle must have a style.");
            	return Subtitles::SubtitlePtr();
            }
            
            //text id
            pSubtitle->strTextID = in_subtitleJSON.get(k_tagSubtitleTextID, "").asString();
            if (pSubtitle->strTextID == "")
            {
                CS_LOG_ERROR("Subtitle must have a text ID.");
                return Subtitles::SubtitlePtr();
            }
            
            pSubtitle->StartTimeMS = ParseTime(in_subtitleJSON.get(k_tagSubtitlestartTime, "0").asString());
            pSubtitle->EndTimeMS = ParseTime(in_subtitleJSON.get(k_tagSubtitleEndTime, "0").asString());

            return pSubtitle;
        }
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        Core::Rectangle CSSubtitlesProvider::LoadBounds(const Json::Value& in_boundsJSON) const
        {
        	f32 fTop = (f32)in_boundsJSON.get(k_tagStyleBoundsTop, k_defaultTop).asDouble();
        	f32 fBottom = (f32)in_boundsJSON.get(k_tagStyleBoundsBottom, k_defaultBottom).asDouble();
        	f32 fLeft = (f32)in_boundsJSON.get(k_tagStyleBoundsLeft, k_defaultLeft).asDouble();
        	f32 fRight = (f32)in_boundsJSON.get(k_tagStyleBoundsRight, k_defaultRight).asDouble();
            
            return Core::Rectangle(ChilliSource::Core::Vector2(fLeft, fTop), ChilliSource::Core::Vector2(fRight - fLeft, fBottom - fTop));
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
