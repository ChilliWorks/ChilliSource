//
//  Logging.h
//  ChilliSource
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
#include <utility>

#ifdef CS_ENABLE_LOGTOFILE
#include <mutex>
#endif

namespace ChilliSource
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
        
        ///
        /// Used to output messages while the logging level is set to verbose. Declare the preprocessor
        /// macro CS_LOGLEVEL_VERBOSE to set the logging level to verbose. This is typically used for
        /// outputting messages while debugging.
        ///
        /// This particular method allows for printf style formatted output
        ///
        /// @param format
        ///     The format of the string to print e.g. "My name is %s"
        /// @param args
        ///     Variadic list of arguments to inject into the format in order
        ///
        template <typename... TArgs> void LogVerboseFormatted(char const * const format, TArgs&&... args) noexcept;
        
        ///
        /// Used to output messages while the logging level is set to verbose. Declare the preprocessor
        /// macro CS_LOGLEVEL_WARNING  or lower to set the logging level to warning. This is typically used for
        /// outputting messages while debugging.
        ///
        /// This particular method allows for printf style formatted output
        ///
        /// @param format
        ///     The format of the string to print e.g. "My name is %s"
        /// @param args
        ///     Variadic list of arguments to inject into the format in order
        ///
        template <typename... TArgs> void LogWarningFormatted(char const * const format, TArgs&&... args) noexcept;
        
        ///
        /// Used to output messages while the logging level is set to verbose. Declare the preprocessor
        /// macro CS_LOGLEVEL_ERROR or lower to set the logging level to error. This is typically used for
        /// outputting error messages.
        ///
        /// This particular method allows for printf style formatted output
        ///
        /// @param format
        ///     The format of the string to print e.g. "My name is %s"
        /// @param args
        ///     Variadic list of arguments to inject into the format in order
        ///
        template <typename... TArgs> void LogErrorFormatted(char const * const format, TArgs&&... args) noexcept;
        
        ///
        /// Used to output messages while the logging level is set to verbose. This is used for unrecoverable errors and
        /// will terminate the app.
        ///
        /// This particular method allows for printf style formatted output
        ///
        /// @param format
        ///     The format of the string to print e.g. "My name is %s"
        /// @param args
        ///     Variadic list of arguments to inject into the format in order
        ///
        template <typename... TArgs> void LogFatalFormatted(char const * const format, TArgs&&... args) noexcept;
        
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
        
        ///
        /// Method that can be overloaded to printf complex objects.
        /// The base wrapper just returns the value and works for the
        /// already supported printf values like ints and floats
        ///
        /// @param value
        ///     Already supported printf value
        ///
        /// @return Given value
        ///
        template <typename T> T ToPrintfFormat(T value) noexcept
        {
            return value;
        }
        
        ///
        /// Converts a std::string to a printf string
        ///
        /// @param value
        ///     String
        ///
        /// @return String as c-string
        ///
        template <typename T> T const* ToPrintfFormat(const std::basic_string<T>& value) noexcept
        {
            return value.c_str();
        }
        
        ///
        /// Converts printf style string to a std string ready for logging
        ///
        /// @param format
        ///     The format of the string to print e.g. "My name is %s"
        /// @param args
        ///     Variadic list of arguments to inject into the format in order
        ///
        template <typename... TArgs> std::string CreateFormattedMessage(char const * const format, TArgs&&... args) noexcept;
        
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
    
    //------------------------------------------------------------------------------
    template <typename... TArgs> std::string Logging::CreateFormattedMessage(char const * const format, TArgs&&... args) noexcept
    {
        auto bufferSize = snprintf(NULL, 0, format, ToPrintfFormat(args)...) + 1;
        char* buffer = new char[bufferSize];
#ifdef CS_TARGETPLATFORM_WINDOWS
        sprintf_s(buffer, bufferSize, format, ToPrintfFormat(args)...);
#else
        sprintf(buffer, format, ToPrintfFormat(args)...);
#endif
        
        std::string result(buffer);
        delete [] buffer;
        return result;
    }
    
    //------------------------------------------------------------------------------
    template <typename... TArgs> void Logging::LogVerboseFormatted(char const * const format, TArgs&&... args) noexcept
    {
#if defined CS_LOGLEVEL_VERBOSE
        LogVerbose(CreateFormattedMessage(format, std::forward<TArgs>(args)...));
#endif
    }
    
    //------------------------------------------------------------------------------
    template <typename... TArgs> void Logging::LogWarningFormatted(char const * const format, TArgs&&... args) noexcept
    {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING
        LogWarning(CreateFormattedMessage(format, std::forward<TArgs>(args)...));
#endif
    }
    
    //------------------------------------------------------------------------------
    template <typename... TArgs> void Logging::LogErrorFormatted(char const * const format, TArgs&&... args) noexcept
    {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING || defined CS_LOGLEVEL_ERROR
        LogError(CreateFormattedMessage(format, std::forward<TArgs>(args)...));
#endif
    }
    
    //------------------------------------------------------------------------------
    template <typename... TArgs> void Logging::LogFatalFormatted(char const * const format, TArgs&&... args) noexcept
    {
#if defined CS_LOGLEVEL_VERBOSE || defined CS_LOGLEVEL_WARNING || defined CS_LOGLEVEL_ERROR || defined CS_LOGLEVEL_FATAL
        LogFatal(CreateFormattedMessage(format, std::forward<TArgs>(args)...));
#endif
    }
}

#endif
