//
//  Transform.h
//  moFloTest
//
//  Created by Scott Downie on 24/03/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_CORE_TRANSFORM_H_
#define _MO_FLO_CORE_TRANSFORM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Matrix4x4Old.h>
#include <ChilliSource/Core/Math/Vector3Old.h>
#include <ChilliSource/Core/Math/QuaternionOld.h>

#include <functional>

namespace ChilliSource
{
    namespace Core
    {
        class Transform
        {
        public:
            typedef std::function<void()> TransformChangedDelegate;
            
            Transform();
            //----------------------------------------------------------
            /// Set Look At
            ///
            /// Set the camera orientation, target and position
            /// @param Position
            /// @param Look target
            /// @param Up direction
            //----------------------------------------------------------
            void SetLookAt(const Core::Vector3Old& invPos, const Core::Vector3Old& invTarget, const Core::Vector3Old& invUp);
            //----------------------------------------------------------------
			/// Set Position Scale Orientation
			///
			/// @param Position vector
            /// @param Scale vector
            /// @param Orientation quaternion
			//----------------------------------------------------------------
            void SetPositionScaleOrientation(const Vector3Old& invPos, const Vector3Old& invScale, const QuaternionOld& invOrientation);
            //----------------------------------------------------------------
			/// Set Position
			///
			/// @param X Component
			/// @param Y Component
			/// @param Z Component
			//----------------------------------------------------------------
			void SetPosition(f32 infX, f32 infY, f32 infZ = 0.0f);
			//----------------------------------------------------------------
			/// Set Position
			///
			/// @param Position vector
			//----------------------------------------------------------------
			void SetPosition(const Vector3Old &invPos);
			//----------------------------------------------------------------
			/// Get Local Position
			///
			/// @return The position of the object 
			//----------------------------------------------------------------
			const Vector3Old& GetLocalPosition() const;
            //----------------------------------------------------------------
			/// Get World Position
			///
			/// @return The relative position of the object 
			//----------------------------------------------------------------
			const Vector3Old& GetWorldPosition() const;
			//----------------------------------------------------------------
			/// Move By
			///
			/// @param X Component
			/// @param Y Component
			/// @param Z Component
			//----------------------------------------------------------------
			void MoveBy(f32 infX, f32 infY, f32 infZ = 0.0f);
			//----------------------------------------------------------------
			/// Move By
			///
			/// Movement direction vector
			//----------------------------------------------------------------
			void MoveBy(const Vector3Old &invPos);
			//----------------------------------------------------------------
			/// Set Orientation
			///
			/// @param Orientation quaternion
			//----------------------------------------------------------------
			void SetOrientation(const QuaternionOld & inqOrientation);
			//----------------------------------------------------------------
			/// Get Local Orientation
			///
			/// @return Orientation quaternion
			//----------------------------------------------------------------
			const QuaternionOld& GetLocalOrientation() const;
            //----------------------------------------------------------------
			/// Get World Orientation
			///
			/// @return Orientation quaternion relative to parent tranform
			//----------------------------------------------------------------
			const QuaternionOld& GetWorldOrientation() const;
			//----------------------------------------------------------------
			/// Rotate X By
			///
			/// Rotate around the x-axis
			///
			/// @param Angle to rotate in radians
			//----------------------------------------------------------------
			void RotateXBy(f32 infAngleRads);
			//----------------------------------------------------------------
			/// Rotate Y By
			///
			/// Rotate around the y-axis
			///
			/// @param Angle to rotate in radians
			//----------------------------------------------------------------
			void RotateYBy(f32 infAngleRads);
			//----------------------------------------------------------------
			/// Rotate Z By
			///
			/// Rotate around the z-axis
			///
			/// @param Angle to rotate in radians
			//----------------------------------------------------------------
			void RotateZBy(f32 infAngleRads);
			//----------------------------------------------------------------
			/// Rotate By
			///
			/// Rotate around the arbitary axis
			///
			/// @param X-axis Component
			/// @param Y-axis Component
			/// @param Z-axis Component
			/// @param Angle in radians
			//----------------------------------------------------------------
			void RotateBy(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads);
			//----------------------------------------------------------------
			/// Rotate By
			///
			/// Rotate around the arbitary axis
			///
			/// @param Axis vector
			/// @param Angle in radians
			//----------------------------------------------------------------
			void RotateBy(const Vector3Old &vAxis, f32 infAngleRads);
			//----------------------------------------------------------------
			/// Rotate To
			///
			/// Rotate about the given axis to the given angle 
			///
			/// @param X-axis Component
			/// @param Y-axis Component
			/// @param Z-axis Component
			/// @param Angle in radians
			//----------------------------------------------------------------
			void RotateTo(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads);
			//----------------------------------------------------------------
			/// Rotate To
			///
			/// Rotate about the given axis to the given angle 
			///
			/// @param Axis vector
			/// @param Angle in radians
			//----------------------------------------------------------------
			void RotateTo(const Vector3Old &vAxis, f32 infAngleRads);
			//----------------------------------------------------------------
			/// Scale By
			///
			/// Scale uniformly by the given amount
			///
			/// @param Scale factor
			//----------------------------------------------------------------
			void ScaleBy(f32 inScale);
			//----------------------------------------------------------------
			/// Scale By
			///
			/// Scale along each axis by the given amount
			///
			/// @param X Component
			/// @param Y Component
			/// @param Z Component
			//----------------------------------------------------------------
			void ScaleBy(f32 inX, f32 inY, f32 inZ = 0.0f);
			//----------------------------------------------------------------
			/// Scale By
			///
			/// Scale along each axis by the given amount
			///
			/// @param Axis vector
			//----------------------------------------------------------------
			void ScaleBy(const Vector3Old &Vec);
			//----------------------------------------------------------------
			/// Scale To
			///
			/// Scale uniformly to the given amount
			///
			/// @param Scale factor
			//----------------------------------------------------------------
			void ScaleTo(f32 inScale);
			//----------------------------------------------------------------
			/// Scale To
			///
			/// Scale each axis to the given amount
			///
			/// @param X Component
			/// @param Y Component
			/// @param Z Component
			//----------------------------------------------------------------
			void ScaleTo(f32 inX, f32 inY, f32 inZ = 0.0f);
			//----------------------------------------------------------------
			/// Scale To
			///
			/// Scale each axis to the given amount
			///
			/// @param Axis vector
			//----------------------------------------------------------------
			void ScaleTo(const Vector3Old &Vec);
			//----------------------------------------------------------------
			/// Get Scale
			///
			/// @return The scale of object
			//----------------------------------------------------------------
			const Vector3Old& GetLocalScale() const;
            //----------------------------------------------------------------
			/// Get World Scale
			///
			/// @return The relative scale of object
			//----------------------------------------------------------------
			const Vector3Old& GetWorldScale() const;
            
            
            //----------------------------------------------------------------
			/// Fade By
			///
			/// Change of the opacity of the object
			///
			/// @param inFade The amount to fade by
			//----------------------------------------------------------------
            void FadeBy(f32 inFade);
			//----------------------------------------------------------------
			/// Scale To
			///
			/// Fade uniformly to the given amount
			///
			/// @param Fade factor
			//----------------------------------------------------------------
			void FadeTo(f32 inFade);

