//
//  DirectionalLightComponent.cpp
//  Chilli Source
//  Created by Scott Downie on 31/01/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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
#include <ChilliSource/Rendering/Lighting/DirectionalLightComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Image/ImageCompression.h>
#include <ChilliSource/Core/Image/ImageFormat.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Rendering/Base/RenderCapabilities.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            u32 g_nextShadowMapId = 0;
        }
        
		CS_DEFINE_NAMEDTYPE(DirectionalLightComponent);
        
        //----------------------------------------------------------
        /// Constructor
        //----------------------------------------------------------
        DirectionalLightComponent::DirectionalLightComponent(u32 in_shadowMapRes)
            : m_shadowMapRes(in_shadowMapRes), mfShadowTolerance(0.0f) , mbMatrixCacheValid(false)
		{
            m_shadowMapId = g_nextShadowMapId;
            ++g_nextShadowMapId;
            
            m_renderCapabilities = Core::Application::Get()->GetSystem<RenderCapabilities>();
            
            CreateShadowMapTextures();
		}
		//----------------------------------------------------------
		/// Is A
		//----------------------------------------------------------
		bool DirectionalLightComponent::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == LightComponent::InterfaceID || inInterfaceID == DirectionalLightComponent::InterfaceID;
		}
        //----------------------------------------------------------
        /// Set Shadow Volume
        //----------------------------------------------------------
        void DirectionalLightComponent::SetShadowVolume(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar)
        {
			mmatProj = Core::Matrix4::CreateOrthographicProjectionLH(infWidth, infHeight, infNear, infFar);
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Direction
        //----------------------------------------------------------
        Core::Vector3 DirectionalLightComponent::GetDirection() const
        {
            if(GetEntity() != nullptr)
            {
				return Core::Vector3::Rotate(Core::Vector3::k_unitPositiveZ, GetEntity()->GetTransform().GetWorldOrientation());
            }
            else
            {
				return Core::Vector3::k_unitPositiveZ;
            }
        }
        //----------------------------------------------------------
        /// Get Light Matrix
        //----------------------------------------------------------
        const Core::Matrix4& DirectionalLightComponent::GetLightMatrix() const
        {
            //The matrix is a view projection
            if(mbMatrixCacheValid == false && GetEntity() != nullptr)
            {
                Core::Matrix4 matView = Core::Matrix4::Inverse(GetEntity()->GetTransform().GetWorldTransform());
				mmatLight = matView * mmatProj;
                mbMatrixCacheValid = true;
            }
            
            return mmatLight;
        }
        //----------------------------------------------------------
        /// Get Shadow Tolerance
        //----------------------------------------------------------
        f32 DirectionalLightComponent::GetShadowTolerance() const
        {
            return mfShadowTolerance;
        }
        //----------------------------------------------------------
        /// Set Shadow Tolerance
        //----------------------------------------------------------
        void DirectionalLightComponent::SetShadowTolerance(f32 infTolerance)
        {
            mfShadowTolerance = infTolerance;
            
            mbCacheValid = false;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Ptr
        //----------------------------------------------------------
        const TextureSPtr& DirectionalLightComponent::GetShadowMapPtr() const
        {
            return m_shadowMap;
        }
        //----------------------------------------------------------
        /// Get Shadow Map Debug Ptr
        //----------------------------------------------------------
        const TextureSPtr& DirectionalLightComponent::GetShadowMapDebugPtr() const
        {
            return m_shadowMapDebug;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void DirectionalLightComponent::OnAddedToScene()
        {
            m_transformChangedConnection = GetEntity()->GetTransform().GetTransformChangedEvent().OpenConnection(Core::MakeDelegate(this, &DirectionalLightComponent::OnEntityTransformChanged));
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void DirectionalLightComponent::OnRemovedFromScene()
        {
            m_transformChangedConnection = nullptr;
        }
        //----------------------------------------------------
        /// On Entity Transform Changed
        //----------------------------------------------------
        void DirectionalLightComponent::OnEntityTransformChanged()
        {
            mbMatrixCacheValid = false;
            mbCacheValid = false;
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void DirectionalLightComponent::CreateShadowMapTextures()
        {
            if(m_shadowMap == nullptr && m_renderCapabilities->IsShadowMappingSupported() == true && m_shadowMapRes > 0)
            {
                m_shadowMap = Core::Application::Get()->GetResourcePool()->CreateResource<Rendering::Texture>("_ShadowMap" + Core::ToString(m_shadowMapId));
                Texture::Descriptor desc;
                desc.m_width = m_shadowMapRes;
                desc.m_height = m_shadowMapRes;
                desc.m_format = Core::ImageFormat::k_Depth16;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = 0;
                m_shadowMap->Build(desc, nullptr, false, false);
                
#ifdef CS_ENABLE_DEBUGSHADOW
                m_shadowMapDebug = Core::Application::Get()->GetResourcePool()->CreateResource<Rendering::Texture>("_ShadowMapDebug" + Core::ToString(m_shadowMapId));
                desc.m_width = m_shadowMapRes;
                desc.m_height = m_shadowMapRes;
                desc.m_format = Core::ImageFormat::k_RGB888;
                desc.m_compression = Core::ImageCompression::k_none;
                desc.m_dataSize = 0;
                m_shadowMapDebug->Build(desc, nullptr, false, false);
#endif
            }
        }
        //----------------------------------------------------
        //----------------------------------------------------
        void DirectionalLightComponent::DestroyShadowMapTextures()
        {
            Core::ResourcePool* resourcePool = Core::Application::Get()->GetResourcePool();
            
            if(m_shadowMap != nullptr)
            {
                Texture* release = m_shadowMap.get();
                m_shadowMap.reset();
                resourcePool->Release(release);
            }

            if(m_shadowMapDebug != nullptr)
            {
                Texture* release = m_shadowMapDebug.get();
                m_shadowMapDebug.reset();
                resourcePool->Release(release);
            }
        }
        //----------------------------------------------------------
        //----------------------------------------------------------
        DirectionalLightComponent::~DirectionalLightComponent()
        {
            DestroyShadowMapTextures();
        }
	}
}

