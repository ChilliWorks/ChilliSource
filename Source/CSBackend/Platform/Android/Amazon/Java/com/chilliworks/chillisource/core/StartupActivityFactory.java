/**
 * StartupActivityFactory.java
 * ChilliSource
 * Created by Ian Copland on 01/06/2015.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.core;

import android.app.Activity;

/**
 * A factory class for creating a flavour specific Activity which is presented prior to the
 * CSActivity. There is no start up activity for Amazon builds so no activity will be
 * created by this factory.
 *
 * @author Ian Copland
 */
public final class StartupActivityFactory
{
    /**
     * Amazon builds do not require a start up activity, so this will never start an activity
     * and will always return false.
     *
     * @author Ian Copland
     *
     * @param in_currentActivity - The currently active activity, typically the CSActivity.
     *
     * @return Always returns false.
     */
    public static boolean tryStartActivity(Activity in_currentActivity)
    {
        return false;
    }
}
