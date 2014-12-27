//
//  Random.cpp
//  Chilli Source
//  Created by Ian Copland on 24/10/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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


#include <ChilliSource/Core/Math/Random.h>

#include <mutex>

#ifdef CS_TARGETPLATFORM_IOS
#import <Foundation/Foundation.h>
#import <CSBackend/Platform/iOS/Core/Math/RNGContainer.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
		namespace
		{
            //----------------------------------------------------------------
            /// This generates a random seed for the thread local random
            /// number generators.
            ///
            /// @author Ian Copland
            ///
            /// @return A randomly generated 32-bit integer.
            //----------------------------------------------------------------
            u32 GenerateSeed()
            {
                static std::mt19937* g_seedGenerator = nullptr;
                static std::mutex g_seedMutex;
                
                std::unique_lock<std::mutex> lock(g_seedMutex);
                
                if (g_seedGenerator == nullptr)
                {
                    g_seedGenerator = new std::mt19937(std::random_device()());
                }
                
                std::uniform_int_distribution<u32> distribution(std::numeric_limits<u32>::min(), std::numeric_limits<u32>::max());
                return distribution(*g_seedGenerator);
            }
            
#ifdef CS_TARGETPLATFORM_IOS
            //----------------------------------------------------------------
            /// iOS doesn't support C++ thread_local so we have to use the
            /// objective c version instead.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
            const char k_rngKey[] = "_threadLocalRNG";
            
#elif defined (CS_TARGETPLATFORM_WINDOWS)
            //----------------------------------------------------------------
            /// Visual C++ doesn't support thread_local yet, so the compiler
            /// specific version should be used instead. This doesn't support
			/// storage of objects that have a contructor so we are storing 
			/// the RNG as a pointer and allocating it lazily when it's first 
			/// used. To ensure that the RNG is cleaned up when the thread is 
			/// destroyed, placement new is used to store the RNG in an array.
			/// The RNG doesn't have a destructor, so this should be safe.
            ///
            /// @author Ian Copland
            //----------------------------------------------------------------
			__declspec(thread) u8 g_rngMemory[sizeof(std::mt19937)];
			__declspec(thread) std::mt19937* g_randomNumberGenerator = nullptr;
            
#else
			thread_local std::mt19937 g_randomNumberGenerator(GenerateSeed());
#endif
		}

		namespace Random
		{
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			std::mt19937& GetRandomNumberGenerator()
			{
#ifdef CS_TARGETPLATFORM_IOS
                @autoreleasepool
                {
                    NSMutableDictionary* threadLocalDict = [[NSThread currentThread] threadDictionary];
                
                    NSString* key = [NSString stringWithCString:k_rngKey encoding:NSUTF8StringEncoding];
                    RNGContainer* rngContainer = (RNGContainer*)[threadLocalDict objectForKey:key];
                    if (rngContainer == nil)
                    {
                        std::mt19937 newRandomNumberGenerator(GenerateSeed());
                        rngContainer = [[RNGContainer alloc] initWithRNG:newRandomNumberGenerator];
                        [threadLocalDict setObject:rngContainer forKey:key];
                        [rngContainer release];
                    }
                    
                    return [rngContainer rng];
                }
#elif CS_TARGETPLATFORM_WINDOWS
				if (g_randomNumberGenerator == nullptr)
				{
					g_randomNumberGenerator = new (g_rngMemory) std::mt19937(GenerateSeed());
				}

				return *g_randomNumberGenerator;
#else
				return g_randomNumberGenerator;
#endif
			}
		}
	}
}
