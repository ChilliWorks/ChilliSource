//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <ChilliSource/Rendering/Base/RenderPassObjectSorter.h>

#include <ChilliSource/Rendering/Camera/RenderCamera.h>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    void RenderPassObjectSorter::OpaqueSort(const RenderCamera& camera, std::vector<RenderPassObject>& renderPassObjects) noexcept
    {
        //The tile base deferred renderer on all iOS hardware does not benefit from front to back sorting https://developer.apple.com/library/content/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/Performance/Performance.html
#ifndef CS_TARGETPLATFORM_IOS
        std::sort(renderPassObjects.begin(), renderPassObjects.end(), [&camera](const RenderPassObject& a, const RenderPassObject& b)
        {
            if (a.GetRenderMaterial() == b.GetRenderMaterial())
            {
                Matrix4 wvpA = a.GetWorldMatrix() * camera.GetViewProjectionMatrix();
                Matrix4 wvpB = b.GetWorldMatrix() * camera.GetViewProjectionMatrix();
                
                if (wvpA.GetTranslation().z == wvpB.GetTranslation().z)
                {
                    return (a.GetRenderMesh() < b.GetRenderMesh());
                }
                else
                {
                    return (wvpA.GetTranslation().z > wvpB.GetTranslation().z);
                }
            }
            else
            {
                return (a.GetRenderMaterial() < b.GetRenderMaterial());
            }
        });
#endif
    }
    
    //------------------------------------------------------------------------------
    void RenderPassObjectSorter::TransparentSort(const RenderCamera& camera, std::vector<RenderPassObject>& renderPassObjects) noexcept
    {
        std::sort(renderPassObjects.begin(), renderPassObjects.end(), [&camera](const RenderPassObject& a, const RenderPassObject& b)
        {
            Matrix4 wvpA = a.GetWorldMatrix() * camera.GetViewProjectionMatrix();
            Matrix4 wvpB = b.GetWorldMatrix() * camera.GetViewProjectionMatrix();
            
            if (wvpA.GetTranslation().z == wvpB.GetTranslation().z)
            {
                return (a.GetRenderMesh() < b.GetRenderMesh());
            }
            else
            {
                return (wvpA.GetTranslation().z > wvpB.GetTranslation().z);
            }
        });
    }
    
    //------------------------------------------------------------------------------
    void RenderPassObjectSorter::PrioritySort(std::vector<RenderPassObject>& renderPassObjects) noexcept
    {
        std::sort(renderPassObjects.begin(), renderPassObjects.end(), [](const RenderPassObject& a, const RenderPassObject& b)
        {
            if (a.GetPriority() == b.GetPriority())
            {
                return (a.GetRenderMaterial() < b.GetRenderMaterial());
            }
            else
            {
                return (a.GetPriority() < b.GetPriority());
            }
        });
    }
}
