/*
 *  3DPhysicsSystem.h
 *  moFloTest
 *
 *  Created by Scott Downie on 07/02/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLO_PHYSICS3D_I3DPHYSICSSYSTEM_H
#define _MOFLO_PHYSICS3D_I3DPHYSICSSYSTEM_H

#include <moFlo/Physics3D/ForwardDeclarations.h>
#include <moFlo/Core/System.h>
#include <moFlo/Core/SystemConcepts.h>
#include <moFlo/Core/ForwardDeclarations.h>
#include <moFlo/Core/Component.h>
#include <moFlo/Core/Math/Vector3.h>

namespace moFlo
{
	namespace Physics3D
	{
		//A standard set of data about a physical object (not all component types use all variables)
		struct Physics3DComponentDesc 
		{
			Core::CVector3 vPosition;			//< Initial position of the physics object
			Core::CVector3 vInertia;			//< Initial interia of the object
			Core::CQuaternion qOrientation;		//< Initial orientation
			
			f32 fMass;							//< Mass of the object
			
			///Default 
			Physics3DComponentDesc()
			: fMass(0){}
		};
		
		class I3DPhysicsSystem : public Core::ISystem, public Core::IUpdateable, public Core::IComponentProducer
		{
		public:
			DECLARE_NAMED_INTERFACE(I3DPhysicsSystem);
			
			I3DPhysicsSystem(): mvGravity(0.0f, -10.0f, 0.0f), mfTimeScaleFactor(1.0f){}
			virtual ~I3DPhysicsSystem(){};
			
			//-------------------------------------------------------------------------------
			/// Set Gravity
			///
			/// @param Gravity velocity vector
			//-------------------------------------------------------------------------------
			virtual void SetGravity(const Core::CVector3& invGravity) = 0;
			//-------------------------------------------------------------------------------
			/// Get Gravity
			///
			/// @return Gravity velocity vector
			//-------------------------------------------------------------------------------
			virtual const Core::CVector3 & GetGravity() const = 0;
			//-------------------------------------------------------------------------------
			/// Set Time Scale Factor
			///
			/// @param Scale factor for time between physics steps
			//-------------------------------------------------------------------------------
			void SetTimeScaleFactor(f32 infFactor){mfTimeScaleFactor = infFactor;}
			//-------------------------------------------------------------------------------
			/// Get Time Scale Factor
			///
			/// @return Scale factor for time between physics steps
			//-------------------------------------------------------------------------------
			f32 GetTimeScaleFactor() const{return mfTimeScaleFactor;}
			
		protected:
			
			Core::CVector3 mvGravity;
			
			f32 mfTimeScaleFactor;
		};
	}	
}

#endif