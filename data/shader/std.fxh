#pragma once
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 gWorld= 
{
1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1
};	// World matrix
float4x4 gView= 
{
1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1
};	// View matrix
float4x4 gProjection= 
{
1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1
};	// Projection matrix
//√…∆§æÿ’Û
float4x4 gSkinPalette[50];

int     gMipFilter     : MipFilter = 2;
int     gMinMagFilter  : MinMagFilter = 2;
int     gMaxAnisotropy : MaxAnisotropy = 1;

#define Zen_Sampler(map, addressType)\
sampler_state\
{\
	Texture = (map);\
	ADDRESSU = addressType;\
	ADDRESSV = addressType;\
	ADDRESSW = addressType;\
	MAGFILTER = LINEAR;\
	MINFILTER = (gMinMagFilter);\
	MIPFILTER = (gMipFilter);\
	MAXANISOTROPY = (gMaxAnisotropy);\
	MAXMIPLEVEL = 0;\
	MIPMAPLODBIAS = 0;\
};

//