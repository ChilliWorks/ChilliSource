#include <moFlo/Core/Math/Vector4.h>
#include <moFlo/Core/Math/MathUtils.h>

namespace moFlo 
{
	namespace Core 
	{
		const CVector4 CVector4::ZERO;
		
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise to zero
		//---------------------------------------------------------
		CVector4::CVector4(void) : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
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
		CVector4::CVector4(const f32 fX, const f32 fY, const f32 fZ, const f32 fW) : x(fX), y(fY), z(fZ), w(fW)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector4::CVector4(const CVector4 &Vec)
		{
			(*this) = Vec;
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy and ignore w component
		/// @param Vector to copy
		//---------------------------------------------------------
		CVector4::CVector4(const CVector3 &Vec, f32 inW) : x(Vec.x), y(Vec.y), z(Vec.z), w(inW)
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
		void CVector4::CrossProduct(const CVector4 &Vec, CVector4 &Out)
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
		CVector4 CVector4::CrossProduct(const CVector4 &Vec, const CVector4 &Vec2)
		{
            CVector4 Out;
            
            CrossProduct(&Vec, &Vec2, &Out);
            
            return Out;
		}
		//---------------------------------------------------------
		/// Vector4 - Normalise
		///
		/// Normalises this vector
		///  
		//---------------------------------------------------------
        CVector4 & CVector4::Normalise()
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
		CVector4 CVector4::NormalisedCopy() const 
		{
			CVector4 Result;
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
		f32 CVector4::LengthSquared() const
		{
			return (this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}
		//---------------------------------------------------------
		/// Vector4 - Length
		///
		/// Calculate the magnitude of a vector
		/// @return The magnitude of the vector
		//---------------------------------------------------------
		f32 CVector4::Length() const
		{
			//Calculate Magnitude
			return (f32)sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
		}
        CVector4 CVector4::InversedCopy() const
        {
            CVector4 Out;
			
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
        CVector4 & CVector4::Inverse()
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
        void CVector4::Multiply(const CVector4* inpVec, const CVector4* inpVec2, CVector4* outpVec)
        {
            outpVec->x = inpVec->x * inpVec2->x; 
            outpVec->y = inpVec->y * inpVec2->y; 
            outpVec->z = inpVec->z * inpVec2->z;
            outpVec->w = inpVec->w * inpVec2->w;
        }
        void CVector4::CrossProduct(const CVector4* inpVec, const CVector4* inpVec2, CVector4* outpVec)
        {
            outpVec->x = (inpVec->y * inpVec2->z - inpVec->z * inpVec2->y);
			outpVec->y = (inpVec->z * inpVec2->x - inpVec->x * inpVec2->z);
			outpVec->z = (inpVec->x * inpVec2->y - inpVec->y * inpVec2->x);
            outpVec->w = 1.0f;
        }
	}
}
