#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace moFlo 
{
	namespace Core 
	{
		const CVector2 CVector2::ZERO;
		const CVector2 CVector2::ONE(1.0f,1.0f);
		const CVector2 CVector2::X_UNIT_POSITIVE(1.0f,0);
		const CVector2 CVector2::X_UNIT_NEGATIVE(-1.0f,0);
		const CVector2 CVector2::Y_UNIT_POSITIVE(0,1.0f);
		const CVector2 CVector2::Y_UNIT_NEGATIVE(0,-1.0f);
		//---------------------------------------------------------
		/// Constructor
		///
		/// Default. Initialises to zero
		//---------------------------------------------------------
		CVector2::CVector2() : x(0.0f), y(0.0f)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise individual components
		/// @param X component
		/// @param Y component
		/// @param Z component
		//---------------------------------------------------------
		CVector2::CVector2(const f32 fX, const f32 fY) : x(fX), y(fY)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector2::CVector2(const CVector2 &Vec)
		{
			(*this) = Vec;
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector2::CVector2(const CVector3 &Vec) : x(Vec.x), y(Vec.y)
		{
			
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector2::CVector2(const CVector4 &Vec) : x(Vec.x), y(Vec.y)
		{
			
		}
		//---------------------------------------------------------
		/// Vector2 - Normalise
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
        CVector2 & CVector2::Normalise()
		{
			//Calculate Magnitude
			f32 fMag = Length();
			//Prevent division by zero
			if(fMag == 0) fMag = 1; 
			//Return Unit Vector
			this->x/=fMag;
			this->y/=fMag;
            
            return *this;
		}
		//---------------------------------------------------------
		/// Vector2 - NormalisedCopy
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
		CVector2 CVector2::NormalisedCopy() const
		{
			CVector2 Out;
			//Calculate Magnitude
			f32 fMag = Length();
			//Prevent division by zero
			if(fMag == 0) fMag = 1; 
			//Return Unit Vector
			Out.x = this->x/fMag;
			Out.y = this->y/fMag;
			
			return Out;
		}
		//---------------------------------------------------------
		/// Vector2 - Dot Product
		///
		/// Return the scalar product between two vectors
		/// @param The adjoining vector
		/// @return The scalar product
		//---------------------------------------------------------
		f32 CVector2::DotProduct(const CVector2 &inOtherVec) const
		{
			return (this->x * inOtherVec.x) + (this->y * inOtherVec.y);
		}
		//---------------------------------------------------------
		/// Vector2 - Length Squared
		///
		/// Calculate the  squared magnitude of a vector
		/// no costly sqrt function
		/// @return The magnitude squared
		//---------------------------------------------------------
		f32 CVector2::LengthSquared() const
		{
			return (this->x * this->x + this->y * this->y);
		}
		//---------------------------------------------------------
		/// Vector2 - Length
		///
		/// Calculate the magnitude of a vector
		/// @return The magnitude of the vector
		//---------------------------------------------------------
		f32 CVector2::Length() const
		{
			//Calculate Magnitude
			return (f32)sqrt(this->x * this->x + this->y * this->y);
		}
		//---------------------------------------------------------
		/// Vector2 - Angle Between Two Vectors
		///
		/// Using the Dot Product
		/// 
		/// @param The vector to which the angle is defined
		/// @return The angle in radians
		//---------------------------------------------------------
		f32 CVector2::Angle(const CVector2 &inOtherVec) const
		{			
			return std::atan2(inOtherVec.y,inOtherVec.x) - std::atan2(y,x);
		}
        CVector2 & CVector2::Inverse()
        {
            if(this->x != 0.0f)
			{
				this->x = 1.0f/this->x;
			}
			if(this->y != 0.0f)
			{
				this->y = 1.0f/this->y;
			}
            
            return *this;
        }
        CVector2 CVector2::InversedCopy() const
        {
            CVector2 Out;
            
            if(this->x != 0.0f)
			{
				Out.x = 1.0f/this->x;
			}
			if(this->y != 0.0f)
			{
				Out.y = 1.0f/this->y;
			}
            
            return Out;
        }
        void CVector2::Multiply(const CVector2* inpVec, const CVector2* inpVec2, CVector2* outpVec)
        {
            outpVec->x = inpVec->x * inpVec2->x; 
            outpVec->y = inpVec->y * inpVec2->y; 
        }
	}
}
