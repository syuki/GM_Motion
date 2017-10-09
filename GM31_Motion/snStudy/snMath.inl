/**
@file	snMath.inl
@brief	ベクトルや行列の計算
@author	syuki nishida
@date	2014/08/21
*/
#pragma once
#ifndef _SN_MATH_INL_
#define _SN_MATH_INL_

/// Macros
#define INLINE_LEVEL	__forceinline

/// 
namespace snStudy{

/// Vector2f
INLINE_LEVEL
Vector2f::Vector2f(const float &f){
	x = y = f;
}

INLINE_LEVEL
Vector2f::Vector2f(const float &fx, const float &fy){
	x = fx; y = fy;
}

/// Vector3f
INLINE_LEVEL
Vector3f::Vector3f(const float &f){
	x = y = z = f;
}

INLINE_LEVEL
Vector3f::Vector3f(const float &fx, const float &fy, const float &fz){
	x = fx;
	y = fy;
	z = fz;
}

INLINE_LEVEL
Vector3f::Vector3f(const Vector4f &vec){
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

INLINE_LEVEL float
Vector3f::Length(){
	return sqrt(x*x + y*y + z*z);
}

INLINE_LEVEL Vector3f
Vector3f::Scale(float f){
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

INLINE_LEVEL Vector3f
Vector3f::Normal(){
	return Scale(1.0f / Length());
}

INLINE_LEVEL Vector3f
Vector3f::Cross(Vector3f lVal, Vector3f rVal){
	x = (lVal.y * rVal.z - lVal.z * rVal.y);
	y = (lVal.z * rVal.x - lVal.x * rVal.z);
	z = (lVal.x * rVal.y - lVal.y * rVal.x);
	return *this;
}

INLINE_LEVEL float
Vector3f::CrossXZ(Vector3f Val){
	return x * Val.z - z * Val.x;
}

INLINE_LEVEL float
Vector3f::Dot(Vector3f lVal, Vector3f rVal){
	return ((lVal.x)* (rVal.x) + (lVal.y) * (rVal.y) + (lVal.z) * (rVal.z));
}

INLINE_LEVEL Vector3f
Vector3f::Lerp(Vector3f end, float per){
	Vector3f ret;

	ret.x = snStudy::Lerp(x, end.x, per);
	ret.y = snStudy::Lerp(y, end.y, per);
	ret.z = snStudy::Lerp(z, end.z, per);

	return ret;
}

INLINE_LEVEL Vector3f
Vector3f::operator-(const Vector3f &val) const{
	Vector3f ret;
	ret.x = x - val.x;
	ret.y = y - val.y;
	ret.z = z - val.z;
	return ret;
}

INLINE_LEVEL Vector3f
Vector3f::operator+(const Vector3f &val) const{
	Vector3f ret;
	ret.x = x + val.x;
	ret.y = y + val.y;
	ret.z = z + val.z;
	return ret;
}

INLINE_LEVEL Vector3f
Vector3f::operator*(const float &val) const{
	Vector3f ret;
	ret.x = x * val;
	ret.y = y * val;
	ret.z = z * val;
	return ret;
}

INLINE_LEVEL
Vector3d::Vector3d(double dx,double dy,double dz){
	x = dx;
	y = dy;
	z = dz;
}

/// Matrix44f
INLINE_LEVEL
Matrix44f::Matrix44f(const float &f){
	_11 = _12 = _13 = _14 =
	_21 = _22 = _23 = _24 =
	_31 = _32 = _33 = _34 =
	_41 = _42 = _43 = _44 = f;
}

INLINE_LEVEL
Matrix44f::Matrix44f(const float &f_11, const float &f_12, const float &f_13, const float &f_14,
const float &f_21, const float &f_22, const float &f_23, const float &f_24,
const float &f_31, const float &f_32, const float &f_33, const float &f_34,
const float &f_41, const float &f_42, const float &f_43, const float &f_44){
	_11 = f_11; _12 = f_12; _13 = f_13; _14 = f_14;
	_21 = f_21; _22 = f_22; _23 = f_23; _24 = f_24;
	_31 = f_31; _32 = f_32; _33 = f_33; _34 = f_34;
	_41 = f_41; _42 = f_42; _43 = f_43; _44 = f_44;
}

/**
@brief	単位行列化
@param	none
@return matrix
*/
INLINE_LEVEL Matrix44f
Matrix44f::Identity(){
	*this = Matrix44f(0);
	_11 = _22 = _33 = _44 = 1;
	return *this;
};

/**
@brief	左手座標のビュー行列
@param	視点、注視点、上方向
@return	ビュー行列
*/
INLINE_LEVEL Matrix44f
Matrix44f::EyeAtL(Vector3f eye, Vector3f at, Vector3f up){

	Vector3f zaxis = Vector3f(at - eye).Normal();
	Vector3f xaxis = Vector3f().Cross(up, zaxis).Normal();
	Vector3f yaxis = Vector3f().Cross(zaxis, xaxis);

	*this = Matrix44f(
	xaxis.x, yaxis.x, zaxis.x, 0,
	xaxis.y, yaxis.y, zaxis.y, 0,
	xaxis.z, yaxis.z, zaxis.z, 0,
	-Vector3f().Dot(xaxis, eye), -Vector3f().Dot(yaxis, eye), -Vector3f().Dot(zaxis, eye), 1);

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::OrthoL(float w, float h, float zn, float zf){
	*this = Matrix44f(0);
	_11 = 2 / w;
	_22 = 2 / h;
	_33 = 1 / (zf - zn);
	_43 = zn / (zn - zf);
	_44 = 1;

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::OrthoScreenL(float w, float h, float zn, float zf){
	*this = Matrix44f(0);
	_11 = 2 / w;
	_22 = 2 / -h;
	_33 = 1 / (zf - zn);
	_41 = -1;
	_42 = 1;
	_43 = zn / (zn - zf);
	_44 = 1;

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::PerthL(float w, float h, float zn, float zf){
	*this = Matrix44f(0);
	_11 = 2 * zn / w;
	_22 = 2 * zn / h;
	_33 = zf / (zf - zn);
	_34 = 1;
	_43 = zn*zf / (zn - zf);

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::PerthFovL(float fovY, float aspect, float zn, float zf){
	*this = Matrix44f(0);
	_22 = 1.f/tanf(fovY/2.f);
	_11 = _22 / aspect;
	_33 = zf / (zf - zn);
	_34 = 1;
	_43 = zn*zf / (zn - zf);

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::Translation(float x, float y, float z){
	Matrix44f work;
	work.Identity();
	work._41 = x;
	work._42 = y;
	work._43 = z;

	*this = *this * work;

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::RotatinX(float angle){
	Matrix44f work;
	work.Identity();

	work._23 = work._32 = sinf(angle);
	work._22 = work._33 = cosf(angle);
	work._32 *= -1;

	*this = *this * work;

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::RotatinY(float angle){
	Matrix44f work;
	work.Identity();

	work._11 = work._33 = cosf(angle);
	work._13 = work._31 = sinf(angle);
	work._13 *= -1;

	*this = *this * work;

	return *this;
}

INLINE_LEVEL Matrix44f
Matrix44f::RotatinZ(float angle){
	Matrix44f work;
	work.Identity();

	work._11 = work._22 = cosf(angle);
	work._12 = work._21 = sinf(angle);
	work._21 *= -1;

	*this = *this * work;

	return *this;
}


/// Color
INLINE_LEVEL
Color4f::Color4f(const float &fr, const float &fg, const float &fb, const float &fa){
	r = fr;
	g = fg;
	b = fb;
	a = fa;
}

INLINE_LEVEL float
Lerp(float s, float e, float per){
	return (e * per) + (s * (1 - per));
}

}

#endif // _SN_MATH_INL_
//=EOF=