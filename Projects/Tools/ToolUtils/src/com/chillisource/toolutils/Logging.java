package com.chillisource.toolutils;

import java.util.ArrayList;
import java.util.List;

/**
 * A convenience class for logging messages and errors. All logging
 * should occur between calling start() and finish().
 * 
 * @author Ian Copland
 */
public class Logging 
{	
	/**
	 * An enum describing the different logging levels.
	 * 
	 * @author Ian Copland
	 */
	public enum LoggingLevel
	{
		k_none,
		k_fatal,
		k_error,
		k_warning,
		k_verbose
	}

	public static String k_paramLoggingLevel = "--logginglevel";
	public static String k_paramLoggingLevelShort = "--ll";
	public static String k_loggingLevelNone = "none";
	public static String k_loggingLevelFatal = "fatal";
	public static String k_loggingLevelError = "error";
	public static String k_loggingLevelWarning = "warning";
	public static String k_loggingLevelVerbose = "verbose";

	private static LoggingLevel m_loggingLevel = LoggingLevel.k_warning;
	private static int m_numWarnings = 0;
	private static int m_numErrors = 0;
	private static boolean m_active = false;
	
	/**
	 * Checks the input arguments for the logging level parameter and updates
	 * accordingly.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The list of parameters into the application.
	 * 
	 * @return The parameter array with the logging params stripped out.
	 */
	public static String[] start(String[] in_args)
	{
		List<String> remainingArgs = new ArrayList<String>();
		
		if (m_active == false)
		{
			m_active = true;
			
			for (int i = 0; i < in_args.length; ++i)
			{
				if (in_args[i].equalsIgnoreCase(k_paramLoggingLevel) == true || in_args[i].equalsIgnoreCase(k_paramLoggingLevelShort) == true)
				{
					if (i+1 < in_args.length)
					{
						String strLoggingLevel = in_args[i+1];
						if (strLoggingLevel.equalsIgnoreCase(k_loggingLevelNone) == true) 			
							Logging.setLoggingLevel(LoggingLevel.k_none);
						else if (strLoggingLevel.equalsIgnoreCase(k_loggingLevelFatal) == true) 	
							Logging.setLoggingLevel(LoggingLevel.k_fatal); 
						else if (strLoggingLevel.equalsIgnoreCase(k_loggingLevelError) == true) 	
							Logging.setLoggingLevel(LoggingLevel.k_error); 
						else if (strLoggingLevel.equalsIgnoreCase(k_loggingLevelWarning) == true) 	
							Logging.setLoggingLevel(LoggingLevel.k_warning); 
						else if (strLoggingLevel.equalsIgnoreCase(k_loggingLevelVerbose) == true) 	
							Logging.setLoggingLevel(LoggingLevel.k_verbose);
						else
							Logging.logFatal("Invalid Logging Level parameter provided.");
					}
					else
					{
						Logging.logFatal("Logging level argument specified with no level provided.");
					}
					i++;
				}
				else
				{
					remainingArgs.add(in_args[i]);
				}
			}
		}
		
		return remainingArgs.toArray(new String[0]);
	}
	/**
	 * Sets the current logging level. Note that this should normally
	 * be set by the user through the --logginglevel parameter.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The logging level.
	 */
	public static void setLoggingLevel(LoggingLevel in_loggingLevel)
	{
		m_loggingLevel = in_loggingLevel;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The current logging level.
	 */
	public static LoggingLevel getLoggingLevel()
	{
		return m_loggingLevel;
	}
	/**
	 * Logs a message to the standard out stream if the logging 
	 * level is set to verbose.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The message to log.
	 */
	public static void logVerbose(String in_message)
	{
		if (m_active == true && m_loggingLevel.ordinal() >= LoggingLevel.k_verbose.ordinal())
		{
			printStandard(in_message);
		}
	}
	/**
	 * Logs a warning to the standard error stream if the logging
	 * level is set to warning or higher.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The warning to log.
	 */
	public static void logWarning(String in_message)
	{
		if (m_active == true && m_loggingLevel.ordinal() >= LoggingLevel.k_warning.ordinal())
		{
			printError("WARNING: " + in_message);
			++m_numWarnings;
		}
	}
	/**
	 * Logs an error to the standard error stream if the logging
	 * level is set to error or higher.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The error to log.
	 */
	public static void logError(String in_message)
	{
		if (m_active == true && m_loggingLevel.ordinal() >= LoggingLevel.k_error.ordinal())
		{
			printError("ERROR: " + in_message);
			++m_numErrors;
		}
	}
	/**
	 * Logs an error to the standard error stream if the logging
	 * level is set to error or higher. Also exits the application.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The error to log.
	 */
	public static void logFatal(String in_message)
	{
		if (m_active == true)
		{
			if (m_loggingLevel.ordinal() >= LoggingLevel.k_error.ordinal())
			{
				printError("FATAL: " + in_message);
				++m_numErrors;
				finish();
			}
			
			System.exit(1);
		}
	}
	/**
	 * Finishes logging. If any warnings or errors have occurred the number
	 * of them will be printed.
	 * 
	 * @author Ian Copland
	 */
	public static void finish()
	{
		if (m_active == true && m_loggingLevel.ordinal() >= LoggingLevel.k_fatal.ordinal())
		{
			if (m_numWarnings > 0 || m_numErrors > 0 || m_loggingLevel.ordinal() >= LoggingLevel.k_verbose.ordinal())
			{
				printStandard(Integer.toString(m_numWarnings) + " Errors found.");
				printStandard(Integer.toString(m_numErrors) + " Warnings found.");
			}
		}
	}
	/**
	 * Prints a single line to the standard out stream.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The line to print.
	 */
	private static void printStandard(String in_message)
	{
		System.out.print(in_message + "\n");
	}
	/**
	 * Prints a single line to the standard error stream.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The line to print.
	 */
	private static void printError(String in_message)
	{
		System.err.print(in_message + "\n");
	}
}

