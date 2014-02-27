/*
 *  MoSubtitlesLoader.cpp
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Video/Base/MoSubtitlesLoader.h>

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
        //----------------------------------------------------------------
        // Constants
        //----------------------------------------------------------------
		const std::string kstrMoSubtitlesExtension("mosubtitles");
		const std::string kstrTagVersionNumber = "VersionNumber";
        const std::string kstrTagStyles = "Styles";
        const std::string kstrTagSubtitles = "Subtitles";
        const std::string kstrTagStyleName = "Name";
        const std::string kstrTagStyleFont = "Font";
        const std::string kstrTagStyleFontSize = "FontSize";
        const std::string kstrTagStyleFontColour = "FontColour";
        const std::string kstrTagStyleFadeTime = "FadeTime";
        const std::string kstrTagStyleAlignment = "Alignment";
        const std::string kstrTagStyleBounds = "Bounds";
        const std::string kstrTagStyleBoundsTop = "Top";
        const std::string kstrTagStyleBoundsBottom = "Bottom";
        const std::string kstrTagStyleBoundsLeft = "Left";
        const std::string kstrTagStyleBoundsRight = "Right";
        const std::string kstrTagSubtitleStyle = "Style";
        const std::string kstrTagSubtitleStartTime = "StartTime";
        const std::string kstrTagSubtitleEndTime = "EndTime";
        const std::string kstrTagSubtitleTextID = "TextID";
        const std::string kstrDefaultFont = "Arial";
        const std::string kstrDefaultColour = "1.0 1.0 1.0 1.0";
        const std::string kstrDefaultAlignment = "MiddleCentre";
        const u32 kstrDefaultFontSize = 18;
        const TimeIntervalMs kDefaultFadeTimeMS = 250;
        const f32 kfDefaultTop = 0.0f;
        const f32 kfDefaultLeft = 0.0f;
        const f32 kfDefaultRight = 1.0f;
        const f32 kfDefaultBottom = 1.0f;
        //----------------------------------------------------------------
        // Constructor
        //----------------------------------------------------------------
        MoSubtitlesLoader::MoSubtitlesLoader()
        {
        }
		//----------------------------------------------------------------
		// Is A
		//----------------------------------------------------------------
		bool MoSubtitlesLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		// Can Create Resource of Kind
		//----------------------------------------------------------------
		bool MoSubtitlesLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Subtitles::InterfaceID);
		}
		//----------------------------------------------------------------
		// Can Create Resource From File With Extension
		//----------------------------------------------------------------
		bool MoSubtitlesLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMoSubtitlesExtension);
		}
        //--------------------------------------------------------------
		// Destructor
		//--------------------------------------------------------------
		MoSubtitlesLoader::~MoSubtitlesLoader()
		{
		}
		//--------------------------------------------------------------
		// Create Resource From File
		//--------------------------------------------------------------
		bool MoSubtitlesLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
		{
            SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(outpResource);
            
            return LoadMoSubtitles(ineStorageLocation, inFilePath, pSubtitles);
		}
		//--------------------------------------------------------------
		// Async Create Resource From File
		//--------------------------------------------------------------
		bool MoSubtitlesLoader::AsyncCreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)  
		{
			SubtitlesSPtr pSubtitles = std::static_pointer_cast<Subtitles>(outpResource);
			
			//Load model as task
            Core::Task<Core::StorageLocation, const std::string&, SubtitlesSPtr&> task(this, &MoSubtitlesLoader::LoadMoSubtitlesTask,ineStorageLocation, inFilePath, pSubtitles);
			Core::TaskScheduler::ScheduleTask(task);
			
			return true;
		}
        //--------------------------------------------------------------
        // Load MoSubtitles Task
        //--------------------------------------------------------------
        void MoSubtitlesLoader::LoadMoSubtitlesTask(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, SubtitlesSPtr& outpResource)
        {
            LoadMoSubtitles(ineStorageLocation, inFilePath, outpResource);
        }
        //--------------------------------------------------------------
        // Load MoSubtitles
        //--------------------------------------------------------------
        bool MoSubtitlesLoader::LoadMoSubtitles(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, SubtitlesSPtr& outpResource)
        {
            //read the JSON
            Json::Value root;
            ChilliSource::Core::Utils::ReadJson(ineStorageLocation, inFilePath, &root);
            
            //get the version number
            u32 udwVersionNumber = root.get(kstrTagVersionNumber, 0).asUInt();
            if (udwVersionNumber != 1)
            {
                CS_ERROR_LOG("MoSubtitles file '" + inFilePath + "' has version number '" + Core::ToString(udwVersionNumber) + "'. Only version 1 is supported.");
                return false;
            }
            
            //get the styles
            if (root.isMember(kstrTagStyles) == true)
            {
                for (Json::ValueIterator it = root[kstrTagStyles].begin(); it != root[kstrTagStyles].end(); ++it)
                {
                    Subtitles::StylePtr pStyle = LoadStyle((*it));
                    if (pStyle != nullptr)
                    {
                        outpResource->AddStyle(pStyle);
                    }
                    else
                    {
                        CS_ERROR_LOG("MoSubtitles file '" + inFilePath + "' failed to load.");
                        return false;
                    }
                }
            }
            else
            {
                CS_ERROR_LOG("MoSubtitles file '" + inFilePath + "' does not have styles.");
                return false;
            }
            
            //get the subtitles
            if (root.isMember(kstrTagSubtitles) == true)
            {
                for (Json::ValueIterator it = root[kstrTagSubtitles].begin(); it != root[kstrTagSubtitles].end(); ++it)
                {
                    Subtitles::SubtitlePtr pSubtitle = LoadSubtitle((*it));
                    if (pSubtitle != nullptr)
                    {
                        outpResource->AddSubtitle(pSubtitle);
                    }
                    else
                    {
                        CS_ERROR_LOG("MoSubtitles file '" + inFilePath + "' failed to load.");
                        return false;
                    }
                }
            }
            else
            {
                CS_ERROR_LOG("MoSubtitles file '" + inFilePath + "' does not have subtitles.");
                return false;
            }
            
            return true;
        }
        //-------------------------------------------------------------------------
        // Load Style
        //-------------------------------------------------------------------------
        Subtitles::StylePtr MoSubtitlesLoader::LoadStyle(const Json::Value& inStyleJson)
        {
            Subtitles::StylePtr pStyle(new Subtitles::Style());
            
            //name
            pStyle->strName = inStyleJson.get(kstrTagStyleName,"").asString();
            if(pStyle->strName == "")
            {
                CS_ERROR_LOG("Subtitle style must have a name.");
                return Subtitles::StylePtr();
            }
            
            pStyle->strFontName = inStyleJson.get(kstrTagStyleFont, kstrDefaultFont).asString();
            pStyle->udwFontSize = inStyleJson.get(kstrTagStyleFontSize, kstrDefaultFontSize).asUInt();
            pStyle->Colour = Core::ParseColour(inStyleJson.get(kstrTagStyleFontColour, kstrDefaultColour).asString());
            pStyle->FadeTimeMS = ParseTime(inStyleJson.get(kstrTagStyleFadeTime, (s32)kDefaultFadeTimeMS).asString());
            pStyle->eAlignment = Rendering::AlignmentAnchorFromString(inStyleJson.get(kstrTagStyleAlignment, kstrDefaultAlignment).asString());
            pStyle->Bounds = LoadBounds(inStyleJson.get(kstrTagStyleBounds, ""));
            
            return pStyle;
        }
        //-------------------------------------------------------------------------
        // Load Subtitle
        //-------------------------------------------------------------------------
        Subtitles::SubtitlePtr MoSubtitlesLoader::LoadSubtitle(const Json::Value& inSubtitleJson)
        {
            Subtitles::SubtitlePtr pSubtitle(new Subtitles::Subtitle());
            
            //style
            pSubtitle->strStyleName = inSubtitleJson.get(kstrTagSubtitleStyle, "").asString();
            if (pSubtitle->strStyleName == "")
            {
            	CS_ERROR_LOG("Subtitle must have a style.");
            	return Subtitles::SubtitlePtr();
            }
            
            //text id
            pSubtitle->strTextID = inSubtitleJson.get(kstrTagSubtitleTextID, "").asString();
            if (pSubtitle->strTextID == "")
            {
                CS_ERROR_LOG("Subtitle must have a text ID.");
                return Subtitles::SubtitlePtr();
            }
            
            pSubtitle->StartTimeMS = ParseTime(inSubtitleJson.get(kstrTagSubtitleStartTime, "0").asString());
            pSubtitle->EndTimeMS = ParseTime(inSubtitleJson.get(kstrTagSubtitleEndTime, "0").asString());

            return pSubtitle;
        }
        //-------------------------------------------------------------------------
        // Load Bounds
        //-------------------------------------------------------------------------
        Core::Rectangle MoSubtitlesLoader::LoadBounds(const Json::Value& inBoundsJson)
        {
        	f32 fTop = (f32)inBoundsJson.get(kstrTagStyleBoundsTop, kfDefaultTop).asDouble();
        	f32 fBottom = (f32)inBoundsJson.get(kstrTagStyleBoundsBottom, kfDefaultBottom).asDouble();
        	f32 fLeft = (f32)inBoundsJson.get(kstrTagStyleBoundsLeft, kfDefaultLeft).asDouble();
        	f32 fRight = (f32)inBoundsJson.get(kstrTagStyleBoundsRight, kfDefaultRight).asDouble();
            
            return Core::Rectangle(ChilliSource::Core::Vector2(fLeft, fTop), ChilliSource::Core::Vector2(fRight - fLeft, fBottom - fTop));
        }
        //-------------------------------------------------------------------------
        // Parse Time
        //-------------------------------------------------------------------------
        TimeIntervalMs MoSubtitlesLoader::ParseTime(const std::string& instrTime)
        {
            u32 udwHours, udwMinutes, udwSeconds, udwMilliseconds;
            std::sscanf(instrTime.c_str(), "%d:%d:%d:%d", &udwHours, &udwMinutes, &udwSeconds, &udwMilliseconds);
            TimeIntervalMs outputMS = ((TimeIntervalMs)udwHours) * 60 * 60 * 1000 + ((TimeIntervalMs)udwMinutes) * 60 * 1000 + ((TimeIntervalMs)udwSeconds) * 1000 + ((TimeIntervalMs)udwMilliseconds);
            return outputMS;
        }
	}
}
