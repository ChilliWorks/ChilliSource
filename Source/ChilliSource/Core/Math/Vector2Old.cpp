#include <ChilliSource/Core/Math/Vector2Old.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource 
{
	namespace Core 
	{
		const Vector2Old Vector2Old::ZERO;
		const Vector2Old Vector2Old::ONE(1.0f,1.0f);
		const Vector2Old Vector2Old::X_UNIT_POSITIVE(1.0f,0);
		const Vector2Old Vector2Old::X_UNIT_NEGATIVE(-1.0f,0);
		const Vector2Old Vector2Old::Y_UNIT_POSITIVE(0,1.0f);
		const Vector2Old Vector2Old::Y_UNIT_NEGATIVE(0,-1.0f);
		//---------------------------------------------------------
		/// Constructor
		///
		/// Default. Initialises to zero
		//---------------------------------------------------------
		Vector2Old::Vector2Old() : x(0.0f), y(0.0f)
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
		Vector2Old::Vector2Old(const f32 fX, const f32 fY) : x(fX), y(fY)
		{
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector2Old::Vector2Old(const Vector2Old &Vec)
		{
			(*this) = Vec;
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector2Old::Vector2Old(const Vector3Old &Vec) : x(Vec.x), y(Vec.y)
		{
			
		}
		//---------------------------------------------------------
		/// Constructor
		///
		/// Initialise with copy
		/// @param Vector to copy
		//---------------------------------------------------------
		Vector2Old::Vector2Old(const Vector4Old &Vec) : x(Vec.x), y(Vec.y)
		{
			
		}
		//---------------------------------------------------------
		/// Vector2Old - Normalise
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
        Vector2Old & Vector2Old::Normalise()
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
		/// Vector2Old - NormalisedCopy
		///
		/// Takes a vector as a parameter and converts it to
		/// unit length.
		/// @param A vector object that will be set to the unit 
		//---------------------------------------------------------
		Vector2Old Vector2Old::NormalisedCopy() const
		{
			Vector2Old Out;
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
		/// Vector2Old - Dot Product
		///
		/// Return the scalar product between two vectors
		/// @param The adjoining vector
		/// @return The scalar product
		//---------------------------------------------------------
		f32 Vector2Old::DotProduct(const Vector2Old &inOtherVec) const
		{
			return (this->x * inOtherVec.x) + (this->y * inOtherVec.y);
		}
		//---------------------------------------------------------
		/// Vector2Old - Length Squared
		///
		/// Calculate the  squared magnitude of a vector
		/// no costly sqrt function
		/// @return The magnitude squared
		//---------------------------------------------------------
		f32 Vector2Old::LengthSquared() const
		{
			return (this->x * this->x + this->y * this->y);
		}
		//---------------------------------------------------------
		/// Vector2Old - Length
		///
		/// Calculate the magnitude of a vector
		/// @return The magnitude of the vector
		//---------------------------------------------------------
		f32 Vector2Old::Length() const
		{
			//Calculate Magnitude
			return (f32)sqrt(this->x * this->x + this->y * this->y);
		}
		//---------------------------------------------------------
		/// Vector2Old - Angle Between Two Vectors
		///
		/// Using the Dot Product
		/// 
		/// @param The vector to which the angle is defined
		/// @return The angle in radians
		//---------------------------------------------------------
		f32 Vector2Old::Angle(const Vector2Old &inOtherVec) const
		{			
			return std::atan2(inOtherVec.y,inOtherVec.x) - std::atan2(y,x);
		}
        Vector2Old & Vector2Old::Inverse()
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
        Vector2Old Vector2Old::InversedCopy() const
        {
            Vector2Old Out;
            
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
        void Vector2Old::Multiply(const Vector2Old* inpVec, const Vector2Old* inpVec2, Vector2Old* outpVec)
        {
            outpVec->x = inpVec->x * inpVec2->x; 
            outpVec->y = inpVec->y * inpVec2->y; 
        }
	}
}
