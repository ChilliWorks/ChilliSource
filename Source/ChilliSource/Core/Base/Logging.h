//
//  Logging.h
//  Chilli Source
//  Created by Scott Downie on 18/10/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_BASE_LOGGING_H_
#define _CHILLISOURCE_CORE_BASE_LOGGING_H_

#include <ChilliSource/ChilliSource.h>

#include <string>

#ifdef CS_ENABLE_LOGTOFILE
#include <mutex>
#endif

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
            /// @author Ian Copland
            ///
            /// @return The singleton instance of the Logger.
            //-----------------------------------------------------
            static Logging* Get();
            //-----------------------------------------------------
            /// Used to output messages while the logging
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
            /// Used to output messages while the logging level is set
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
            /// Used to output messages while the logging level is set to
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
            /// Used to output messages whle the logging level is set to
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
            /// @author Ian Copland
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
            /// @author Ian Copland
            ///
            /// @return The message to log.
            //-----------------------------------------------------
            static void Create();
            //-----------------------------------------------------
            /// Destroys the Logger.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------
            static void Destroy();
            //-----------------------------------------------------
            /// Constructor.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------
            Logging();
            //-----------------------------------------------------
            /// Logs the given message to file. How this is logged
            /// is dependant on platform.
            ///
            /// @author Ian Copland
            ///
            /// @param The logging level.
            /// @param The message to log.
            //-----------------------------------------------------
            void LogMessage(LogLevel in_logLevel, const std::string& in_message);
#ifdef CS_ENABLE_LOGTOFILE
            //-----------------------------------------------------
            /// Creates a new Log file.
            ///
            /// @author Ian Copland
            //-----------------------------------------------------
            void CreateLogFile();
            //-----------------------------------------------------
            /// Adds the message to to the log file.
            ///
            /// @author Ian Copland
            ///
            /// @param The message to log.
            //-----------------------------------------------------
            void LogToFile(const std::string& in_message);
            
            bool m_isFirstLog;
            std::string m_logBuffer;
            std::mutex m_mutex;
#endif
            static Logging* s_logging;
        };
    }
}

#endif
