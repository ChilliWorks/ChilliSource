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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4.h>

#include <vector>

namespace ChilliSource
{
	namespace Rendering
    {
		//---------------------------------------------------------
		/// An abstract base for classes representing different ordering operations
		//---------------------------------------------------------
		class RendererSortPredicate
        {
		public:
			virtual ~RendererSortPredicate(){}
			virtual void PrepareForSort(std::vector<RenderComponent*> * inpRenderables = nullptr) = 0;
			virtual bool SortItem(const RenderComponent* p1, const RenderComponent* p2) const = 0;
			bool operator()(const RenderComponent* p1, const RenderComponent* p2) const
            {
				return SortItem(p1, p2);
			}
		};

		//---------------------------------------------------------
		/// This class doesn't really sort objects at all
		//---------------------------------------------------------
		class NullSortPredicate final : public RendererSortPredicate
        {
		public:
            void PrepareForSort(std::vector<RenderComponent*> * inpRenderables = nullptr) override {}
            bool SortItem(const RenderComponent* p1, const RenderComponent* p2) const override {return false;}
		};
		
		//---------------------------------------------------------
		/// This class sorts objects in order of far to near projected screen Z depth
		//---------------------------------------------------------
		class BackToFrontSortPredicate final : public RendererSortPredicate
        {
		public:
            void PrepareForSort(std::vector<RenderComponent*> * inpRenderables = nullptr) override;
            bool SortItem(const RenderComponent* p1, const RenderComponent* p2) const override;
		private:
			Core::Matrix4 mCameraViewProj;
		};
        
        //---------------------------------------------------------
		/// This class sorts objects by material pointer
		//---------------------------------------------------------
		class MaterialSortPredicate final : public RendererSortPredicate
        {
		public:
            void PrepareForSort(std::vector<RenderComponent*> * inpRenderables = nullptr) override {}
            bool SortItem(const RenderComponent* p1, const RenderComponent* p2) const override;
		};
	}
}

#endif