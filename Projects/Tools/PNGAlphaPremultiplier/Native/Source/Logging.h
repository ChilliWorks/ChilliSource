//
//  Logging.h
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

#ifndef _PNGALPHAPREMULTIPLIER_LOGGING_H_
#define _PNGALPHAPREMULTIPLIER_LOGGING_H_
 
#include <vector>
#include <string>

//------------------------------------------------------------
/// A collection of convenience methods for logging. All logging
/// must occur between calls to Start and Finish.
///
/// @author Ian Copland
//------------------------------------------------------------
namespace Logging
{
	static std::string k_paramLoggingLevel = "--logginglevel";
	static std::string k_paramLoggingLevelShort = "-ll";
	static std::string k_loggingLevelNone = "none";
	static std::string k_loggingLevelFatal = "fatal";
	static std::string k_loggingLevelError = "error";
	static std::string k_loggingLevelWarning = "warning";
	static std::string k_loggingLevelVerbose = "verbose";
	//-------------------------------------------------
	/// The level of message that should be displayed.
	///
	/// @author Ian Copland
	//-------------------------------------------------
	enum class LoggingLevel
	{
		k_none,
		k_fatal,
		k_error,
		k_warning,
		k_verbose
	};
	//-------------------------------------------------
	/// Checks the input parameters for logger params
	/// and updates the logger accordingly.
	///
	/// @author Ian Copland
	///
	/// @param the input pararmeters.
	//-------------------------------------------------
	void Start(int in_argCount, char* in_arguments[]);
	//-------------------------------------------------
	/// Sets the level of message that should be logged.
	///
	/// @author Ian Copland
	/// 
	/// @param the logging level.
	//-------------------------------------------------
	void SetLoggingLevel(LoggingLevel in_loggingLevel);
	//-------------------------------------------------
	/// Logs a message if logging level is set to
	///	verbose.
	///
	/// @author Ian Copland
	///
	/// @param The message to display.
	//-------------------------------------------------
	void LogVerbose(const std::string& in_message);
	//-------------------------------------------------
	/// Logs a warning if the logging level is set to
	/// warning or higher.
	///
	/// @author Ian Copland
	///
	/// @param the warning message.
	//-------------------------------------------------
	void LogWarning(const std::string& in_message);
	//-------------------------------------------------
	/// Logs an error if the logging level is set to
	/// error or higher.
	///
	/// @author Ian Copland
	///
	/// @param the error message.
	//-------------------------------------------------
	void LogError(const std::string& in_message);
	//-------------------------------------------------
	/// This will log a fatal error message then exit
	/// the application.
	///
	/// @author Ian Copland
	///
	/// @param the fatal error message to display.
	//-------------------------------------------------
	void LogFatal(const std::string& in_message);
	//-------------------------------------------------
	/// Finishes logging.
	///
	/// @author Ian Copland
	//-------------------------------------------------
	void Finish();
}

#endif
