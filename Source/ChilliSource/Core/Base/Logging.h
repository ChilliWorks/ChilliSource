/*
 * File: Logging.h
 * Date: 18/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_CORE_LOGGING_H_
#define _MO_FLO_CORE_LOGGING_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
    namespace Core
    {
        class CLogging
        {
        public:
            //----------------------------------------------
            /// Init
            ///
            /// Initialise the logging system 
            //----------------------------------------------
            static void Init();
            //----------------------------------------------
            /// Log Verbose
            ///
            /// Used to output helpful but not neccessary 
            /// messages.
            /// @param Message
            /// Logging Level: VERBOSE
            //----------------------------------------------
            static void LogVerbose(const std::string &instrMessage);
            //----------------------------------------------
            /// Log Warning
            ///
            /// Used to output warning messages; that the 
            /// user may have handled (i.e. cannot find 
            /// resource provider)
            /// @param Message
            /// Logging Level: WARNING
            //----------------------------------------------
            static void LogWarning(const std::string &instrMessage);
            //----------------------------------------------
            /// Log Error
            ///
            /// Used to output error messages (i.e. trying 
            /// to create a component that the render system
            /// cannot instantiate)
            /// @param Message
            /// Logging Level: ERROR
            //----------------------------------------------
            static void LogError(const std::string &instrMessage);
            //----------------------------------------------
            /// Log Fatal
            ///
            /// Used to output fatal messages before exiting
            /// the application
            /// @param Message
            /// Logging Level: FATAL
            //----------------------------------------------
            static void LogFatal(const std::string &instrMessage);
            
    #ifdef MOFLOW_LOG_TO_FILE
            //----------------------------------------------
            /// Get Log Data
            ///
            /// Fetch all the log data as a string stream
            ///
            /// @param Out: Populated string stream
            //----------------------------------------------
            static void GetLogData(std::stringstream& outStream);
    #endif
        private:
            static bool mbInitialised;
        };
    }
}

#endif
