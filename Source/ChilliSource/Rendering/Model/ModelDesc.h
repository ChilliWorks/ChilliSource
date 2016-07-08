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

#ifndef _CHILLISOURCE_RENDERING_MODEL_MODELDESC_H_
#define _CHILLISOURCE_RENDERING_MODEL_MODELDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Geometry/Shapes.h>
#include <ChilliSource/Rendering/Model/MeshDesc.h>
#include <ChilliSource/Rendering/Model/SkeletonDesc.h>

namespace ChilliSource
{
    /// A description of a model. This is used to build a single model instance. Attempting
    /// to use the same model description to create a second model will assert, as the mesh
    /// data will have been moved to the first model instance.
    ///
    /// This is not thread safe and should only be accessed by one thread at a time.
    ///
    class ModelDesc final
    {
    public:
        CS_DECLARE_NOCOPY(ModelDesc);
        
        ModelDesc() = default;
        ModelDesc(ModelDesc&&) = default;
        ModelDesc& operator=(ModelDesc&&) = default;
        
        /// Creates a new model description with the given mesh data and bounds.
        ///
        /// @param meshDescs
        ///     The list of mesh descriptions.
        /// @param aabb
        ///     The local AABB of the model.
        /// @param boundingSphere
        ///     The local bounding sphere of the model.
        /// @param shouldBackupData
        ///     If the model mesh data should be backed up in main memory for restoring it later.
        ///
        ModelDesc(std::vector<MeshDesc> meshDescs, const AABB& aabb, const Sphere& boundingSphere, bool shouldBackupData = true) noexcept;
        
        /// Creates a new model description with the given mesh data, bounds and skeleton
        /// description.
        ///
        /// @param meshDescs
        ///     The list of mesh descriptions. This must be passed by move to avoid duplicating
        ///     mesh data.
        /// @param aabb
        ///     The local AABB of the model.
        /// @param boundingSphere
        ///     The local bounding sphere of the model.
        /// @param skeletonDesc
        ///     The skeleton description.
        /// @param shouldBackupData
        ///     If the model mesh data should be backed up in main memory for restoring it later.
        ///
        ModelDesc(std::vector<MeshDesc> meshDescs, const AABB& aabb, const Sphere& boundingSphere, const SkeletonDesc& skeletonDesc, bool shouldBackupData = true) noexcept;
        
        /// @return The list of mesh descriptions.
        ///
        u32 GetNumMeshDescs() const noexcept { return u32(m_meshDescs.size()); }
        
        /// @param index
        ///     The mesh description index. Must be lower than the value returned from
        ///     GetNumMeshDesc() else this will assert.
        ///
        /// @return The mesh description at the given index.
        ///
        MeshDesc& GetMeshDesc(u32 index) noexcept;
        
        /// @param index
        ///     The mesh description index. Must be lower than the value returned from
        ///     GetNumMeshDesc() else this will assert.
        ///
        /// @return The mesh description at the given index.
        ///
        const MeshDesc& GetMeshDesc(u32 index) const noexcept;
        
        /// @return The local AABB of the mode.
        ///
        const AABB& GetAABB() const noexcept { return m_aabb;  }
        
        /// @return The local bounding sphere of the mode.
        ///
        const Sphere& GetBoundingSphere() const noexcept { return m_boundingSphere;  }
        
        /// @return The skeleton description.
        ///
        const SkeletonDesc& GetSkeletonDesc() const noexcept { return m_skeletonDesc; }
        
        /// @return If the data should be backed up for restoration.
        ///
        bool ShouldBackupData() const noexcept { return m_shouldBackupData; }
        
    private:
        std::vector<MeshDesc> m_meshDescs;
        AABB m_aabb;
        Sphere m_boundingSphere;
        SkeletonDesc m_skeletonDesc;
        bool m_shouldBackupData = true;
    };
}

#endif
