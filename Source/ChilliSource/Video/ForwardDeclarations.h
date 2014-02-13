//
//  ForwardDeclarations.h
//  moFlow
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


namespace moFlo
{
    namespace Video
    {
        class IVideoPlayerActivity;
        class CSubtitles;
        class CSubtitlesManager;
        class CMoSubtitlesLoader;
        
        typedef SHARED_PTR<CSubtitles> SubtitlesPtr;
        typedef SHARED_PTR<CSubtitlesManager> SubtitlesManagerPtr;
        typedef SHARED_PTR<CMoSubtitlesLoader> MoSubtitlesLoaderPtr;
    }
}