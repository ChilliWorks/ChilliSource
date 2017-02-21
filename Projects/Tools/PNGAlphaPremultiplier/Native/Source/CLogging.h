/**
 * CLogging.h
 * ChilliSource
 * Created by Ian Copland on 25/06/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _PNGALPHAPREMULTIPLIER_CLOGGING_H_
#define _PNGALPHAPREMULTIPLIER_CLOGGING_H_

#ifdef __cplusplus
extern "C" 
{
#endif 

/**
 * Provides a C interface for the Logging::LogVerbose() function.
 * 
 * @author Ian Copland
 * 
 * @param The message, null terminated.
 */
void LogVerbose(const char* in_message);
/**
 * Provides a C interface for the Logging::LogWarning() function.
 * 
 * @author Ian Copland
 * 
 * @param The message, null terminated.
 */
void LogWarning(const char* in_message);
/**
 * Provides a C interface for the Logging::LogError() function.
 * 
 * @author Ian Copland
 * 
 * @param The message, null terminated.
 */
void LogError(const char* in_message);
/**
 * Provides a C interface for the Logging::LogFatal() function.
 * 
 * @author Ian Copland
 * 
 * @param The message, null terminated.
 */
void LogFatal(const char* in_message);

#ifdef __cplusplus
}
#endif

#endif
