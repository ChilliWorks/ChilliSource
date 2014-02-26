/*
 *  MoAnimLoader.h
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */


#ifndef _MO_FLO_RENDERING_MOANIM_LOADER_H_
#define _MO_FLO_RENDERING_MOANIM_LOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Rendering
	{
		//-------------------------------------------------------------------------
		/// MoAnimLoader class.
		/// Loads Mo Anim files into a SkinnedAnimation
		//-------------------------------------------------------------------------
		class MoAnimLoader : public Core::IResourceProvider
		{
		public:
			MoAnimLoader(Core::CApplication* inpApp);
			~MoAnimLoader();
			
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------------------
			/// Can Create Resource From File With Extension
			///
			/// @param Extension to compare
			/// @return Whether the object can create a resource with the given extension
			//----------------------------------------------------------------------------
			bool CanCreateResourceFromFileWithExtension(const std::string & inExtension) const override;

		private:
			//----------------------------------------------------------------------------
			/// Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource) override;
			//----------------------------------------------------------------------------
			/// Async Create Resource From File
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool AsyncCreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource) override;
			//----------------------------------------------------------------------------
			/// ReadAnimationTask
			///
			/// Reads a MoAnim file asynchroniously
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			//----------------------------------------------------------------------------
			void ReadAnimationTask(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, const SkinnedAnimationSPtr& outpResource);
			//----------------------------------------------------------------------------
			/// Create Skinned Animation From File
			///
			/// Loads a MoAnim file into a SkinnedAnimation.
			///
            /// @param The storage location to load from
			/// @param Filename
			/// @param the output resource pointer
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool CreateSkinnedAnimationFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, const SkinnedAnimationSPtr& outpResource);
			//----------------------------------------------------------------------------
			/// Read Header
			///
			/// Reads the header of a MoAnim file.
			///
			/// @param The file stream.
			/// @param the Skeletal Animation that this data is being loaded into.
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool ReadHeader(const ChilliSource::Core::FileStreamPtr& inpStream, const std::string & inFilePath, const SkinnedAnimationSPtr& outpResource, u32& outudwNumFrames, s32& outdwNumSkeletonNodes);
			//----------------------------------------------------------------------------
			/// Read Animation Data
			///
			/// Reads all of the data for the animation into the SkinnedAnimation.
			///
			/// @param The file stream.
			/// @param the Skeletal Animation that this data is being loaded into.
            /// @param The number of frames.
            /// @param The number of skeleton nodes.
			/// @return whether or not this was successful
			//----------------------------------------------------------------------------
			bool ReadAnimationData(const ChilliSource::Core::FileStreamPtr& inpStream, u32 inudwNumFrames, s32 indwNumSkeletonNodes, const SkinnedAnimationSPtr& outpResource);
			//----------------------------------------------------------------------------
			/// Read Value (Templated)
			///
			/// Read value in for given type
			/// @param File stream
			/// @return Value of type T
			//----------------------------------------------------------------------------
			template <typename T> T ReadValue(const ChilliSource::Core::FileStreamPtr& inpFileStream)
			{
				T Value;
				inpFileStream->Read(reinterpret_cast<s8*>(&Value), sizeof(T));
				return Value;
			}
			
			friend class SkinnedAnimation;
		private:	
			
			Core::CApplication* mpApp;
		};
	}
}

#endif
