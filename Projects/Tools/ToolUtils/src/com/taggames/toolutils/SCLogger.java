package com.taggames.toolutils;

import java.util.LinkedList;

public class SCLogger 
{	
	//-------------------------------------------------
	/// Constants
	//-------------------------------------------------
	public static final int LOGGING_LEVEL_NONE 				= 0;
	public static final int LOGGING_LEVEL_FATAL 			= LOGGING_LEVEL_NONE + 1;
	public static final int LOGGING_LEVEL_ERROR 			= LOGGING_LEVEL_FATAL + 1;
	public static final int LOGGING_LEVEL_WARNING 			= LOGGING_LEVEL_ERROR + 1;
	public static final int LOGGING_LEVEL_VERBOSE			= LOGGING_LEVEL_WARNING + 1;
	public static final int DISPLAY_ERRORS_NEVER			= 0;
	public static final int DISPLAY_ERRORS_AT_END			= DISPLAY_ERRORS_NEVER + 1;
	public static final int DISPLAY_ERRORS_WHEN_RECIEVED	= DISPLAY_ERRORS_AT_END + 1;
	//-------------------------------------------------
	/// Static Member Data
	//-------------------------------------------------
	private static LinkedList<String> mastrErrorMessages;
	private static LinkedList<String> mastrWarningMessages;
	private static int mdwLoggingLevel;
	private static int mdwDisplayErrors;
	private static boolean mbIsError;
	//-------------------------------------------------
	/// Static Constructor
	//-------------------------------------------------
	static
	{
		mastrErrorMessages = new LinkedList<String>();
		mastrWarningMessages = new LinkedList<String>();
		mdwLoggingLevel = LOGGING_LEVEL_WARNING;
		mdwDisplayErrors = DISPLAY_ERRORS_AT_END;
		mbIsError = false;
	}
	//-------------------------------------------------
	/// Initialise
	/// 
	/// Checks the input parameters for logger params
	/// and updates the logger accordingly.
	///
	/// @param the input pararmeters.
	//-------------------------------------------------
	public static void Initialise(String[] inastrArgs)
	{
		for (int i = 0; i < inastrArgs.length; ++i)
		{
			//logging level param
			if (inastrArgs[i].equalsIgnoreCase("-l") == true || inastrArgs[i].equalsIgnoreCase("--logginglevel") == true)
			{
				
				if (i+1 < inastrArgs.length)
				{
					String strLoggingLevel = inastrArgs[i+1];
					if (strLoggingLevel.equalsIgnoreCase("none") == true) 			SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_NONE);
					else if (strLoggingLevel.equalsIgnoreCase("fatal") == true) 	SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_FATAL); 
					else if (strLoggingLevel.equalsIgnoreCase("error") == true) 	SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_ERROR); 
					else if (strLoggingLevel.equalsIgnoreCase("warning") == true) 	SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_WARNING); 
					else if (strLoggingLevel.equalsIgnoreCase("verbose") == true) 	SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_VERBOSE); 
				}
				else
				{
					SCLogger.LogFatalError("No logging level provided!");
				}
				i++;
			}
			
			//error display param
			if (inastrArgs[i].equalsIgnoreCase("-e") == true || inastrArgs[i].equalsIgnoreCase("--errordisplay") == true)
			{
				
				if (i+1 < inastrArgs.length)
				{
					String strErrorDisplay = inastrArgs[i+1];
					if (strErrorDisplay.equalsIgnoreCase("never") == true) 				SCLogger.SetWhenToDisplayErrors(SCLogger.DISPLAY_ERRORS_NEVER);
					else if (strErrorDisplay.equalsIgnoreCase("atend") == true) 		SCLogger.SetWhenToDisplayErrors(SCLogger.DISPLAY_ERRORS_AT_END); 
					else if (strErrorDisplay.equalsIgnoreCase("whenreceived") == true) 	SCLogger.SetWhenToDisplayErrors(SCLogger.DISPLAY_ERRORS_WHEN_RECIEVED); 
				}
				else
				{
					SCLogger.LogFatalError("No error display provided!");
				}
				i++;
			}
		}
	}
	//-------------------------------------------------
	/// Is Error
	///
	/// Returns if an error has occurred
	///
	/// @return Error occurred.
	//-------------------------------------------------
	public static boolean IsError()
	{
		return mbIsError;
	}
	//-------------------------------------------------
	/// Set Logging Level
	///
	/// Sets the level of message that should be logged.
	///
	/// @param the logging level.
	//-------------------------------------------------
	public static void SetLoggingLevel(int indwLoggingLevel)
	{
		mdwLoggingLevel = indwLoggingLevel;
	}
	//-------------------------------------------------
	/// Set When To Display Errors
	///
	/// Sets when errors should be shown.
	///
	/// @param integer denoting when errors should be
	///		   shown.
	//-------------------------------------------------
	public static void SetWhenToDisplayErrors(int indwDisplayErrors)
	{
		mdwDisplayErrors = indwDisplayErrors;
	}
	//-------------------------------------------------
	/// Get Logging Level
	///
	/// @return the logging level.
	//-------------------------------------------------
	public static int GetLoggingLevel()
	{
		return mdwLoggingLevel;
	}
	//-------------------------------------------------
	/// Get When To Display Errors
	///
	/// @return when errors should be shown.
	//-------------------------------------------------
	public static int GetWhenToDisplayErrors()
	{
		return mdwDisplayErrors;
	}
	//-------------------------------------------------
	/// LogMessage
	///
	/// Logs a message if logging level is set to
	///	verbose.
	///
	/// @param The message to display.
	//-------------------------------------------------
	public static void LogMessage(String instrMessage)
	{
		if (mdwLoggingLevel >= LOGGING_LEVEL_VERBOSE)
		{
			Print(instrMessage);
		}
	}
	//-------------------------------------------------
	/// Log Warning
	///
	/// This will try to print a warning or cache it
	/// for displaying later depending on the logger
	/// configuration.
	///
	/// @param the warning.
	//-------------------------------------------------
	public static void LogWarning(String instrMessage)
	{
		if (mdwLoggingLevel >= LOGGING_LEVEL_WARNING)
		{
			switch(mdwDisplayErrors)
			{
			case DISPLAY_ERRORS_AT_END:
				mastrWarningMessages.add("WARNING: " + instrMessage);
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
	///
	/// This will try to print an error or cache it
	/// for displaying later depending on the logger
	/// configuration.
	///
	/// @param the error.
	//-------------------------------------------------
	public static void LogError(String instrMessage)
	{
		if (mdwLoggingLevel >= LOGGING_LEVEL_ERROR)
		{
			switch(mdwDisplayErrors)
			{
			case DISPLAY_ERRORS_AT_END:
				mastrErrorMessages.add("ERROR: " + instrMessage);
				mbIsError = true;
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
	/// Log Fatal Error.
	///
	/// This will log a fatal error message then exit
	/// the application. 
	///
	/// @param the fatal error message to display.
	//-------------------------------------------------
	public static void LogFatalError(String instrMessage)
	{
		if (mdwLoggingLevel >= LOGGING_LEVEL_FATAL)
		{
			if (mdwDisplayErrors == DISPLAY_ERRORS_AT_END)
			{
				mastrErrorMessages.add("FATAL: " + instrMessage);
				mbIsError = true;
			}
			
			PrintErrorsAndWarnings();
			
			if (mdwDisplayErrors == DISPLAY_ERRORS_WHEN_RECIEVED)
			{
				PrintError("FATAL: " + instrMessage);
			}
		}
		
		System.exit(1);
	}
	//-------------------------------------------------
	/// Print Errors And Warnings
	/// 
	/// This will print out all cached errors and warnings
	/// if the logger has been configured to do so.
	//-------------------------------------------------
	public static void PrintErrorsAndWarnings()
	{
		if (mdwLoggingLevel >= LOGGING_LEVEL_FATAL)
		{
			if (mastrErrorMessages.size() > 0 || mastrWarningMessages.size() > 0 || mdwLoggingLevel >= LOGGING_LEVEL_VERBOSE)
			{
				Print(Integer.toString(mastrErrorMessages.size()) + " Errors found.");
				Print(Integer.toString(mastrWarningMessages.size()) + " Warnings found.");
			
				for (String strError : mastrErrorMessages)
					PrintError(strError);
				
				for (String strWarning : mastrWarningMessages)
					PrintError(strWarning);
			}
		}
	}
	//-------------------------------------------------
	/// Print
	///
	/// Outputs a line to std out.
	///
	/// @param The line to display
	//-------------------------------------------------
	private static void Print(String instrMessage)
	{
		System.out.print(instrMessage + "\n");
	}
	//-------------------------------------------------
	/// Print Error
	///
	/// Outputs a line to std error.
	///
	/// @param The error to display
	//-------------------------------------------------
	private static void PrintError(String instrMessage)
	{
		System.err.print(instrMessage + "\n");
		mbIsError = true;
	}
}

