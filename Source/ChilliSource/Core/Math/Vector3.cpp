#include <moFlo/Core/Math/Vector2.h>
#include <moFlo/Core/Math/Vector3.h>
#include <moFlo/Core/Math/Vector4.h>
#include <moFlo/Core/Math/MathUtils.h>

namespace moFlo 
{
	namespace Core 
	{
		const CVector3 CVector3::ZERO;
        const CVector3 CVector3::ONE(1.0f, 1.0f, 1.0f);
		const CVector3 CVector3::X_UNIT_POSITIVE(1.0f,0,0);
		const CVector3 CVector3::X_UNIT_NEGATIVE(-1.0f,0,0);
		const CVector3 CVector3::Y_UNIT_POSITIVE(0,1.0f,0);
		const CVector3 CVector3::Y_UNIT_NEGATIVE(0,-1.0f,0);
		const CVector3 CVector3::Z_UNIT_POSITIVE(0,0,1.0f);
		const CVector3 CVector3::Z_UNIT_NEGATIVE(0,0,-1.0f);
		
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise to zero
		//---------------------------------------------------------
		CVector3::CVector3(void) : x(0.0f), y(0.0f), z(0.0f)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector3::CVector3(const CVector3 &Vec)
		{
			(*this) = Vec;
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy and ignore w component
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector3::CVector3(const CVector4 &Vec) : x(Vec.x), y(Vec.y), z(Vec.z)
		{
			
		}
		CVector3::CVector3(const CVector2 &Vec2)
		: x(Vec2.x), y(Vec2.y), z(0)
		{
			
		}

		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy and ignore z component
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector3::CVector3(const CVector2 &Vec2, float fZ)
		: x(Vec2.x), y(Vec2.y), z(fZ)
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
		CVector3::CVector3(const f32 fX, const f32 fY, const f32 fZ) : x(fX), y(fY), z(fZ)
		{
		}
		//---------------------------------------------------------
		/// Vector3 - Cross Product
		///
		/// Returns the perpendicular vector to the plane
		/// described by the two input vectors
		///
		/// @param The adjoining vector
		/// @param The vector product result of the calculation
		//---------------------------------------------------------
		void CVector3::CrossProduct(const CVector3 &Vec, CVector3 &Out)
		{
            CrossProduct(this, &Vec, &Out);
		}
		//---------------------------------------------------------
		/// Vector3 - Cross Product Copy
		///
		/// Returns the perpendicular vector to the plane
		/// described by the two input vectors
		///
		/// @param The adjoining vector
		/// @return The vector product result of the calculation
		//---------------------------------------------------------
		CVector3 CVector3::CrossProduct(const CVector3 &Vec) const
		{
			CVector3 vResult;
			CrossProduct(this, &Vec, &vResult);
            
			return vResult;
		}
		//---------------------------------------------------------
		/// Vector3 - Normalise
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
		CVector3 & CVector3::Normalise()
		{
			//Calculate Magnitude
			f32 fMag = Length();
			//Prevent division by zero
			if(fMag == 0) fMag = 1; 
			//Return Unit Vector
			this->x/=fMag;
			this->y/=fMag;
			this->z/=fMag;
            
            return *this;
		}
		//---------------------------------------------------------
		/// Vector3 - Normalised Copy
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
		CVector3 CVector3::NormalisedCopy() const
		{
			CVector3 Out;
			//Calculate Magnitude
			f32 fMag = Length();
			//Prevent division by zero
			if(fMag == 0) fMag = 1; 
			//Return Unit Vector
			Out.x = this->x/fMag;
			Out.y = this->y/fMag;
			Out.z = this->z/fMag;
			
			return Out;
		}
		//---------------------------------------------------------
		/// Vector3 - Inversed 
		///
		/// @param A vector object that will be the inverse of this
		//---------------------------------------------------------
		CVector3 CVector3::InversedCopy() const
		{
			CVector3 Out;
			
			if(this->x != 0.0f)
			{
				Out.x = 1.0f/this->x;
			}
			if(this->y != 0.0f)
			{
				Out.y = 1.0f/this->y;
			}
			if(this->z != 0.0f)
			{
				Out.z = 1.0f/this->z;
			}

			return Out;
		}
        //---------------------------------------------------------
		/// Vector3 - Inversed 
		///
		/// @param A vector object that will be the inverse of this
		//---------------------------------------------------------
		CVector3 & CVector3::Inverse()
		{
			if(this->x != 0.0f)
			{
				this->x = 1.0f/this->x;
			}
			if(this->y != 0.0f)
			{
				this->y = 1.0f/this->y;
			}
			if(this->z != 0.0f)
			{
				this->z = 1.0f/this->z;
			}
            
            return *this;
		}
		//---------------------------------------------------------
		/// Vector3 - Dot Product
		///
		/// @param The adjoining vector
		/// @return The scalar product between two vectors
		//---------------------------------------------------------
		f32 CVector3::DotProduct(const CVector3 &Vec) const
		{
			return DotProduct(this, &Vec);
		}
		//---------------------------------------------------------
		/// Vector3 - Length Squared
		///
		/// Calculate the  squared magnitude of a vector
		/// no costly sqrt function
		/// @return The magnitude of the vector squared
		//---------------------------------------------------------
		f32 CVector3::LengthSquared() const
		{
			return (this->x * this->x + this->y * this->y + this->z * this->z);
		}
		//---------------------------------------------------------
		/// Vector3 - Length
		///
		/// Calculate the magnitude of a vector
		/// @return The magnitude of the vector
		//---------------------------------------------------------
		f32 CVector3::Length() const
		{
			//Calculate Magnitude
			return (f32)sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		}
		//---------------------------------------------------------
		/// Vector3 - Angle Between Two Vectors
		///
		/// Using the Dot Product
		/// @param The vector to which we find the angle
		/// @return The angle between two given vectors in radians
		//---------------------------------------------------------
		f32 CVector3::Angle(const CVector3 &Vec) const
		{
			//a.b = |a||b|cos theta
			f32 fDot = DotProduct(Vec);
			f32 fTotMag = Length() * Vec.Length();

			//Prevent division by zero
			if(fTotMag == 0) 
				fTotMag = 1; 
            
			// This just caps the quotient because in practical use, sometimes fDot and fTotMag yield a very small number above 1.0, e.g. 1.00003 when angles tend towards 0 degree
			f32 fRatio = fDot / fTotMag;
			if (fRatio > 1.0f)
				fRatio = 1.0f;
			else
			{
				if (fRatio < -1.0f)
					fRatio = -1.0f;
			}
			
			//Calculate the angle using inverse cosine and converting to degrees
			return acosf(fRatio);
		}
        void CVector3::Multiply(const CVector3* inpVec, const CVector3* inpVec2, CVector3* outpVec)
        {
            outpVec->x = inpVec->x * inpVec2->x; 
            outpVec->y = inpVec->y * inpVec2->y; 
            outpVec->z = inpVec->z * inpVec2->z;
        }
        void CVector3::CrossProduct(const CVector3* inpVec, const CVector3* inpVec2, CVector3* outpVec)
        {
            outpVec->x = (inpVec->y * inpVec2->z - inpVec->z * inpVec2->y);
			outpVec->y = (inpVec->z * inpVec2->x - inpVec->x * inpVec2->z);
			outpVec->z = (inpVec->x * inpVec2->y - inpVec->y * inpVec2->x);
        }
        f32 CVector3::DotProduct(const CVector3* inpVec, const CVector3* inpVec2)
        {
            return (inpVec->x * inpVec2->x) + (inpVec->y * inpVec2->y) + (inpVec->z * inpVec2->z);
        }
        
		CVector3 operator*(const f32 Scale, const CVector3 & Vec)
		{CVector3 Result; Result.x = Vec.x * Scale; Result.y = Vec.y * Scale; Result.z = Vec.z * Scale; return Result;}
	}
}
