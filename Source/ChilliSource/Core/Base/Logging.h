//
//  Logging.h
//  Chilli Source
//
//  Created by S Downie on 18/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_CORE_BASE_LOGGING_H_
#define _CHILLISOURCE_CORE_BASE_LOGGING_H_

#include <ChilliSource/ChilliSource.h>

#include <string>

namespace ChilliSource
{
    namespace Core
    {
        //------------------------------------------------------------
        /// A cross platform logging system. This provides logging
        /// of debug, warning, error and fatal error messages. This
        /// implements the singleton pattern but does not inherit
        /// from singleton. This is because singleton uses Logging.
        ///
        /// @author S Downie
        //------------------------------------------------------------
        class Logging final
        {
        public:
            CS_DECLARE_NOCOPY(Logging);
            //-----------------------------------------------------
            /// @author I Copland
            ///
            /// @return The singleton instance of the Logger.
            //-----------------------------------------------------
            static Logging* Get();
            //-----------------------------------------------------
            /// Outputs messages while in debug mode the logging
            /// level is set to verbose. Declare the preprocessor
            /// macro CS_LOGLEVEL_VERBOSE to set the logging
            /// level to verbose. This is typically used for
            /// outputting messages while debugging.
            ///
            /// @author S Downie
            ///
            /// @param The message to log.
            //-----------------------------------------------------
            void LogVerbose(const std::string& in_message);
            //-----------------------------------------------------
            /// Outputs messages while the logging level is set
            /// to warning or higher. Declare the preprocessor
            /// macro CS_LOGLEVEL_WARNING to set the logging
            /// level to warning. This is typically used to warn of
            /// potential error states that the user can handle.
            ///
            /// @author S Downie
            ///
            /// @param The message to log.
            //-----------------------------------------------------
            void LogWarning(const std::string& in_message);
            //-----------------------------------------------------
            /// Outputs messages while the logging level is set to
            /// error or higher. Declare the preprocessor macro
            /// CS_LOGLEVEL_ERROR to set the logging level to
            /// error. This is typically used to indicate that
            /// the system has not been able to complete a process
            /// due to entering an error state, but that the
            /// application can recover from.
            ///
            /// @author S Downie
            ///
            /// @param The message to log.
            //-----------------------------------------------------
            void LogError(const std::string& in_message);
            //-----------------------------------------------------
            /// Outputs messages whle the logging level is set to
            /// fatal or higher and ends the application. Declare the
            /// preprocessor macro CS_LOGLEVEL_FATAL to set the
            /// logging level to fatal. This is typically used to
            /// inform the user that the application has entered into
            /// an irrecoverable error state.
            ///
            /// @author S Downie
            ///
            /// @param The message to log.
            //-----------------------------------------------------
            void LogFatal(const std::string& in_message);
        private:
            friend class Application;
            
            //-----------------------------------------------------
            /// An enum describing the various logging levels.
            ///
            /// @author I Copland
            //-----------------------------------------------------
            enum class LogLevel
            {
                k_verbose,
                k_warning,
                k_error
            };
            //-----------------------------------------------------
            /// Creates the singleton instance of the Logger.
            ///
            /// @author I Copland
            ///
            /// @return The message to log.
            //-----------------------------------------------------
            static void Create();
            //-----------------------------------------------------
            /// Destroys the Logger.
            ///
            /// @author I Copland
            //-----------------------------------------------------
            static void Destroy();
            //-----------------------------------------------------
            /// Constructor.
            ///
            /// @author I Copland
            //-----------------------------------------------------
            Logging();
            //-----------------------------------------------------
            /// Logs the given message to file. How this is logged
            /// is dependant on platform.
            ///
            /// @author I Copland
            ///
            /// @param The logging level.
            /// @param The message to log.
            //-----------------------------------------------------
            void LogMessage(LogLevel in_logLevel, const std::string& in_message);
#ifdef CS_ENABLE_LOGTOFILE
            //-----------------------------------------------------
            /// Creates a new Log file.
            ///
            /// @author I Copland
            //-----------------------------------------------------
            void CreateLogFile();
            //-----------------------------------------------------
            /// Adds the message to to the log file.
            ///
            /// @author I Copland
            ///
            /// @param The message to log.
            //-----------------------------------------------------
            void LogToFile(const std::string& in_message);
            //-----------------------------------------------------
            /// Write the contents of the log buffer to file.
            ///
            /// @author S Downie
            //-----------------------------------------------------
            void FlushBuffer();
            
            std::string m_logBuffer;
#endif
            static Logging* s_logging;
        };
    }
}

#endif