			//----------------------------------------------------------------
			/// Get the opacity
			///
			/// @return The opacity of object
			//----------------------------------------------------------------
			const f32 GetLocalOpacity() const;
            
            //----------------------------------------------------------------
			/// Get World opacity
			///
			/// @return The relative opacity of object
			//----------------------------------------------------------------
			const f32 GetWorldOpacity() const;
            
            
            
            //----------------------------------------------------------------
			/// Get Local Transform
			///
			/// @return The currently cached transform (rebuilds if invalid)
			//----------------------------------------------------------------
			const Matrix4x4Old& GetLocalTransform() const;
            //----------------------------------------------------------------
            /// Get World Transform
            ///
            /// @return The tranform in relation to its parent transform
            //----------------------------------------------------------------
			const Matrix4x4Old& GetWorldTransform() const;
            //----------------------------------------------------------------
            /// Set World Transform
            ///
            /// This will overwrite any parent or previous transformations
            ///
            /// @param Objects transformation matrix
            //----------------------------------------------------------------
            void SetWorldTransform(const Matrix4x4Old& inmatTransform);
            //----------------------------------------------------------------
            /// Set Local Transform
            ///
            /// This will overwrite any local previous transformations
            ///
            /// @param Objects transformation matrix
            //----------------------------------------------------------------
            void SetLocalTransform(const Matrix4x4Old& inmatTransform);
            //----------------------------------------------------------------
            /// Is Transform Valid
            ///
            /// @return Whether the transform cache has been invalidated 
            //----------------------------------------------------------------
            bool IsTransformValid() const;
            //----------------------------------------------------------------
            /// Set Parent Transform
            ///
            /// Link this transform to another so that our transformations
            /// become World to the parent transform
            ///
            /// @param Transform object
            //----------------------------------------------------------------
            void SetParentTransform(Transform* inpTransform);
			//----------------------------------------------------------------
            /// Get Parent Transform
            /// @return what it says on tin
            //----------------------------------------------------------------
            Transform* GetParentTransform() const;
			//----------------------------------------------------------------
            /// Add Child Transform
            ///
            /// Link this transform to another so that our transformations
            /// influence the childs transform
            ///
            /// @param Transform object
            //----------------------------------------------------------------
            void AddChildTransform(Transform* inpTransform);
			//----------------------------------------------------------------
            /// Remove Child Transform
            ///
            /// Unlink this transform from another so that our transformations
            /// no longer influence the childs transform
            ///
            /// @param Transform object
			/// @return Whether the child was removed successfully
            //----------------------------------------------------------------
            bool RemoveChildTransform(Transform* inpTransform);
			//----------------------------------------------------------------
            /// Remove All Child Transforms
            ///
            /// Unlink this transform from all children so that our transformations
            /// no longer influence the childs transform
            //----------------------------------------------------------------
            void RemoveAllChildTransforms();
            //----------------------------------------------------------------
            /// Get Tranform Changed Event
            ///
            /// Subscribe to this event for notifications of when this
            /// transform is invalidated
            ///
            /// @return TransformChangedDelegate event
            //----------------------------------------------------------------
            IConnectableEvent<TransformChangedDelegate>& GetTransformChangedEvent();
            
