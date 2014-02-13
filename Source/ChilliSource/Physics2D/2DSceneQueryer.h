/*
 *  2DSceneQueryer.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 15/10/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PHYSICS2D_2DSCENEQUERYER_H_
#define _MOFLO_PHYSICS2D_2DSCENEQUERYER_H_

#include <moFlo/Physics2D/ForwardDeclarations.h>
#include <moFlo/Core/Math/Vector2.h>
#include <moFlo/Core/Math/Shapes.h>
#include <vector>

namespace moFlo {
	namespace Physics2D
	{
		
		enum QueryContactFilters
		{
			QCF_NONE = 0,
			QCF_TRIGGER_OBJECTS = 1<<0,
			QCF_CHARACTER_OBJECTS = 1<<1,
			QCF_DYNAMIC_OBJECTS = 1<<2,
			QCF_STATIC_OBJECTS = 1<<3,
			QCF_ALL = QCF_TRIGGER_OBJECTS | QCF_CHARACTER_OBJECTS | QCF_DYNAMIC_OBJECTS | QCF_STATIC_OBJECTS,
			QCF_STATIC_DYNAMICS = QCF_DYNAMIC_OBJECTS | QCF_STATIC_OBJECTS,
			QCF_NON_TRIGGERS = QCF_ALL & ~QCF_TRIGGER_OBJECTS
		};
		
		/**
		 * \struct RaycastQuery
		 *	 
		 */
		struct RaycastQuery
		{
			RaycastQuery()
			{
				bOnlyCareAboutFirstHit = false;
				eFilterFlags = QCF_ALL;
				eGroupMask = DefaultMask;
			}
			moFlo::Core::CVector2 vFrom; //<Start of the ray query (ZERO vector)
			moFlo::Core::CVector2 vTo; //<End of the query (ZERO vector)
			
			
			u32 eFilterFlags; //<A combination of the above flags which determine what categories of object (RCF_ALL by default)
			GroupIDMaskType eGroupMask; //<A bitmask of groups the raycast should intersect (DefaultMask by default)
			bool bOnlyCareAboutFirstHit; //<Whether the caller is only interested in the first object it hits rather than the number (false by default)
		};
		
		struct RaycastResult
		{
			RaycastResult()
			:pFirstIntersectingComponent(NULL),
			nIntersections(0)
			{
				
			}
			u32 nIntersections; //<How many objects the raycast hit
			moFlo::Core::IComponent* pFirstIntersectingComponent; //< The first object the raycast hit or NULL if it missed
			moFlo::Core::CVector2 vPointOfImpact; //<Where the raycast hit something
			moFlo::Core::CVector2 vImpactNormal; //<Normal
		};
		
		struct RectQuery
		{
			RectQuery()
			{
				eFilterFlags = QCF_ALL;
				eGroupMask = DefaultMask;
				bOnlyCareAboutFirstHit = false;
			}
			moFlo::Core::CRect sRect; //<Underlying rect shape which contains 
			u32 eFilterFlags; //<A combination of the above flags which determine what categories of object (RCF_ALL by default)
			GroupIDMaskType eGroupMask; //<A bitmask of groups the raycast should intersect (DefaultMask by default)
			bool bOnlyCareAboutFirstHit; //<Whether the caller is only interested in the first object it hits rather than the number (false by default)
		};
		
		struct RectResult
		{
			DYNAMIC_ARRAY<moFlo::Core::IComponent*> IntersectingComponents;
		};
		
		///Interface defining an object which can query the physics scene it is part of.
		class I2DSceneQueryer
		{
		public:
			virtual ~I2DSceneQueryer(){}
			virtual bool Raycast(const RaycastQuery & insQuery, RaycastResult & outsResult) = 0;
			virtual bool QueryRect(const RectQuery & insQuery, RectResult & outsResult) = 0;
		};
	}
}

#endif
