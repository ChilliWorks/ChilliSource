//
//  Main.cpp
//  Chilli Source
//  Created by Ian Copland on 28/03/2012.
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

#include "Logging.h"
#include "PNGAlphaPremultiplier.h"
#include "StringUtils.h"

//-------------------------------------------------
/// Displays the help messages.
///
/// @author Ian Copland
//-------------------------------------------------
void DisplayHelp()
{
	Logging::SetLoggingLevel(Logging::LoggingLevel::k_verbose);
	Logging::LogVerbose("Usage: --input <filename> --output <filename> [" + Logging::k_paramLoggingLevel + " <level>] [--help]");
	Logging::LogVerbose("Parameters");
	Logging::LogVerbose(" --input(-i): The input filename.");
	Logging::LogVerbose(" --output(-o): The output filename.");
	Logging::LogVerbose(" " + Logging::k_paramLoggingLevel + "(" + Logging::k_paramLoggingLevelShort + "): The logging level.");
	Logging::LogVerbose(" --help(-h): Display this help message.");
	Logging::LogVerbose("Logging Levels:");
	Logging::LogVerbose(" " + Logging::k_loggingLevelNone + ": No logging.");
	Logging::LogVerbose(" " + Logging::k_loggingLevelFatal + ": Only log fatal errors.");
	Logging::LogVerbose(" " + Logging::k_loggingLevelError + ": Only log errors.");
	Logging::LogVerbose(" " + Logging::k_loggingLevelWarning + ": Log errors and warnings.");
	Logging::LogVerbose(" " + Logging::k_loggingLevelVerbose + ": Log all messages.");
}
//-------------------------------------------------
/// The entry point into the application.
///
/// @author Ian Copland
//-------------------------------------------------
int main(int in_argCount, char* in_arguments[])
{
	//initialise the logger
	Logging::Start(in_argCount, in_arguments);

	//if no params have been passed, display the help message.
	if (in_argCount < 2)
	{
		DisplayHelp();
		return 0;
	}

	//collect parameters
	std::string strInputFilename;
	std::string strOutputFilename;
	for (int i = 1; i < in_argCount; ++i)
	{
		std::string strArgument(in_arguments[i]);
		
		//input param
		if (strArgument == "-i" || strArgument == "--input")
		{
			if (i + 1 < in_argCount)
				strInputFilename = in_arguments[i + 1];
			else
				Logging::LogFatal("No input filename provided!");
			i++;
		}
		
		//output param
		else if (strArgument == "-o" || strArgument == "--output")
		{
			if (i + 1 < in_argCount)
				strOutputFilename = in_arguments[i + 1];
			else
				Logging::LogFatal("No output filename provided!");
			i++;
		}

		//help
		else if (strArgument == "-h" || strArgument == "--help")
		{
			DisplayHelp();
			return 0;
		}

		//skip logging level
		else if (strArgument == Logging::k_paramLoggingLevelShort || strArgument == Logging::k_paramLoggingLevel)
		{
			i++;
		}

		//failure
		else 
		{
			Logging::LogFatal("Invalid argument found: " + strArgument);
		}
	}
	
	//check that the required params have been provided.
	if (strInputFilename == "")
		Logging::LogFatal("No input provided!");
	if (strOutputFilename == "")
		Logging::LogFatal("No output provided!");
		
	//ensure input paths are in the correct format
	strInputFilename = StringUtils::StandardisePath(strInputFilename);
	strOutputFilename = StringUtils::StandardisePath(strOutputFilename);

	//check the params are valid
	if (StringUtils::EndsWith(StringUtils::ToLowerCase(strInputFilename), ".png") == false)
		Logging::LogFatal("Input is not a png.");
	if (StringUtils::EndsWith(StringUtils::ToLowerCase(strOutputFilename), ".png") == false)
		Logging::LogFatal("Output is not a png.");
		
	//Convert
	convert(strInputFilename.c_str(), strOutputFilename.c_str());
	
	//output any errors or warnings reported and exit happily
	Logging::Finish();
	return 0;
}