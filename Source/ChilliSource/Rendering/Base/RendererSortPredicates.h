/*
 *  RendererSortPredicates.h
 *  MoFlow
 *
 *  Created by Stuart McGaw on 26/04/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_RENDERING_RENDERERSORTPREDICATES_H_
#define _MOFLO_RENDERING_RENDERERSORTPREDICATES_H_

#include <ChilliSource/Rendering/ForwardDeclarations.h>

namespace ChilliSource
{
	namespace Rendering
    {
		//---------------------------------------------------------
		/// An abstract base for classes representing different ordering operations
		//---------------------------------------------------------
		class CRendererSortPredicate
        {
		public:
			virtual ~CRendererSortPredicate(){}
			virtual void PrepareForSort(DYNAMIC_ARRAY<IRenderComponent*> * inpRenderables = NULL) = 0;
			virtual bool SortItem(IRenderComponent* p1, IRenderComponent* p2) = 0;
			bool operator()(IRenderComponent* p1, IRenderComponent* p2)
            {
				return SortItem(p1, p2);
			}
		};

		//---------------------------------------------------------
		/// This class doesn't really sort objects at all
		//---------------------------------------------------------
		class CNullSortPredicate : public CRendererSortPredicate
        {
		public:
            void PrepareForSort(DYNAMIC_ARRAY<IRenderComponent*> * inpRenderables = NULL){}
            bool SortItem(IRenderComponent* p1, IRenderComponent* p2){return false;}
		};
		
		//---------------------------------------------------------
		/// This class sorts objects in order of far to near projected screen Z depth
		//---------------------------------------------------------
		class CBackToFrontSortPredicate : public CRendererSortPredicate
        {
		public:
            void PrepareForSort(DYNAMIC_ARRAY<IRenderComponent*> * inpRenderables = NULL);
            bool SortItem(IRenderComponent* p1, IRenderComponent* p2);
		private:
			Core::CMatrix4x4 mCameraViewProj;
		};
        
        //---------------------------------------------------------
		/// This class sorts objects by material pointer
		//---------------------------------------------------------
		class CMaterialSortPredicate : public CRendererSortPredicate
        {
		public:
			virtual void PrepareForSort(DYNAMIC_ARRAY<IRenderComponent*> * inpRenderables = NULL){}
			virtual bool SortItem(IRenderComponent* p1, IRenderComponent* p2);
		};
	}
}

#endif