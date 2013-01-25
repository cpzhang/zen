// 3ds max effect file
#include "std.fxh"
// This is used by 3dsmax to load the correct parser
string ParamID = "0x0001";

struct VS_INPUT
{
  float4 Pos  : POSITION;
  float3 Nor : NORMAL;
  float2 Tex : TEXCOORD0;      
  float4 Color : COLOR0;
  int4 boneIndices : BLENDINDICES;
  float4 weights  : BLENDWEIGHT;
};
struct VS_OUTPUT
{
  float4 Pos  : POSITION;
  float2 Tex : TEXCOORD0;
  //float4 Color : COLOR0;
  //float4 LightColor : COLOR1;
  //float4 vPosLight : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT ip)
{
	bool skined = false;
	float4 p = float4(0, 0, 0, 1);
    for(int i = 0; i != 4; i++)
	{
		float w = ip.weights[i];
		if(w > 0.0001f) 
		{
			//skined=true;
			p += mul(ip.Pos, gSkinPalette[ip.boneIndices[i]]) * w;
		}
	}
	if(!skined)
	{
		//p = ip.Pos;
	}
    p.w = 1;
	p = mul(p, gWorld);
	p = mul(p, gView);
	VS_OUTPUT op;
	op.Pos = mul(p, gProjection);
	op.Tex  = ip.Tex;
	return op;
}
float4x4 gUV= 
{
1,0,0,0,
0,1,0,0,
0,0,1,0,
0,0,0,1
};
float4 g_TFactor = {1.0, 1.0, 1.0, 1.0};
texture g_Texture0 < 
string UIName = "Texture0";
string ResourceType = "2D";
int Texcoord = 0;
>;

sampler gSampler0 = Zen_Sampler(g_Texture0, Wrap);
float4 PS(VS_OUTPUT In) : COLOR
{
  float4 color = (float4)0;
  float2 uv = mul(float4(In.Tex, 1.0, 0.0), gUV);
  color = tex2D(gSampler0, uv) * g_TFactor;
  return  color;
}

technique T0
{
    pass P0
    {
      WorldTransform[0] = <gWorld>;
      ViewTransform = <gView>;
      ProjectionTransform = <gProjection>;

      Texture[0] = <g_Texture0>;

      VertexShader = NULL;
      PixelShader  = NULL;
    }
}
technique T2
{
    pass P0
    {
		VertexShader = compile vs_2_0 VS();
		//
		Texture[0] = <g_Texture0>;
		PixelShader  = NULL;//compile ps_2_0 PS();
    }  
}



