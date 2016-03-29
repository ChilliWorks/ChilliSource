#
#  android-proguard-rules.pro
#  Chilli Source
#  Created by Ian Copland on 24/03/2015.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2015 Tag Games Limited
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#

-dontpreverify
-repackageclasses ''
-allowaccessmodification
-optimizations !code/simplification/arithmetic

#keep all of the classes accessible through the AndroidManifest.xml
-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider

#keep everthing in R
-keep public class **.R { public *; }
-keep public class **.R$* { public *; }

#Keep all classes with native methods
-keepclasseswithmembers class * {
    native <methods>;
}

#Keep all java systems as these are typically accessed from native
-keep public class com.chilliworks.chillisource.core.System { public *; }
-keep public class * extends com.chilliworks.chillisource.core.System { public *; }

#Keep boxed pointers
-keep public class com.chilliworks.chillisource.core.BoxedPointer { public *; }

#also keep old-style Native Interfaces. This will be removed in the future
-keep public class **NativeInterface { *; }

#keep anything which extends ApkExpansionDownloader, since reflection is used create them
-keep public class * extends com.chilliworks.chillisource.networking.ApkExpansionDownloadView { public *; }

#keep the following classes are they are accessed from native
-keep public class com.chilliworks.chillisource.core.LocalNotification { public *; }
-keep public class com.chilliworks.chillisource.core.ApkExpansionInfo { public *; }
-keep public class com.chilliworks.chillisource.core.ApkExpansionContentProvider { public *; }
-keep public class com.chilliworks.chillisource.networking.HttpRequest { public *; }

#keep facebook
-keep class com.facebook.** { *; }
-keepattributes Signature

#keep google play classes
-keep class com.google.android.vending.expansion.downloader.impl.DownloadsDB

#keep amazon classes
-dontwarn com.amazon.**
-keep class com.amazon.** {*;}
-keepattributes *Annotation*