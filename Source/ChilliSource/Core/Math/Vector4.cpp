#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource 
{
	namespace Core 
	{
		const Vector4 Vector4::ZERO;
		
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise to zero
		//---------------------------------------------------------
		Vector4::Vector4(void) : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise individual components
		/// @param X component
		/// @param Y component
		/// @param Z component
		/// @param W component
		//---------------------------------------------------------
		Vector4::Vector4(const f32 fX, const f32 fY, const f32 fZ, const f32 fW) : x(fX), y(fY), z(fZ), w(fW)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector4::Vector4(const Vector4 &Vec)
		{
			(*this) = Vec;
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy and ignore w component
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector4::Vector4(const Vector3 &Vec, f32 inW) : x(Vec.x), y(Vec.y), z(Vec.z), w(inW)
		{
			
		}
		//---------------------------------------------------------
		/// Vector4 - Cross Product
		///
		/// Returns the perpendicular vector to the plane
		/// described by the two input vectors
		///
		/// @param The adjoining vector
		/// @param The vector product result of the calculation
		//---------------------------------------------------------
		void Vector4::CrossProduct(const Vector4 &Vec, Vector4 &Out)
		{
            CrossProduct(this, &Vec, &Out);
		}
        //---------------------------------------------------------
		/// Vector4 - Cross Product
		///
		/// Returns the perpendicular vector to the plane
		/// described by the two input vectors
		///
		/// @param The adjoining vector
		/// @param The vector product result of the calculation
		//---------------------------------------------------------
		Vector4 Vector4::CrossProduct(const Vector4 &Vec, const Vector4 &Vec2)
		{
            Vector4 Out;
            
            CrossProduct(&Vec, &Vec2, &Out);
            
            return Out;
		}
		//---------------------------------------------------------
		/// Vector4 - Normalise
		///
		/// Normalises this vector
		///  
		//---------------------------------------------------------
        Vector4 & Vector4::Normalise()
		{
			//Calculate Magnitude
			f32 fMag = Length();
			//Prevent division by zero
			if(fMag == 0) fMag = 1; 
			//Return Unit Vector
			this->x/=fMag;
			this->y/=fMag;
			this->z/=fMag;
			this->w/=fMag;
            
            return *this;
		}
		//---------------------------------------------------------
		/// Vector4 - Normalise Copy
		///
		/// @return Normalised copy of this vector
		///  
		//---------------------------------------------------------
		Vector4 Vector4::NormalisedCopy() const 
		{
			Vector4 Result;
			//Calculate Magnitude
			f32 fMag = Length();
			//Prevent division by zero
			if(fMag == 0) fMag = 1; 
			//Return Unit Vector
			Result.x = this->x/fMag;
			Result.y = this->y/fMag;
			Result.z = this->z/fMag;
			Result.w = this->w/fMag;
			
			return Result;
		}
		//---------------------------------------------------------
		/// Vector4 - Length Squared
		///
		/// Calculate the  squared magnitude of a vector
		/// no costly sqrt function
		/// @return The magnitude of the vector squared
		//---------------------------------------------------------
		f32 Vector4::LengthSquared() const
		{
			return (this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}
		//---------------------------------------------------------
		/// Vector4 - Length
		///
		/// Calculate the magnitude of a vector
		/// @return The magnitude of the vector
		//---------------------------------------------------------
		f32 Vector4::Length() const
		{
			//Calculate Magnitude
			return (f32)sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}
        Vector4 Vector4::InversedCopy() const
        {
            Vector4 Out;
			
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
            if(this->w != 0.0f)
			{
				Out.w = 1.0f/this->w;
			}
            
			return Out;
        }
        Vector4 & Vector4::Inverse()
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
            if(this->w != 0.0f)
			{
				this->w = 1.0f/this->w;
			}
            
            return *this;
        }
        void Vector4::Multiply(const Vector4* inpVec, const Vector4* inpVec2, Vector4* outpVec)
        {
            outpVec->x = inpVec->x * inpVec2->x; 
            outpVec->y = inpVec->y * inpVec2->y; 
            outpVec->z = inpVec->z * inpVec2->z;
            outpVec->w = inpVec->w * inpVec2->w;
        }
        void Vector4::CrossProduct(const Vector4* inpVec, const Vector4* inpVec2, Vector4* outpVec)
        {
            outpVec->x = (inpVec->y * inpVec2->z - inpVec->z * inpVec2->y);
			outpVec->y = (inpVec->z * inpVec2->x - inpVec->x * inpVec2->z);
			outpVec->z = (inpVec->x * inpVec2->y - inpVec->y * inpVec2->x);
            outpVec->w = 1.0f;
        }
	}
}
