/*
 *  Logger.cpp
 *  ToolUtils
 *
 *  Created by Ian Copland on 15/03/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */
 
#include "ToolUtils/Logger.h"
#include "ToolUtils/StringUtils.h"
#include <iostream>
#include <cstdlib>
 
 
namespace ToolUtils
{
	//============================================
	/// Logger
	//============================================
	namespace Logger
	{
		//-------------------------------------------------
		/// globals
		//-------------------------------------------------
		std::vector<std::string> gastrErrorMessages;
		std::vector<std::string> gastrWarningMessages;
		LOGGING_LEVEL geLoggingLevel = LOGGING_LEVEL_WARNING;
		DISPLAY_ERRORS geDisplayErrors = DISPLAY_ERRORS_AT_END;
		//-------------------------------------------------
		/// Print
		///
		/// Outputs a line to std out.
		///
		/// @param The line to display
		//-------------------------------------------------
		void Print(std::string instrMessage)
		{
			std::cout << instrMessage << std::endl;
		}
		//-------------------------------------------------
		/// Print Error
		///
		/// Outputs a line to std error.
		///
		/// @param The error to display
		//-------------------------------------------------
		void PrintError(std::string instrMessage)
		{
			std::clog << instrMessage << std::endl;
		}
		//-------------------------------------------------
		/// Initialise
		//-------------------------------------------------
		void Initialise(int indwArgumentCount, char* inapbyArguments[])
		{
			for (int i = 0; i < indwArgumentCount; ++i)
			{
				std::string strArgument(inapbyArguments[i]);
				
				//logging level param
				if (strArgument == "-l" || strArgument == "--logginglevel")
				{
					if (i + 1 < indwArgumentCount)
					{
						std::string strValue(inapbyArguments[i+1]);
						if (strValue == "none")				SetLoggingLevel(LOGGING_LEVEL_NONE);
						else if (strValue == "fatal")		SetLoggingLevel(LOGGING_LEVEL_FATAL);
						else if (strValue == "error")		SetLoggingLevel(LOGGING_LEVEL_ERROR);
						else if (strValue == "warning")		SetLoggingLevel(LOGGING_LEVEL_WARNING);
						else if (strValue == "verbose")		SetLoggingLevel(LOGGING_LEVEL_VERBOSE);
					}
					else
					{
						LogFatalError("No logging level provided!", 1);
					}
					i++;
				}
				
				//error display param
				if (strArgument == "-e" || strArgument == "--errordisplay")
				{
					if (i + 1 < indwArgumentCount)
					{
						std::string strValue(inapbyArguments[i+1]);
						if (strValue == "never")			SetWhenToDisplayErrors(DISPLAY_ERRORS_NEVER);
						else if (strValue == "atend")		SetWhenToDisplayErrors(DISPLAY_ERRORS_AT_END);
						else if (strValue == "whenrecieved")SetWhenToDisplayErrors(DISPLAY_ERRORS_WHEN_RECIEVED);
					}
					else
					{
						LogFatalError("No error display provided!", 2);
					}
					i++;
				}
			}
		}
		//-------------------------------------------------
		/// Set Logging Level
		//-------------------------------------------------
		void SetLoggingLevel(LOGGING_LEVEL ineLoggingLevel)
		{
			geLoggingLevel = ineLoggingLevel;
		}
		//-------------------------------------------------
		/// Set When To Display Errors
		//-------------------------------------------------
		void SetWhenToDisplayErrors(DISPLAY_ERRORS ineDisplayErrors)
		{
			geDisplayErrors = ineDisplayErrors;
		}
		//-------------------------------------------------
		/// LogMessage
		//-------------------------------------------------
		void LogMessage(std::string instrMessage)
		{
			if (geLoggingLevel >= LOGGING_LEVEL_VERBOSE)
			{
				Print(instrMessage);
			}
		}
		//-------------------------------------------------
		/// Log Warning
		//-------------------------------------------------
		void LogWarning(std::string instrMessage)
		{
			if (geLoggingLevel >= LOGGING_LEVEL_WARNING)
			{
				switch(geDisplayErrors)
				{
				case DISPLAY_ERRORS_AT_END:
					gastrWarningMessages.push_back("WARNING: " + instrMessage);
					break;
				case DISPLAY_ERRORS_WHEN_RECIEVED:
					PrintError("WARNING: " + instrMessage);
					break;
				case DISPLAY_ERRORS_NEVER:
				default:
					break;
				}
			}
		}
		//-------------------------------------------------
		/// Log Error
		//-------------------------------------------------
		void LogError(std::string instrMessage)
		{
			if (geLoggingLevel >= LOGGING_LEVEL_ERROR)
			{
				switch(geDisplayErrors)
				{
				case DISPLAY_ERRORS_AT_END:
					gastrErrorMessages.push_back("ERROR: " + instrMessage);
					break;
				case DISPLAY_ERRORS_WHEN_RECIEVED:
					PrintError("ERROR: " + instrMessage);
					break;
				case DISPLAY_ERRORS_NEVER:
				default:
					break;
				}
			}
		}
		//-------------------------------------------------
		/// Log Fatal Error
		//-------------------------------------------------
		void LogFatalError(std::string instrMessage, int indwErrorNumber)
		{
			if (geLoggingLevel >= LOGGING_LEVEL_FATAL)
			{
				if (geDisplayErrors == DISPLAY_ERRORS_AT_END)
				{
					gastrErrorMessages.push_back("FATAL: " + instrMessage);
				}
				
				PrintErrorsAndWarnings();
				
				if (geDisplayErrors == DISPLAY_ERRORS_WHEN_RECIEVED)
				{
					PrintError("FATAL: " + instrMessage);
				}
			}
			
			exit(indwErrorNumber);
		}
		//-------------------------------------------------
		/// Print Errors And Warnings
		//-------------------------------------------------
		void PrintErrorsAndWarnings()
		{
			if (geLoggingLevel >= LOGGING_LEVEL_FATAL)
			{
				if (geLoggingLevel >= LOGGING_LEVEL_VERBOSE || gastrErrorMessages.size() != 0 || gastrWarningMessages.size() != 0)
				{
					if (gastrErrorMessages.size() == 0)
						Print("No errors found.");
					else
						Print(StringUtils::StringCast(gastrErrorMessages.size()) + " Errors found!");
						
					if (gastrWarningMessages.size() == 0)
						Print("No warnings found.");
					else
						Print(StringUtils::StringCast(gastrWarningMessages.size()) +" Warnings found!");
				}
				
				for (std::vector<std::string>::iterator it = gastrErrorMessages.begin(); it != gastrErrorMessages.end(); ++it)
					PrintError((*it));
				
				for (std::vector<std::string>::iterator it = gastrWarningMessages.begin(); it != gastrWarningMessages.end(); ++it)
					PrintError((*it));
			}
		}
	}
}