            //----------------------------------------------------------------
            /// Resets the transform back to identity and removes any
            /// connections
            ///
            /// @author S Downie
            //----------------------------------------------------------------
            void Reset();
            
        private:
            
            //----------------------------------------------------------------
            /// On Transform Changed 
            ///
            /// Triggered when our transform changes so we can 
            /// notify any dependant transforms
            //----------------------------------------------------------------
            void OnTransformChanged();
            //----------------------------------------------------------------
            /// On Parent Transform Changed 
            ///
            /// Triggered when our parent transform changes so we can 
            /// recalculate our transform
            //----------------------------------------------------------------
            void OnParentTransformChanged();
            
        private:
            
            mutable Matrix4x4Old mmatTransform;
            mutable Matrix4x4Old mmatWorldTransform;
            
            Vector3Old mvPosition;
			Vector3Old mvScale;
			QuaternionOld mqOrientation;
            
            f32 mfOpacity;
            
            mutable Vector3Old mvWorldPosition;
			mutable Vector3Old mvWorldScale;
			mutable QuaternionOld mqWorldOrientation;
            
            Event<TransformChangedDelegate> mTransformChangedEvent;
            
            Transform* mpParentTransform;
			
			std::vector<Transform*> mChildTransforms;
            
            mutable bool mbIsTransformCacheValid;
            mutable bool mbIsParentTransformCacheValid;
        };
    }
}

#endif