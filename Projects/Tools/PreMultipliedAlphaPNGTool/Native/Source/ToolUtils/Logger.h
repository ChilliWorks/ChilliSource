/*
 *  Logger.h
 *  ToolUtils
 *
 *  Created by Ian Copland on 15/03/2012.
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _TOOLUTILS_LOGGER_H_
#define _TOOLUTILS_LOGGER_H_
 
#include <vector>
#include <string>

namespace ToolUtils
{
	//============================================
	/// Logger
	///
	/// A "static class" used for logging
	/// all errors and output messages.
	//============================================
	namespace Logger
	{
		//=================================================
		/// Logging Level
		///
		/// The level of message that should be displayed
		//=================================================
		enum LOGGING_LEVEL
		{
			LOGGING_LEVEL_NONE,
			LOGGING_LEVEL_FATAL,
			LOGGING_LEVEL_ERROR,
			LOGGING_LEVEL_WARNING,
			LOGGING_LEVEL_VERBOSE
		};
		//=================================================
		/// Logging Level
		///
		/// The level of message that should be displayed
		//=================================================
		enum DISPLAY_ERRORS
		{
			DISPLAY_ERRORS_NEVER,
			DISPLAY_ERRORS_AT_END,
			DISPLAY_ERRORS_WHEN_RECIEVED
		};
		//-------------------------------------------------
		/// Initialise
		/// 
		/// Checks the input parameters for logger params
		/// and updates the logger accordingly.
		///
		/// @param the input pararmeters.
		//-------------------------------------------------
		void Initialise(int indwArgumentCount, char* inapbyArguments[]);
		//-------------------------------------------------
		/// Initialise
		///
		/// Sets the level of message that should be logged.
		///
		/// @param the logging level.
		//-------------------------------------------------
		void Initialise(LOGGING_LEVEL ineLoggingLevel);
		//-------------------------------------------------
		/// Set Logging Level
		///
		/// Sets the level of message that should be logged.
		///
		/// @param the logging level.
		//-------------------------------------------------
		void SetLoggingLevel(LOGGING_LEVEL ineLoggingLevel);
		//-------------------------------------------------
		/// Set When To Display Errors
		///
		/// Sets when errors should be shown.
		///
		/// @param when to display errors.
		//-------------------------------------------------
		void SetWhenToDisplayErrors(DISPLAY_ERRORS ineDisplayErrors);
		//-------------------------------------------------
		/// LogMessage
		///
		/// Logs a message if logging level is set to
		///	verbose.
		///
		/// @param The message to display.
		//-------------------------------------------------
		void LogMessage(std::string instrMessage);
		//-------------------------------------------------
		/// Log Warning
		///
		/// This will try to print a warning or cache it
		/// for displaying later depending on the logger
		/// configuration.
		///
		/// @param the warning.
		//-------------------------------------------------
		void LogWarning(std::string instrMessage);
		//-------------------------------------------------
		/// Log Error
		///
		/// This will try to print an error or cache it
		/// for displaying later depending on the logger
		/// configuration.
		///
		/// @param the error.
		//-------------------------------------------------
		void LogError(std::string instrMessage);
		//-------------------------------------------------
		/// Log Fatal Error.
		///
		/// This will log a fatal error message then exit
		/// the application.
		///
		/// @param the fatal error message to display.
		/// @param the error number to report.
		//-------------------------------------------------
		void LogFatalError(std::string instrMessage, int indwErrorNumber);
		//-------------------------------------------------
		/// Print Errors And Warnings
		/// 
		/// This will print out all cached errors and warnings
		/// if the logger has been configured to do so.
		//-------------------------------------------------
		void PrintErrorsAndWarnings();
	}
}
#endif