/**
@file	snMath.h
@brief	ベクトルや行列の計算
@author	syuki nishida
@date	2014/08/21
*/
#pragma once
#ifndef _SN_MATH_H_
#define _SN_MATH_H_

namespace snStudy{
	const float Pi = 3.141592654f;
	/// Struct

	struct Vector4f;

	/////////////////////////////
	// Vector
	typedef struct  Vector2f{
		float x, y;

		Vector2f(){};
		Vector2f(const float &f);
		Vector2f(const float &x, const float &y);
	}Vector2f;

	struct Vector3f{
		float x, y, z;

		Vector3f(){};
		Vector3f(const float &f);
		Vector3f(const float &x, const float &y, const float &z);
		Vector3f(const Vector4f &vec);

		float Length();
		Vector3f Scale(float f);
		Vector3f Normal();
		Vector3f Cross(Vector3f lVal, Vector3f rVal);
		float CrossXZ(Vector3f Val);
		float Dot(Vector3f lVal, Vector3f rVal);
		Vector3f Lerp(Vector3f end, float per);

		Vector3f operator + (const Vector3f &val) const;
		Vector3f operator - (const Vector3f &val) const;
		Vector3f operator * (const float &val) const;
		Vector3f& operator += (const Vector3f &val){
			x += val.x;
			y += val.y;
			z += val.z;
			return *this;
		}
		Vector3f& operator *= (const Vector3f &val){
			x *= val.x;
			y *= val.y;
			z *= val.z;
			return *this;
		}

#ifdef __D3DX9MATH_H__
		Vector3f(D3DXVECTOR3 vec){
			x = vec.x;
			y = vec.y;
			z = vec.z;
		};

		operator D3DXVECTOR3 (){
			D3DVECTOR ret;
			ret.x = x;
			ret.y = y;
			ret.z = z;
			return ret;
		}

#endif

#ifdef D3DVECTOR_DEFINED

		operator D3DVECTOR (){
			D3DVECTOR ret;
			ret.x = x;
			ret.y = y;
			ret.z = z;
			return ret;
		}
#endif
	};

	struct Vector3us{
		unsigned short x,y,z;
	};

	struct Vector3d{
		double x,y,z;
		Vector3d(){};
		Vector3d(double x,double y,double z);
	};


	struct Vector4f{
		float x, y, z, w;
	};

	/////////////////////////////////
	// Matrix
	struct Matrix44f{
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;

		Matrix44f(){};
		Matrix44f(const float &f);
		Matrix44f(const float &_11, const float &_12, const float &_13, const float &_14,
			const float &_21, const float &_22, const float &_23, const float &_24,
			const float &_31, const float &_32, const float &_33, const float &_34,
			const float &_41, const float &_42, const float &_43, const float &_44);

		Matrix44f Identity();
		Matrix44f EyeAtL(Vector3f eye, Vector3f at, Vector3f up);
		Matrix44f OrthoL(float w, float h, float zn, float zf);
		Matrix44f OrthoScreenL(float w, float h, float zn, float zf);
		Matrix44f PerthL(float w, float h, float zn, float zf);
		Matrix44f PerthFovL(float fovY, float aspect, float zn, float zf);
		Matrix44f Translation(float x, float y, float z);
		Matrix44f RotatinX(float angle);
		Matrix44f RotatinY(float angle);
		Matrix44f RotatinZ(float angle);

		Matrix44f operator * (const Matrix44f &Val) const{
			Matrix44f ret;
			ret._11 = (_11 * Val._11) + (_12 * Val._21) + (_13 * Val._31) + (_14 * Val._41);
			ret._12 = (_11 * Val._12) + (_12 * Val._22) + (_13 * Val._32) + (_14 * Val._42);
			ret._13 = (_11 * Val._13) + (_12 * Val._23) + (_13 * Val._33) + (_14 * Val._43);
			ret._14 = (_11 * Val._14) + (_12 * Val._24) + (_13 * Val._34) + (_14 * Val._44);

			ret._21 = (_21 * Val._11) + (_22 * Val._21) + (_23 * Val._31) + (_24 * Val._41);
			ret._22 = (_21 * Val._12) + (_22 * Val._22) + (_23 * Val._32) + (_24 * Val._42);
			ret._23 = (_21 * Val._13) + (_22 * Val._23) + (_23 * Val._33) + (_24 * Val._43);
			ret._24 = (_21 * Val._14) + (_22 * Val._24) + (_23 * Val._34) + (_24 * Val._44);

			ret._31 = (_31 * Val._11) + (_32 * Val._21) + (_33 * Val._31) + (_34 * Val._41);
			ret._32 = (_31 * Val._12) + (_32 * Val._22) + (_33 * Val._32) + (_34 * Val._42);
			ret._33 = (_31 * Val._13) + (_32 * Val._23) + (_33 * Val._33) + (_34 * Val._43);
			ret._34 = (_31 * Val._14) + (_32 * Val._24) + (_33 * Val._34) + (_34 * Val._44);

			ret._41 = (_41 * Val._11) + (_42 * Val._21) + (_43 * Val._31) + (_44 * Val._41);
			ret._42 = (_41 * Val._12) + (_42 * Val._22) + (_43 * Val._32) + (_44 * Val._42);
			ret._43 = (_41 * Val._13) + (_42 * Val._23) + (_43 * Val._33) + (_44 * Val._43);
			ret._44 = (_41 * Val._14) + (_42 * Val._24) + (_43 * Val._34) + (_44 * Val._44);
			return ret;
		}

#ifdef __D3DX9MATH_H__
		operator D3DMATRIX (){
			D3DMATRIX ret;
			ret._11 = _11; ret._12 = _12; ret._13 = _13; ret._14 = _14;
			ret._21 = _21; ret._22 = _22; ret._23 = _23; ret._24 = _24;
			ret._31 = _31; ret._32 = _32; ret._33 = _33; ret._34 = _34;
			ret._41 = _41; ret._42 = _42; ret._43 = _43; ret._44 = _44;

			return ret;
		}
		Matrix44f(D3DMATRIX mtx){
			_11 = mtx._11; _12 = mtx._12; _13 = mtx._13; _14 = mtx._14;
			_21 = mtx._21; _22 = mtx._22; _23 = mtx._23; _24 = mtx._24;
			_31 = mtx._31; _32 = mtx._32; _33 = mtx._33; _34 = mtx._34;
			_41 = mtx._41; _42 = mtx._42; _43 = mtx._43; _44 = mtx._44;
		}
#endif
	};

	/////////////////////////////////
	// Color
	struct Color1ul{
		unsigned long color;

		Color1ul(){};
		Color1ul(const unsigned long &Color){
			color = Color;
		}
	};


	struct Color4f{
		float r, g, b, a;

		Color4f(){};
		Color4f(const float &f){
			r = g = b = a = f;
		}
		Color4f(const float &r, const float &g, const float &b, const float &a);


		operator unsigned long(){
			unsigned long dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (unsigned long)(r * 255.0f + 0.5f);
			unsigned long dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (unsigned long)(g * 255.0f + 0.5f);
			unsigned long dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (unsigned long)(b * 255.0f + 0.5f);
			unsigned long dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (unsigned long)(a * 255.0f + 0.5f);

			return (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
		}

#ifdef D3DCOLORVALUE_DEFINED
		operator D3DCOLORVALUE (){
			D3DCOLORVALUE ret;
			ret.r = r;
			ret.g = g;
			ret.b = b;
			ret.a = a;
			return ret;
		}
#endif
	};

	float Lerp(float s, float e, float per);

}

#include "snMath.inl"

#endif // _SN_MATH_H_
//=EOF=