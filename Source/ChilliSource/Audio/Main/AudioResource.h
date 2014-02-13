/*
 * File: AudioResource.h
 * Date: 16/11/2010 2010 
 * Description: Holds a sound effect resource
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_RESOURCE_H_
#define _MO_FLO_AUDIO_RESOURCE_H_

#include <ChilliSource/Core/Resource.h>

namespace moFlo 
{
	namespace Audio
	{
		class IAudioResource : public Core::IResource
		{
		public:
			DECLARE_NAMED_INTERFACE(IAudioResource);
			virtual ~IAudioResource(){}
        
			//---------------------------------------------------------------------
			/// Get Length
			///
			/// @return Sound length in seconds
			//---------------------------------------------------------------------
			virtual f32 GetLength() = 0;
            //---------------------------------------------------------------------
            /// Set Looping
            ///
            /// @param Whether to loop the sound or not
            //---------------------------------------------------------------------
            virtual void SetLooping(bool inbShouldLoop) = 0;
			//---------------------------------------------------------------------
			/// Is Streamed
			///
			/// @return Whether the resource is streamed
			//---------------------------------------------------------------------
			bool IsStreamed() const{return mbStreaming;}
			//---------------------------------------------------------------------
			/// Set Streamed
			///
			/// @param Whether the resource is streamed
			//---------------------------------------------------------------------
			void SetStreamed(bool inbIsStreamed){mbStreaming = inbIsStreamed;}

		protected:
			bool mbStreaming;
		};
	}
}

#endif

