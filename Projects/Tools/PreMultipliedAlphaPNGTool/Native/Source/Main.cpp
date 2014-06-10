/*
 *  Main.cpp
 *  PreMultipliedAlphaPNGTool
 *
 *  Created by Ian Copland on 28/03/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

#include "Main.h"
#include "ToolUtils/Logger.h"
#include "ToolUtils/StringUtils.h"
#include "PreMultipliedAlphaPNGTool.h"
using namespace ToolUtils;

//-------------------------------------------------
/// DisplayHelp
///
/// Displays the help messages.
//-------------------------------------------------
void DisplayHelp()
{
	Logger::SetLoggingLevel(Logger::LOGGING_LEVEL_VERBOSE);
	Logger::LogMessage("Usage: --input <filename> --output <filename> [--logginglevel <level>] [--errordisplay <display>] [--help]");
	Logger::LogMessage("\t--input(-i) -> The input filename.");
	Logger::LogMessage("\t--output(-o) -> The output filename.");
	Logger::LogMessage("\t--logginglevel(-l) -> The logging level. Possible values are: 'none', 'fatal', 'error', 'warning' or 'verbose'. The default value is 'warning'.");
	Logger::LogMessage("\t--errordisplay(-e) -> When error messages should be displayed. Possible values are: 'never', 'atend' or 'whenrecieved'. The default value is 'atend'.");
	Logger::LogMessage("\t--help(-h) -> Display this help message.");
}
//-------------------------------------------------
/// Main
///
/// The entry point into the application.
//-------------------------------------------------
int main(int indwArgumentCount, char* inapbyArguments[])
{
	//if no params have been passed, display the help message.
	if (indwArgumentCount == 1)
	{
		DisplayHelp();
		return 0;
	}
	
	//initialise the logger
	Logger::Initialise(indwArgumentCount, inapbyArguments);

	//collect parameters
	std::string strInputFilename;
	std::string strOutputFilename;
	for (int i = 0; i < indwArgumentCount; ++i)
	{
		std::string strArgument(inapbyArguments[i]);
		
		//input param
		if (strArgument == "-i" || strArgument == "--input")
		{
			if (i + 1 < indwArgumentCount)
				strInputFilename = inapbyArguments[i + 1];
			else
				Logger::LogFatalError("No input filename provided!", 3);
			i++;
		}
		
		//output param
		if (strArgument == "-o" || strArgument == "--output")
		{
			if (i + 1 < indwArgumentCount)
				strOutputFilename = inapbyArguments[i + 1];
			else
				Logger::LogFatalError("No output filename provided!", 4);
			i++;
		}

		//help
		if (strArgument == "-h" || strArgument == "--help")
		{
			DisplayHelp();
			return 0;
		}
	}
	
	//check that the required params have been provided.
	if (strInputFilename == "")
		Logger::LogFatalError("No input provided!", 5);
	if (strOutputFilename == "")
		Logger::LogFatalError("No output provided!", 6);
		
	//ensure input paths are in the correct format
	strInputFilename = StringUtils::StandardisePath(strInputFilename);
	strOutputFilename = StringUtils::StandardisePath(strOutputFilename);

	//check the params are valid
	if (StringUtils::EndsWith(StringUtils::ToLowerCase(strInputFilename), ".png") == false)
		Logger::LogFatalError("Input is not a png.", 7);
	if (StringUtils::EndsWith(StringUtils::ToLowerCase(strOutputFilename), ".png") == false)
		Logger::LogFatalError("Output is not a png.", 8);
		
	//Convert
	const char* cstrInputFilename = strInputFilename.c_str();
	const char* cstrOutputFilename = strOutputFilename.c_str();
	convert(cstrInputFilename, cstrOutputFilename);
	
	//output any errors or warnings reported and exit happily
	Logger::PrintErrorsAndWarnings();
	return 0;
}
//-------------------------------------------------
/// Print Fatal Error
//-------------------------------------------------
void PrintFatalError(char* inFatalError)
{
	Logger::LogFatalError(std::string(inFatalError), 9);
}
//-------------------------------------------------
/// Print Message
//-------------------------------------------------
void PrintMessage(char* inMessage)
{
	Logger::LogMessage(std::string(inMessage));
}
//-------------------------------------------------
/// Print Warning
//-------------------------------------------------
void PrintWarning(char* inWarning)
{
	Logger::LogWarning(std::string(inWarning));
}