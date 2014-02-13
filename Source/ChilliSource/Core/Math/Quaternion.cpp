
#include <moFlo/Core/Math/Quaternion.h>
#include <moFlo/Core/Math/MathUtils.h>

using namespace moFlo::Core;

const CQuaternion CQuaternion::IDENTITY(0,0,0,1);
const CQuaternion CQuaternion::ZERO(0,0,0,0);

CQuaternion::CQuaternion()
:x(0),y(0),z(0),w(1)
{
	
}

CQuaternion::CQuaternion(const f32 _x, const f32 _y, const f32 _z, const f32 _w)
:w(_w),x(_x),y(_y),z(_z)
{
}

CQuaternion::CQuaternion(const CVector3 & V, f32 theta)
{
	SetAxisAngle(V,theta);
}
//-----------------------------------------------------------------------
CQuaternion::CQuaternion(const CVector3& xaxis, const CVector3& yaxis, const CVector3& zaxis)
{
	CMatrix4x4 matRot;
	
	matRot.m[0] = xaxis.x;
	matRot.m[1] = xaxis.y;
	matRot.m[2] = xaxis.z;
	
	matRot.m[4] = yaxis.x;
	matRot.m[5] = yaxis.y;
	matRot.m[6] = yaxis.z;
	
	matRot.m[8] = zaxis.x;
	matRot.m[9] = zaxis.y;
	matRot.m[10] = zaxis.z;
	
	(*this) = CQuaternion(matRot);
}
f32 CQuaternion::GetAngle() const {
	f32 fSqrLength = (x * x + y * y + z * z);

	if (fSqrLength > 0){
		return 2.0f * std::acos(w);
	} 

	return 0.0f;
}
CVector3 CQuaternion::GetAxis() const {
	f32 fSqrLength = (x * x + y * y + z * z);

	if (fSqrLength > 0){
		f32 fInvLength = 1.0f / std::sqrt(fSqrLength);
		return CVector3(x * fInvLength, y * fInvLength, z * fInvLength);
	} else {
		return CVector3::X_UNIT_POSITIVE;
	}
}

void CQuaternion::ToEulerAxes (CVector3& xaxis, CVector3& yaxis, CVector3& zaxis) const
{
	CMatrix4x4 matRot;
	
	ToRotationMatrix(matRot);
	
	xaxis.x = matRot(0, 0);
	xaxis.y = matRot(0, 1);
	xaxis.z = matRot(0, 2);
	
	yaxis.x = matRot(1, 0);
	yaxis.y = matRot(1, 1);
	yaxis.z = matRot(1, 2);
	
	zaxis.x = matRot(2, 0);
	zaxis.y = matRot(2, 1);
	zaxis.z = matRot(2, 2);
}
CQuaternion::CQuaternion(const CMatrix4x4& inMat)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".
	
	f32 fTrace = inMat.m[0]+inMat.m[5]+inMat.m[10];
	f32 fRoot = 0.0f;
	
	if (fTrace > 0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrtf(fTrace + 1.0f);  // 2w
		w = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;  // 1/(4w)
		x = (inMat.m[6]-inMat.m[9])*fRoot;
		y = (inMat.m[8]-inMat.m[2])*fRoot;
		z = (inMat.m[1]-inMat.m[4])*fRoot;
	}
	else
	{
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if ( inMat.m[5] > inMat.m[0] )
			i = 1;
		if ( inMat.m[10] > inMat(i,i) )
			i = 2;
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];
		
		fRoot = sqrtf(inMat(i,i)-inMat(j,j)-inMat(k,k) + 1.0f);
		
		f32* apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f/fRoot;
		w = (inMat(j,k)-inMat(k,j))*fRoot;
		*apkQuat[j] = (inMat(j,i)+inMat(i,j))*fRoot;
		*apkQuat[k] = (inMat(k,i)+inMat(i,k))*fRoot;
	}
}
void CQuaternion::SetAxisAngle(const CVector3 &V, f32 theta)
{
	CVector3 Vnorm = V.NormalisedCopy();

	f32 fHalfAngle = theta/2.0f;
	f32 fSinAngle = (f32)sin(fHalfAngle);
	x = Vnorm.x * fSinAngle;
	y = Vnorm.y * fSinAngle;
	z = Vnorm.z * fSinAngle;
	w = (f32)cos(fHalfAngle);
}
void CQuaternion::SetAxisAngle(f32 infX, f32 infY, f32 infZ, f32 theta)
{
	f32 fHalfAngle = theta/2.0f;
	f32 fSinAngle = (f32)sin(fHalfAngle);
	x = infX * fSinAngle;
	y = infY * fSinAngle;
	z = infZ * fSinAngle;
	w = (f32)cos(fHalfAngle);
}

