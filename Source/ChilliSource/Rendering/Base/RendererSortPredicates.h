//
//  RendererSortPredicates.h
//  Chilli Source
//  Created by Stuart McGaw on 26/04/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_RENDERING_RENDERERSORTPREDICATES_H_
#define _CHILLISOURCE_RENDERING_RENDERERSORTPREDICATES_H_

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