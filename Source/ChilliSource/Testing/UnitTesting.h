//
//  UnitTesting.h
//  MoFlowSkeleton
//
//  Created by Scott Downie on 06/02/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef _MOFLOW_TESTING_UNIT_TESTING_H
#define _MOFLOW_TESTING_UNIT_TESTING_H

namespace moFlo
{
    namespace Testing
    {
        class ITestCase;
    }
}

//================================================================
/// Test Case 
///
/// Creates and registers a test case with the given name
///
/// @param Name
//================================================================
#define TEST_CASE(NAME)\
namespace moFlo\
{\
    namespace Testing\
    {\
        namespace Tests\
        {\
            class TestCase##NAME : public moFlo::Testing::ITestCase\
            {\
            public:\
                TestCase##NAME(const std::string& instrName) : moFlo::Testing::ITestCase(instrName){}\
                void Run() const;\
            }TestCase##NAME##Instance(#NAME);\
        }\
    }\
}\
\
void moFlo::Testing::Tests::TestCase##NAME::Run() const

//================================================================
/// Test Case Platform
///
/// Creates and registers a test case with the given name that 
/// only executes when the build target matches the given one
///
/// @param Name
/// @param Build target
//================================================================
#define TEST_CASE_PLATFORM(NAME, TARGET)
//================================================================
/// Test Check 
///
/// Checks that the expression X is true and fails otherwise
///
/// @param Expression
//================================================================
#define TEST_CHECK(ID, X)\
if((X) == 0)\
{\
    moFlo::Testing::CRegistry::RecordError(mstrName, std::string(ID) + " " + #X);\
}
//================================================================
/// Test Check Threshold
///
/// Checks that the expression X is true and once count is zero
///
/// @param Expression
//================================================================
#define TEST_CHECK_THRESHOLD(ID, X)\
if((X) == 0)\
{\
    if((++__udwFailCount) == __udwThreshold)\
    {\
        __bFailed = true;\
    }\
}\
if(__bFailed && __i == (__udwTotal - 1))\
{\
    moFlo::Testing::CRegistry::RecordError(mstrName, std::string(ID) + " Failed " + STRING_CAST(__udwFailCount) + " Attempts " + STRING_CAST(__udwTotal));\
}
//================================================================
/// Criteria Time 
///
/// Checks that the test case completes within the 
/// given time
///
/// @param Time in MS
//================================================================
#define CRITERIA_TIME(TIME)\
moFlo::Testing::CTimeConstraint __TimeConstraint__(TIME, mstrName);
//================================================================
/// Criteria Threshold 
///
/// Performs the unit test the given number of time and fails
/// if the success rate is under the threshold
///
/// @param Number of attempts
/// @param Number of successes required
//================================================================
#define CRITERIA_THRESHOLD(TOTAL, PASS)\
u32 __udwThreshold = (TOTAL - PASS) + 1;\
u32 __udwFailCount = 0;\
u32 __udwTotal = TOTAL;\
bool __bFailed = false;\
for(u32 __i=0; __i<TOTAL; ++__i)\


namespace moFlo
{
    namespace Testing
    {
        //=============================================================
        /// Time Constraint
        ///
        /// Measures the time taken from construction to destruction
        //=============================================================
        class CTimeConstraint
        {
        public:
            
            CTimeConstraint(TimeIntervalMs inTargetTime, const std::string& instrName);
            ~CTimeConstraint();
            
        private:
            
            std::string mstrTestCaseName;
            TimeIntervalMs mTargetTime;
            TimeIntervalMs mStartTime;
        };
        
        //=============================================================
        /// Registry
        ///
        /// Database of all the active unit tests
        //=============================================================
        class CRegistry
        {
        public:
            
            ~CRegistry();
            //-------------------------------------------------------
            /// Register 
            ///
            /// @param Test name
            /// @param Test case
            //-------------------------------------------------------
            static void Register(const std::string& instrTestName, ITestCase* inpTest);
            //-------------------------------------------------------
            /// Record Error
            ///
            /// @param Test case name
            /// @param Failure message
            //-------------------------------------------------------
            static void RecordError(const std::string& instrTestCaseName, const std::string& instrFailureMsg);
            //-------------------------------------------------------
            /// Run All
            ///
            /// Run all the registered unit tests
            //-------------------------------------------------------
            static void RunAll();
            //-------------------------------------------------------
            /// Run 
            ///
            /// Run single unit test registered with given name
            //-------------------------------------------------------
            static void Run(const std::string& instrCaseName);
            
        private:
            
            typedef HASH_MAP<std::string, ITestCase*> MapIDToTestCase;
            static MapIDToTestCase* mpMapIDToTestCase;
            
            static DYNAMIC_ARRAY<std::string>* mpastrFailedTests;
        };
        
        //=============================================================
        /// Test Case (Interface)
        ///
        /// A single bit of functionality to test
        //=============================================================
        class ITestCase
        {
        public:
            ITestCase(const std::string& instrName) 
            : mstrName(instrName)
            {
                CRegistry::Register(instrName, this);
            }
            virtual ~ITestCase(){}
            virtual void Run() const = 0;
            
        protected:
            
            std::string mstrName;
        };
    }
}

#endif
