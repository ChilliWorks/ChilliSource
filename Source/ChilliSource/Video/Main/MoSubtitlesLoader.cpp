/*
 *  MoSubtitlesLoader.cpp
 *  moFlow
 *
 *  Created by Ian Copland 21/02/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Video/MoSubtitlesLoader.h>
#include <ChilliSource/Video/Subtitles.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/JSON/json.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Main/Utils.h>

namespace moFlo
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
        CMoSubtitlesLoader::CMoSubtitlesLoader()
        {
        }
		//----------------------------------------------------------------
		// Is A
		//----------------------------------------------------------------
		bool CMoSubtitlesLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------
		// Can Create Resource of Kind
		//----------------------------------------------------------------
		bool CMoSubtitlesLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CSubtitles::InterfaceID);
		}
		//----------------------------------------------------------------
		// Can Create Resource From File With Extension
		//----------------------------------------------------------------
		bool CMoSubtitlesLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMoSubtitlesExtension);
		}
        //--------------------------------------------------------------
		// Destructor
		//--------------------------------------------------------------
		CMoSubtitlesLoader::~CMoSubtitlesLoader()
		{
		}
		//--------------------------------------------------------------
		// Create Resource From File
		//--------------------------------------------------------------
		bool CMoSubtitlesLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
            SubtitlesPtr pSubtitles = SHARED_PTR_CAST<CSubtitles>(outpResource);
            
            return LoadMoSubtitles(ineStorageLocation, inFilePath, pSubtitles);
		}
		//--------------------------------------------------------------
		// Async Create Resource From File
		//--------------------------------------------------------------
		bool CMoSubtitlesLoader::AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)  
		{
			SubtitlesPtr pSubtitles = SHARED_PTR_CAST<CSubtitles>(outpResource);
			
			//Load model as task
			Task3<Core::STORAGE_LOCATION, const std::string&, SubtitlesPtr&> task(this, &CMoSubtitlesLoader::LoadMoSubtitlesTask,ineStorageLocation, inFilePath, pSubtitles);
			CTaskScheduler::ScheduleTask(task);
			
			return true;
		}
        //--------------------------------------------------------------
        // Load MoSubtitles Task
        //--------------------------------------------------------------
        void CMoSubtitlesLoader::LoadMoSubtitlesTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, SubtitlesPtr& outpResource)
        {
            LoadMoSubtitles(ineStorageLocation, inFilePath, outpResource);
        }
        //--------------------------------------------------------------
        // Load MoSubtitles
        //--------------------------------------------------------------
        bool CMoSubtitlesLoader::LoadMoSubtitles(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, SubtitlesPtr& outpResource)
        {
            //read the JSON
            Json::Value root;
            moFlo::Core::CUtils::ReadJson(ineStorageLocation, inFilePath, &root);
            
            //get the version number
            u32 udwVersionNumber = root.get(kstrTagVersionNumber, 0).asUInt();
            if (udwVersionNumber != 1)
            {
                ERROR_LOG("MoSubtitles file '" + inFilePath + "' has version number '" + STRING_CAST(udwVersionNumber) + "'. Only version 1 is supported.");
                return false;
            }
            
            //get the styles
            if (root.isMember(kstrTagStyles) == true)
            {
                for (Json::ValueIterator it = root[kstrTagStyles].begin(); it != root[kstrTagStyles].end(); ++it)
                {
                    CSubtitles::StylePtr pStyle = LoadStyle((*it));
                    if (pStyle != NULL)
                    {
                        outpResource->AddStyle(pStyle);
                    }
                    else
                    {
                        ERROR_LOG("MoSubtitles file '" + inFilePath + "' failed to load.");
                        return false;
                    }
                }
            }
            else
            {
                ERROR_LOG("MoSubtitles file '" + inFilePath + "' does not have styles.");
                return false;
            }
            
            //get the subtitles
            if (root.isMember(kstrTagSubtitles) == true)
            {
                for (Json::ValueIterator it = root[kstrTagSubtitles].begin(); it != root[kstrTagSubtitles].end(); ++it)
                {
                    CSubtitles::SubtitlePtr pSubtitle = LoadSubtitle((*it));
                    if (pSubtitle != NULL)
                    {
                        outpResource->AddSubtitle(pSubtitle);
                    }
                    else
                    {
                        ERROR_LOG("MoSubtitles file '" + inFilePath + "' failed to load.");
                        return false;
                    }
                }
            }
            else
            {
                ERROR_LOG("MoSubtitles file '" + inFilePath + "' does not have subtitles.");
                return false;
            }
            
            return true;
        }
        //-------------------------------------------------------------------------
        // Load Style
        //-------------------------------------------------------------------------
        CSubtitles::StylePtr CMoSubtitlesLoader::LoadStyle(const Json::Value& inStyleJson)
        {
            CSubtitles::StylePtr pStyle(new CSubtitles::Style());
            
            //name
            pStyle->strName = inStyleJson.get(kstrTagStyleName,"").asString();
            if(pStyle->strName == "")
            {
                ERROR_LOG("Subtitle style must have a name.");
                return CSubtitles::StylePtr();
            }
            
            pStyle->strFontName = inStyleJson.get(kstrTagStyleFont, kstrDefaultFont).asString();
            pStyle->udwFontSize = inStyleJson.get(kstrTagStyleFontSize, kstrDefaultFontSize).asUInt();
            pStyle->Colour = PARSE_COLOUR(inStyleJson.get(kstrTagStyleFontColour, kstrDefaultColour).asString());
            pStyle->FadeTimeMS = ParseTime(inStyleJson.get(kstrTagStyleFadeTime, (s32)kDefaultFadeTimeMS).asString());
            pStyle->eAlignment = Core::AlignmentAnchorFromString(inStyleJson.get(kstrTagStyleAlignment, kstrDefaultAlignment).asString());
            pStyle->Bounds = LoadBounds(inStyleJson.get(kstrTagStyleBounds, ""));
            
            return pStyle;
        }
        //-------------------------------------------------------------------------
        // Load Subtitle
        //-------------------------------------------------------------------------
        CSubtitles::SubtitlePtr CMoSubtitlesLoader::LoadSubtitle(const Json::Value& inSubtitleJson)
        {
            CSubtitles::SubtitlePtr pSubtitle(new CSubtitles::Subtitle());
            
            //style
            pSubtitle->strStyleName = inSubtitleJson.get(kstrTagSubtitleStyle, "").asString();
            if (pSubtitle->strStyleName == "")
            {
            	ERROR_LOG("Subtitle must have a style.");
            	return CSubtitles::SubtitlePtr();
            }
            
            //text id
            pSubtitle->strTextID = inSubtitleJson.get(kstrTagSubtitleTextID, "").asString();
            if (pSubtitle->strTextID == "")
            {
                ERROR_LOG("Subtitle must have a text ID.");
                return CSubtitles::SubtitlePtr();
            }
            
            pSubtitle->StartTimeMS = ParseTime(inSubtitleJson.get(kstrTagSubtitleStartTime, "0").asString());
            pSubtitle->EndTimeMS = ParseTime(inSubtitleJson.get(kstrTagSubtitleEndTime, "0").asString());

            return pSubtitle;
        }
        //-------------------------------------------------------------------------
        // Load Bounds
        //-------------------------------------------------------------------------
        Core::Rectangle CMoSubtitlesLoader::LoadBounds(const Json::Value& inBoundsJson)
        {
        	f32 fTop = (f32)inBoundsJson.get(kstrTagStyleBoundsTop, kfDefaultTop).asDouble();
        	f32 fBottom = (f32)inBoundsJson.get(kstrTagStyleBoundsBottom, kfDefaultBottom).asDouble();
        	f32 fLeft = (f32)inBoundsJson.get(kstrTagStyleBoundsLeft, kfDefaultLeft).asDouble();
        	f32 fRight = (f32)inBoundsJson.get(kstrTagStyleBoundsRight, kfDefaultRight).asDouble();
            
            return Core::Rectangle(moFlo::Core::CVector2(fLeft, fTop), moFlo::Core::CVector2(fRight - fLeft, fBottom - fTop));
        }
        //-------------------------------------------------------------------------
        // Parse Time
        //-------------------------------------------------------------------------
        TimeIntervalMs CMoSubtitlesLoader::ParseTime(const std::string& instrTime)
        {
            u32 udwHours, udwMinutes, udwSeconds, udwMilliseconds;
            std::sscanf(instrTime.c_str(), "%d:%d:%d:%d", &udwHours, &udwMinutes, &udwSeconds, &udwMilliseconds);
            TimeIntervalMs outputMS = ((TimeIntervalMs)udwHours) * 60 * 60 * 1000 + ((TimeIntervalMs)udwMinutes) * 60 * 1000 + ((TimeIntervalMs)udwSeconds) * 1000 + ((TimeIntervalMs)udwMilliseconds);
            return outputMS;
        }
	}
}
