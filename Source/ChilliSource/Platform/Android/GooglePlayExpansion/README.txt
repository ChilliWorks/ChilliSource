Scott Downie - 18th April 2013


Google Play Expansion Downloader for Android
=======================================
1. Copy all the jar files in this directory to the top level project folder and place in Libs (Required for release builds)

2. Add these jars to the java build path 

3. Add the expansiondlcstrings.xml and styles.xml to your res/values directory, Add status_bar_ongoing_event_progress_bar.xml to res/layout

4. You need to make modifications to your manifest.xml for extra permissions and registering services/activitys/recievers:

    <!-- Required to access Android Market Licensing -->
    <uses-permission android:name="com.android.vending.CHECK_LICENSE" />

    <!-- Required to keep CPU alive while downloading files (NOT to keep screen awake) -->
    <uses-permission android:name="android.permission.WAKE_LOCK" />

        <!-- Register the google play downloader activity -->
        <activity android:name="com.taggames.moflow.googlePlayDownloader.GooglePlayDownloaderActivity" 
            android:screenOrientation = "landscape" />
                
        <!-- Register our google play downloader as a service -->
        <service android:name="com.taggames.moflow.googlePlayDownloader.GooglePlayDownloaderService" />
        
        <!-- Register our google play download alarm as a receiver -->
        <receiver android:name="com.taggames.moflow.googlePlayDownloader.GooglePlayAlarmReceiver" />



