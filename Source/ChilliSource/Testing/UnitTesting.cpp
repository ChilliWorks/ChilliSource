//
//  UnitTesting.cpp
//  MoFlow
//
//  Created by Scott Downie on 06/02/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <moFlo/Testing/UnitTesting.h>

#include <moFlo/Core/Application.h>

namespace moFlo
{
    namespace Testing
    {  
        //=============================================================
        /// Registry
        ///
        /// Database of all the active unit tests
        //=============================================================
        CRegistry::MapIDToTestCase* CRegistry::mpMapIDToTestCase = NULL;
        DYNAMIC_ARRAY<std::string>* CRegistry::mpastrFailedTests = NULL;
        
        //-------------------------------------------------------
        /// Register 
        ///
        /// @param Test name
        /// @param Test case
        //-------------------------------------------------------
        void CRegistry::Register(const std::string& instrTestName, ITestCase* inpTest)
        {
            if(!mpMapIDToTestCase)
            {
                mpMapIDToTestCase = new CRegistry::MapIDToTestCase();
            }
            if(!mpastrFailedTests)
            {
                mpastrFailedTests = new DYNAMIC_ARRAY<std::string>;
            }
            
            mpMapIDToTestCase->insert(std::make_pair(instrTestName, inpTest));
        }
        //-------------------------------------------------------
        /// Run All
        ///
        /// Run all the registered unit tests
        //-------------------------------------------------------
        void CRegistry::RunAll()
        {
            DEBUG_LOG("");
            DEBUG_LOG("");
            DEBUG_LOG("******************> Running Unit Tests");
            
            if(!mpMapIDToTestCase)
            {
                DEBUG_LOG("******************> No Unit Tests");
                return;
            }
            
            for(MapIDToTestCase::iterator it = mpMapIDToTestCase->begin(); it != mpMapIDToTestCase->end(); ++it)
            {
                it->second->Run();
            }
            
            DEBUG_LOG("******************> Unit Tests COMPLETE: " + STRING_CAST((u32)(mpMapIDToTestCase->size() - mpastrFailedTests->size())) + "/" + STRING_CAST((u32)mpMapIDToTestCase->size()) + " successful");
            DEBUG_LOG("");
            DEBUG_LOG("");
            mpastrFailedTests->clear();
        }
        //-------------------------------------------------------
        /// Run 
        ///
        /// Run single unit test registered with given name
        //-------------------------------------------------------
        void CRegistry::Run(const std::string& instrCaseName)
        {
            DEBUG_LOG("");
            DEBUG_LOG("");
            DEBUG_LOG("******************> Running Unit Tests");
            
            if(!mpMapIDToTestCase)
            {
                DEBUG_LOG("******************> No Unit Tests");
                return;
            }
            
            MapIDToTestCase::iterator it = mpMapIDToTestCase->find(instrCaseName);
            
            if(it != mpMapIDToTestCase->end())
            {
                it->second->Run();
            }
            
            DEBUG_LOG("******************> Unit Test COMPLETE: " + STRING_CAST((u32)(1 - mpastrFailedTests->size())) + "/1 successful");
            DEBUG_LOG("");
            DEBUG_LOG("");
            
            mpastrFailedTests->clear();
        }
        //-------------------------------------------------------
        /// Record Error
        ///
        /// @param Test case name
        /// @param Failure message
        //-------------------------------------------------------
        void CRegistry::RecordError(const std::string& instrTestCaseName, const std::string& instrFailureMsg)
        {
            DEBUG_LOG("******************> Unit Test FAILED: Failure in " + instrTestCaseName + ": " + instrFailureMsg);
            
            if(std::find(mpastrFailedTests->begin(), mpastrFailedTests->end(), instrTestCaseName) == mpastrFailedTests->end())
            {
                mpastrFailedTests->push_back(instrTestCaseName);
            }
        }
        //-------------------------------------------------------
        /// Destructor 
        ///
        //-------------------------------------------------------
        CRegistry::~CRegistry()
        {
            if(mpMapIDToTestCase)
            {
                for(MapIDToTestCase::iterator it = mpMapIDToTestCase->begin(); it != mpMapIDToTestCase->end(); ++it)
                {
                    delete it->second;
                }
                
                SAFE_DELETE(mpMapIDToTestCase);
            }
            
            SAFE_DELETE(mpastrFailedTests);
        }
        
        //=============================================================
        /// Time Constraint
        ///
        /// Measures the time taken from construction to destruction
        //=============================================================
        //-------------------------------------------------------
        /// Constructor
        ///
        /// Starts timing
        ///
        /// @param Target time in MS
        //-------------------------------------------------------
        CTimeConstraint::CTimeConstraint(TimeIntervalMs inTargetTime, const std::string& instrName)
        : mTargetTime(inTargetTime), mstrTestCaseName(instrName)
        {
            mStartTime = moFlo::Core::CApplication::GetSystemTimeInMilliseconds();
        }
        //-------------------------------------------------------
        /// Destructor
        ///
        /// Ends timing
        //-------------------------------------------------------
        CTimeConstraint::~CTimeConstraint()
        {
            TimeIntervalMs TimeTaken = moFlo::Core::CApplication::GetSystemTimeInMilliseconds() - mStartTime;
            if(TimeTaken > mTargetTime)
            {
                //Taken too long
                CRegistry::RecordError(mstrTestCaseName, "Failed to complete in time of " + STRING_CAST(mTargetTime) + "ms (" + STRING_CAST(TimeTaken) + "ms)");
            }
        }
    }
}
