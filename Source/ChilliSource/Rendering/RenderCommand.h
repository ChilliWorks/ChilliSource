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

#ifndef _CHILLISOURCE_RENDERING_RENDERCOMMAND_H_
#define _CHILLISOURCE_RENDERING_RENDERCOMMAND_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandBuffer.h>
#include <ChilliSource/Rendering/RenderCommand/RenderCommandList.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyAmbientLightRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyCameraRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyDirectionalLightRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyDynamicMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMaterialRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyMeshBatchRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplyPointLightRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/ApplySkinnedAnimationRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/BeginRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/BeginWithTargetGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/EndRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTargetGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/LoadCubemapRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/RenderInstanceRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMaterialGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadMeshRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadShaderRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTargetGroupRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadTextureRenderCommand.h>
#include <ChilliSource/Rendering/RenderCommand/Commands/UnloadCubemapRenderCommand.h>

#endif
