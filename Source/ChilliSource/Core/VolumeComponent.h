/*
 *  VolumeComponent.h
 *  MoFlow
 *
 *  Created by Ian Copland on 09/01/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_VOLUME_COMPONENT_H_
#define _MO_FLO_CORE_VOLUME_COMPONENT_H_

#include <ChilliSource/Core/Component.h>
#include <ChilliSource/Core/Math/Shapes.h>

namespace moFlo
{
	//====================================================
	/// Description:
	///
	/// Any object that has a AABB, OOBB or Bounding
	/// Sphere.
	//====================================================
	class IVolumeComponent : public Core::IComponent
	{
	public:
		DECLARE_NAMED_INTERFACE(IVolumeComponent);
		IVolumeComponent() : mfQueryIntersectionValue(0.0f){};
		virtual ~IVolumeComponent(){}

		//----------------------------------------------------
		/// Get Axis Aligned Bounding Box
		///
		/// All render components have a box used for culling
		/// and coarse intersections. This is cached and
		/// recomputed when required.
		/// @return Axis aligned bounding box
		//----------------------------------------------------
		virtual const Core::AABB& GetAABB() = 0;
		//----------------------------------------------------
		/// Get Object Oriented Bounding Box
		///
		/// All render objects have an OOBB for
		/// picking. This is cached and
		/// recomputed when required.
		/// @return OOBB
		//----------------------------------------------------
		virtual const Core::OOBB& GetOOBB() = 0;
		//----------------------------------------------------
		/// Get Bounding Sphere
		///
		/// All render objects have an bounding sphere for
		/// culling. This is cached and
		/// recomputed when required.
		/// @return bounding sphere
		//----------------------------------------------------
		virtual const Core::Sphere& GetBoundingSphere() = 0;
		//----------------------------------------------------
		/// Is Visible
		///
		/// @return Whether or not to render
		//----------------------------------------------------
		virtual bool IsVisible() const = 0;

	public:
		f32 mfQueryIntersectionValue;
	};
}

#endif
