#pragma once
#include "misc/helper.h"
#include "vector3.h"
#include "vector4.h"
namespace Colour
{

	//argb
	const u32 Red(0x00ff0000);
	const u32 Green(0x0000ff00);
	const u32 Blue(0x000000ff);
	const u32 White(0xffffffff);
	const u32 Black(0x00000000);
	inline u8 getAlphaChannelFromARGB(const u32 argb)
	{
		return ( argb >> 24 ) & 255;		
	}
	inline u8 getRedChannelFromARGB(const u32 argb)
	{
		return ( argb >> 16 ) & 255;		
	}
	inline u8 getGreenChannelFromARGB(const u32 argb)
	{
		return ( argb >> 8 ) & 255;		
	}
	inline u8 getBlueChannelFromARGB(const u32 argb)
	{
		return ( argb ) & 255;		
	}
	
	inline
		u32 getUint32( int r, int g, int b, int a = 0 )
	{
		r = r > 255 ? 255 : r < 0 ? 0 : r;
		g = g > 255 ? 255 : g < 0 ? 0 : g;
		b = b > 255 ? 255 : b < 0 ? 0 : b;
		a = a > 255 ? 255 : a < 0 ? 0 : a;

		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
	}

	inline
		u32 getUint32NoClamp( int r, int g, int b, int a = 0 )
	{
		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
	}

	inline
		u32 getUint32NoClamp( int* c )
	{
		return ( ( c[0] << 16 ) | ( c[1] << 8 ) | c[2] | ( c[3] << 24 ) );
	}

	inline
		u32 getUint32( const Vector3 &colour, int a )
	{
		int r = int(colour[ 0 ]);
		int g = int(colour[ 1 ]);
		int b = int(colour[ 2 ]);

		r = r > 255 ? 255 : r < 0 ? 0 : r;
		g = g > 255 ? 255 : g < 0 ? 0 : g;
		b = b > 255 ? 255 : b < 0 ? 0 : b;
		a = a > 255 ? 255 : a < 0 ? 0 : a;

		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
	}

	inline
		u32 getUint32NoClamp( const Vector3 &colour, int a )
	{
		return ( a << 24 ) | ( (u32)colour[ 0 ] << 16 ) | ( (u32)colour[ 1 ] << 8 ) | ( (u32)colour[ 2 ] );
	}

	inline
		u32 getUint32( const Vector3 &colour )
	{
		int r = int(colour[ 0 ]);
		int g = int(colour[ 1 ]);
		int b = int(colour[ 2 ]);

		r = r > 255 ? 255 : r < 0 ? 0 : r;
		g = g > 255 ? 255 : g < 0 ? 0 : g;
		b = b > 255 ? 255 : b < 0 ? 0 : b;

		return ( r << 16 ) | ( g << 8 ) | b;
	}

	inline
		u32 getUint32NoClamp( const Vector3 &colour )
	{
		return ( (u32)colour[ 0 ] << 16 ) | ( (u32)colour[ 1 ] << 8 ) | ( (u32)colour[ 2 ] );
	}

	inline
		u32 getUint32FromNormalised( const Vector3 &colour )
	{
		int r = int(colour[ 0 ] * 255.0f);
		int g = int(colour[ 1 ] * 255.0f);
		int b = int(colour[ 2 ] * 255.0f);

		r = r > 255 ? 255 : r < 0 ? 0 : r;
		g = g > 255 ? 255 : g < 0 ? 0 : g;
		b = b > 255 ? 255 : b < 0 ? 0 : b;

		return ( r << 16 ) | ( g << 8 ) | b;
	}

	inline
		u32 getUint32FromNormalisedNoClamp( const Vector3 &colour )
	{
		return ( (u32)( colour[ 0 ] * 255 ) << 16 ) | ( (u32)( colour[ 1 ] * 255 ) << 8 ) | ( (u32)( colour[ 2 ] * 255 ) );
	}

	inline
		Vector3 getVector3( u32 colour )
	{
		return Vector3( float((colour >> 16) & 255),
			float((colour >> 8)  & 255),
			float( colour        & 255));
	}

	inline
		Vector3 getVector3Normalised( u32 colour )
	{
		Vector3 v(	float((colour >> 16) & 255),
			float((colour >>  8) & 255),
			float( colour        & 255));
		v /= 255;
		return v;		
	}

	inline
		u32 getUint32( const Vector4 &colour )
	{
		int r = int(colour[ 0 ]);
		int g = int(colour[ 1 ]);
		int b = int(colour[ 2 ]);
		int a = int(colour[ 3 ]);

		r = r > 255 ? 255 : r < 0 ? 0 : r;
		g = g > 255 ? 255 : g < 0 ? 0 : g;
		b = b > 255 ? 255 : b < 0 ? 0 : b;
		a = a > 255 ? 255 : a < 0 ? 0 : a;

		return u32(( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b);
	}

	inline
		u32 getUint32NoClamp( const Vector4 &colour )
	{
		return ( (u32)colour[ 3 ] << 24 ) | ( (u32)colour[ 0 ] << 16 ) | ( (u32)colour[ 1 ] << 8 ) | ( (u32)colour[ 2 ] );
	}

	inline
		u32 getUint32FromNormalised( const Vector4 &colour )
	{
		int r = int(colour[ 0 ] * 255.0);
		int g = int(colour[ 1 ] * 255.0);
		int b = int(colour[ 2 ] * 255.0);
		int a = int(colour[ 3 ] * 255.0);

		r = r > 255 ? 255 : r < 0 ? 0 : r;
		g = g > 255 ? 255 : g < 0 ? 0 : g;
		b = b > 255 ? 255 : b < 0 ? 0 : b;
		a = a > 255 ? 255 : a < 0 ? 0 : a;

		return ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
	}

	inline
		u32 getUint32FromNormalisedNoClamp( const Vector4 &colour )
	{
		return ( (u32)( colour[ 0 ] * 255 ) << 16 ) | ( (u32)( colour[ 1 ] * 255 ) << 8 ) | ( (u32)( colour[ 2 ] * 255 ) );
	}

	inline
		Vector4 getVector4( u32 colour )
	{
		return Vector4(
			float( ( colour >> 16 ) & 255),
			float( ( colour >>  8 ) & 255),
			float(   colour         & 255),
			float( ( colour >> 24 ) & 255 ));
	}

	inline
		Vector4 getVector4Normalised( u32 colour )
	{
		Vector4 v(
			float(( colour >> 16 ) & 255),
			float(( colour >>  8 ) & 255),
			float(  colour         & 255),
			float(( colour >> 24 ) & 255 ));
		v *= 1.f / 255;
		return v;
	}

	inline
		float luminance( const Vector4& colour )
	{
		return (0.3f*colour.x + 0.59f*colour.y + 0.11f*colour.z);
	}
};