CQuaternion CQuaternion::Slerp(const CQuaternion &q1, const CQuaternion &q2, float t, bool inbShortestPath) 
{
	CQuaternion result, _q2 = q2;
	
	float cosOmega = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
	
	if ( cosOmega < 0.0f && inbShortestPath ) 
	{
		_q2.x = -_q2.x;
		_q2.y = -_q2.y;
		_q2.z = -_q2.z;
		_q2.w = -_q2.w;
		cosOmega = -cosOmega;
	}
	
	float k0, k1;
	if ( cosOmega > 0.99999f ) {
		k0 = 1.0f - t;
		k1 = t;
		
		result.x = q1.x*k0 + _q2.x*k1;
		result.y = q1.y*k0 + _q2.y*k1;
		result.z = q1.z*k0 + _q2.z*k1;
		result.w = q1.w*k0 + _q2.w*k1;
		
		result.NormaliseSelf();		
	}
	else {
		float sinOmega = (float)sqrt( double(1.0f - cosOmega*cosOmega) );
		float omega = (float)atan2( double(sinOmega), double(cosOmega) );
		
		float invSinOmega = 1.0f/sinOmega;
		
		k0 = float( sin(double((1.0f - t)*omega)) )*invSinOmega;
		k1 = float( sin(double(t*omega)) )*invSinOmega;
		
		result.x = q1.x*k0 + _q2.x*k1;
		result.y = q1.y*k0 + _q2.y*k1;
		result.z = q1.z*k0 + _q2.z*k1;
		result.w = q1.w*k0 + _q2.w*k1;
	}
	return result;
}

CQuaternion CQuaternion::NLerp(const moFlo::Core::CQuaternion &q1, const moFlo::Core::CQuaternion &q2, float t, bool inbShortestPath)
{
    const f32 fDot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.x * q2.x + q1.w * q2.w;
    CQuaternion rVal;
    if(fDot < 0.0f && inbShortestPath)
        rVal = q1 + (t * ((q2 * -1.0f) - q1));
    else
        rVal = q1 + (t * (q2 -q1));
    
    rVal.NormaliseSelf();
    return rVal;
}

CQuaternion::~CQuaternion()
{
}

f32 CQuaternion::Modulus() const
{
    return sqrtf(w * w + x * x + y * y + z * z);
}

f32 CQuaternion::ModulusSquared() const
{
    return (w * w + x * x + y * y + z * z);
}

CQuaternion & CQuaternion::NormaliseSelf()
{
	f32 s = 1.0f / Modulus();
	w *= s;
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

CQuaternion CQuaternion::Conjugate() const
{
	CQuaternion quat;
	quat.x = -x;
	quat.y = -y;
	quat.z = -z;
	quat.w = w;
	return quat;
}

CQuaternion CQuaternion::Inverse() const
{
    return Conjugate() / ModulusSquared();
}

void CQuaternion::ToRotationMatrix(CMatrix4x4 & inMatrix) const
{
	f32 x2 = x * x;
	f32 y2 = y * y;
	f32 z2 = z * z;
	f32 xy = x * y;
	f32 xz = x * z;
	f32 yz = y * z;
	f32 wx = w * x;
	f32 wy = w * y;
	f32 wz = w * z;
	
	inMatrix = CMatrix4x4
	( 
	 1.0f - 2.0f * (y2 + z2),	2.0f * (xy + wz),			2.0f * (xz - wy),			0.0f,
	 2.0f * (xy - wz),			1.0f - 2.0f * (x2 + z2),	2.0f * (yz + wx),			0.0f,
	 2.0f * (xz + wy),			2.0f * (yz - wx),			1.0f - 2.0f * (x2 + y2),	0.0f,
	 0.0f,						0.0f,						0.0f,						1.0f
	 );
}

CMatrix4x4 CQuaternion::ToRotationMatrix4x4() const
{

	CMatrix4x4 Result;
	this->ToRotationMatrix(Result);

	return Result;
}