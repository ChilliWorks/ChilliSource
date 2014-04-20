#include <ChilliSource/Core/Math/Vector2Old.h>
#include <ChilliSource/Core/Math/Vector3Old.h>
#include <ChilliSource/Core/Math/Vector4Old.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource 
{
	namespace Core 
	{
		const Vector3Old Vector3Old::ZERO;
        const Vector3Old Vector3Old::ONE(1.0f, 1.0f, 1.0f);
		const Vector3Old Vector3Old::X_UNIT_POSITIVE(1.0f,0,0);
		const Vector3Old Vector3Old::X_UNIT_NEGATIVE(-1.0f,0,0);
		const Vector3Old Vector3Old::Y_UNIT_POSITIVE(0,1.0f,0);
		const Vector3Old Vector3Old::Y_UNIT_NEGATIVE(0,-1.0f,0);
		const Vector3Old Vector3Old::Z_UNIT_POSITIVE(0,0,1.0f);
		const Vector3Old Vector3Old::Z_UNIT_NEGATIVE(0,0,-1.0f);
		
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise to zero
		//---------------------------------------------------------
		Vector3Old::Vector3Old(void) : x(0.0f), y(0.0f), z(0.0f)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector3Old::Vector3Old(const Vector3Old &Vec)
		{
			(*this) = Vec;
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy and ignore w component
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector3Old::Vector3Old(const Vector4Old &Vec) : x(Vec.x), y(Vec.y), z(Vec.z)
		{
			
		}
		Vector3Old::Vector3Old(const Vector2Old &Vec2)
		: x(Vec2.x), y(Vec2.y), z(0)
		{
			
		}

		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy and ignore z component
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector3Old::Vector3Old(const Vector2Old &Vec2, float fZ)
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
		Vector3Old::Vector3Old(const f32 fX, const f32 fY, const f32 fZ) : x(fX), y(fY), z(fZ)
		{
		}
		//---------------------------------------------------------
		/// Vector3Old - Cross Product
		///
		/// Returns the perpendicular vector to the plane
		/// described by the two input vectors
		///
		/// @param The adjoining vector
		/// @param The vector product result of the calculation
		//---------------------------------------------------------
		void Vector3Old::CrossProduct(const Vector3Old &Vec, Vector3Old &Out)
		{
            CrossProduct(this, &Vec, &Out);
		}
		//---------------------------------------------------------
		/// Vector3Old - Cross Product Copy
		///
		/// Returns the perpendicular vector to the plane
		/// described by the two input vectors
		///
		/// @param The adjoining vector
		/// @return The vector product result of the calculation
		//---------------------------------------------------------
		Vector3Old Vector3Old::CrossProduct(const Vector3Old &Vec) const
		{
			Vector3Old vResult;
			CrossProduct(this, &Vec, &vResult);
            
			return vResult;
		}
		//---------------------------------------------------------
		/// Vector3Old - Normalise
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
		Vector3Old & Vector3Old::Normalise()
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
		/// Vector3Old - Normalised Copy
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
		Vector3Old Vector3Old::NormalisedCopy() const
		{
			Vector3Old Out;
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
		/// Vector3Old - Inversed 
		///
		/// @param A vector object that will be the inverse of this
		//---------------------------------------------------------
		Vector3Old Vector3Old::InversedCopy() const
		{
			Vector3Old Out;
			
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
		/// Vector3Old - Inversed 
		///
		/// @param A vector object that will be the inverse of this
		//---------------------------------------------------------
		Vector3Old & Vector3Old::Inverse()
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
		/// Vector3Old - Dot Product
		///
		/// @param The adjoining vector
		/// @return The scalar product between two vectors
		//---------------------------------------------------------
		f32 Vector3Old::DotProduct(const Vector3Old &Vec) const
		{
			return DotProduct(this, &Vec);
		}
		//---------------------------------------------------------
		/// Vector3Old - Length Squared
		///
		/// Calculate the  squared magnitude of a vector
		/// no costly sqrt function
		/// @return The magnitude of the vector squared
		//---------------------------------------------------------
		f32 Vector3Old::LengthSquared() const
		{
			return (this->x * this->x + this->y * this->y + this->z * this->z);
		}
		//---------------------------------------------------------
		/// Vector3Old - Length
		///
		/// Calculate the magnitude of a vector
		/// @return The magnitude of the vector
		//---------------------------------------------------------
		f32 Vector3Old::Length() const
		{
			//Calculate Magnitude
			return (f32)sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		}
		//---------------------------------------------------------
		/// Vector3Old - Angle Between Two Vectors
		///
		/// Using the Dot Product
		/// @param The vector to which we find the angle
		/// @return The angle between two given vectors in radians
		//---------------------------------------------------------
		f32 Vector3Old::Angle(const Vector3Old &Vec) const
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
        void Vector3Old::Multiply(const Vector3Old* inpVec, const Vector3Old* inpVec2, Vector3Old* outpVec)
        {
            outpVec->x = inpVec->x * inpVec2->x; 
            outpVec->y = inpVec->y * inpVec2->y; 
            outpVec->z = inpVec->z * inpVec2->z;
        }
        void Vector3Old::CrossProduct(const Vector3Old* inpVec, const Vector3Old* inpVec2, Vector3Old* outpVec)
        {
            outpVec->x = (inpVec->y * inpVec2->z - inpVec->z * inpVec2->y);
			outpVec->y = (inpVec->z * inpVec2->x - inpVec->x * inpVec2->z);
			outpVec->z = (inpVec->x * inpVec2->y - inpVec->y * inpVec2->x);
        }
        f32 Vector3Old::DotProduct(const Vector3Old* inpVec, const Vector3Old* inpVec2)
        {
            return (inpVec->x * inpVec2->x) + (inpVec->y * inpVec2->y) + (inpVec->z * inpVec2->z);
        }
        
		Vector3Old operator*(const f32 Scale, const Vector3Old & Vec)
		{Vector3Old Result; Result.x = Vec.x * Scale; Result.y = Vec.y * Scale; Result.z = Vec.z * Scale; return Result;}
	}
}
