//
//  Logging.cpp
//  ChilliSource
//  Created by Ian Copland on 15/03/2012.
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

#include "StringUtils.h"

#include <iostream>
#include <cstdlib>
 
namespace Logging
{
	namespace
	{
		LoggingLevel g_loggingLevel = LoggingLevel::k_warning;
		int g_warningCount = 0;
		int g_errorCount = 0;
		bool g_loggingEnabled = false;

		//-------------------------------------------------
		/// Prints a message on the standard output stream.
		///
		/// @author Ian Copland
		///
		/// @param The message to print.
		//-------------------------------------------------
		void PrintStandard(std::string instrMessage)
		{
			std::cout << instrMessage << std::endl;
		}
		//-------------------------------------------------
		/// Prints a message on the error output stream.
		///
		/// @author Ian Copland
		///
		/// @param The message to print.
		//-------------------------------------------------
		void PrintError(std::string instrMessage)
		{
			std::clog << instrMessage << std::endl;
		}
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void Start(int in_argCount, char* in_arguments[])
	{
		if (g_loggingEnabled == false)
		{
			g_loggingEnabled = true;

			for (int i = 0; i < in_argCount; ++i)
			{
				std::string strArgument(in_arguments[i]);
				
				//logging level param
				if (strArgument == k_paramLoggingLevel || strArgument == k_paramLoggingLevelShort)
				{
					if (i + 1 < in_argCount)
					{
						std::string strValue(in_arguments[i+1]);
						if (strValue == k_loggingLevelNone)				
							SetLoggingLevel(LoggingLevel::k_none);
						else if (strValue == k_loggingLevelFatal)		
							SetLoggingLevel(LoggingLevel::k_fatal);
						else if (strValue == k_loggingLevelError)		
							SetLoggingLevel(LoggingLevel::k_error);
						else if (strValue == k_loggingLevelWarning)		
							SetLoggingLevel(LoggingLevel::k_warning);
						else if (strValue == k_loggingLevelVerbose)		
							SetLoggingLevel(LoggingLevel::k_verbose);
						else
							LogFatal("Invalid logging level: " + strArgument);
					}
					else
					{
						LogFatal("No logging level provided!");
					}
					i++;
				}
			}
		}
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void SetLoggingLevel(LoggingLevel in_loggingLevel)
	{
		g_loggingLevel = in_loggingLevel;
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void LogVerbose(const std::string& in_message)
	{
		if (g_loggingEnabled && ((int)g_loggingLevel) >= ((int)LoggingLevel::k_verbose))
		{
			PrintStandard(in_message);
		}
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void LogWarning(const std::string& in_message)
	{
		if (g_loggingEnabled && ((int)g_loggingLevel) >= ((int)LoggingLevel::k_warning))
		{
			++g_warningCount;
			PrintError("WARNING: " + in_message);
		}
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void LogError(const std::string& in_message)
	{
		if (g_loggingEnabled && ((int)g_loggingLevel) >= ((int)LoggingLevel::k_error))
		{
			++g_errorCount;
			PrintError("ERROR: " + in_message);
		}
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void LogFatal(const std::string& in_message)
	{
		if (g_loggingEnabled && ((int)g_loggingLevel) >= ((int)LoggingLevel::k_fatal))
		{
			++g_errorCount;
			PrintError("FATAL: " + in_message);
			Finish();
		}
		
		exit(5);
	}
	//-------------------------------------------------
	//-------------------------------------------------
	void Finish()
	{
		if (g_loggingEnabled)
		{
			if (((int)g_loggingLevel) >= ((int)LoggingLevel::k_fatal))
			{
				if (((int)g_loggingLevel) >= ((int)LoggingLevel::k_verbose) || g_warningCount != 0 || g_errorCount != 0)
				{
					PrintStandard(StringUtils::StringCast(g_warningCount) +" Warnings found.");
					PrintStandard(StringUtils::StringCast(g_errorCount) + " Errors found.");
				}
			}

			g_loggingEnabled = false;
		}
	}
}
