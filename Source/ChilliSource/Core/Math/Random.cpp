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

//----------------------------------------------------------------
/// Visual studio doesn't support thread_local yet, so the compiler
/// specific version should be used instead.
///
/// @author Ian Copland
//----------------------------------------------------------------
#if defined (CS_TARGETPLATFORM_WINDOWS)
#define thread_local __declspec( thread )
#endif

namespace ChilliSource
{
	namespace Core
	{
		namespace
		{
			thread_local std::mt19937* g_randomNumberGenerator = nullptr;
			std::mt19937* g_seedGenerator = nullptr;
			std::mutex g_seedMutex;

			//----------------------------------------------------------------
			/// This generates a random seed for the thread local random
			/// number generators.
			///
			/// @author Ian Copland
			///
			/// @return A randomly generated 64-bit integer.
			//----------------------------------------------------------------
			u32 GenerateSeed()
			{
				std::unique_lock<std::mutex> lock(g_seedMutex);

				if (g_seedGenerator == nullptr)
				{
					g_seedGenerator = new std::mt19937(std::random_device()());
				}

				std::uniform_int_distribution<u32> distribution(std::numeric_limits<u32>::min(), std::numeric_limits<u32>::max());
				return distribution(*g_seedGenerator);
			}
		}

		namespace Random
		{
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			std::mt19937& GetRandomNumberGenerator()
			{
				if (g_randomNumberGenerator == nullptr)
				{
					g_randomNumberGenerator = new std::mt19937(GenerateSeed());
				}

				return *g_randomNumberGenerator;
			}
		}
	}
}