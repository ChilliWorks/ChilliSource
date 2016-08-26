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

#include <ChilliSource/Rendering/Base/RenderFrameCompiler.h>

#include <vector>

namespace ChilliSource
{
    namespace
    {
        /// There is no reason to perform the ambient pass for each light separately, so
        /// the ambient light values can all be merged into a single light for the sake
        /// of performance.
        ///
        /// @param renderAmbientLights
        ///     The list of render ambient lights in the frame.
        ///
        /// @return A single ambient light representing all of the lights in the frame.
        ///
        AmbientRenderLight MergeAmbientRenderLights(const std::vector<AmbientRenderLight>& renderAmbientLights)
        {
            Colour mergedAmbientColour = Colour::k_transparent;
            
            for (const auto& renderAmbientLight : renderAmbientLights)
            {
                mergedAmbientColour += renderAmbientLight.GetColour();
            }
            
            mergedAmbientColour.a = 1.0f;
            
            return AmbientRenderLight(mergedAmbientColour);
        }
    }
    
    //------------------------------------------------------------------------------
    RenderFrame RenderFrameCompiler::CompileRenderFrame(const RenderTargetGroup* renderTarget, const Integer2& resolution, const Colour& clearColour, const RenderCamera& renderCamera, const std::vector<AmbientRenderLight>& renderAmbientLights,
                                                        const std::vector<DirectionalRenderLight>& renderDirectionalLights, const std::vector<PointRenderLight>& renderPointLights,
                                                        const std::vector<RenderObject>& renderObjects) noexcept
    {
        //TODO: Perform all render jobs in background tasks prior to building the complete render frame.
        
        auto renderAmbientLight = MergeAmbientRenderLights(renderAmbientLights);
        
        return RenderFrame(renderTarget, resolution, clearColour, renderCamera, renderAmbientLight, renderDirectionalLights, renderPointLights, renderObjects);
    }
}
