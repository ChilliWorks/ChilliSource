//
//  ForwardDeclarations.h
//  moFlow
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


namespace ChilliSource
{
    namespace Video
    {
        class IVideoPlayerActivity;
        class CSubtitles;
        class CSubtitlesManager;
        class CMoSubtitlesLoader;
        
        typedef std::shared_ptr<CSubtitles> SubtitlesPtr;
        typedef std::shared_ptr<CSubtitlesManager> SubtitlesManagerPtr;
        typedef std::shared_ptr<CMoSubtitlesLoader> MoSubtitlesLoaderPtr;
    }
}