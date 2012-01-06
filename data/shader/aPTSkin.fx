// 3ds max effect file
#include "std.fxh"// This is used by 3dsmax to load the correct parser
string ParamID = "0x0001";
struct VS_INPUT
{
  float4 Pos  : POSITION;
  float2 Tex : TEXCOORD0;      
  int4 boneIndices : BLENDINDICES;
  float4 weights  : BLENDWEIGHT;
  float4 Color : COLOR0;
  float3 Nor : NORMAL;
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
	float4 p = 0;
    for(int i = 0; i != 4; i++)
	{
		float w = ip.weights[i];
		if(w > 0.00001f)
			p += mul(ip.Pos, gSkinPalette[ip.boneIndices[i]]) * ip.weights[i];
	}
    p.w = 1;
	p = mul(p, gWorld);
	p = mul(p, gView);
	VS_OUTPUT op;
	op.Pos = mul(p, gProjection);
	op.Tex  = ip.Tex;
	return op;
}
texture g_Texture0 < 
string UIName = "Texture0";
string ResourceType = "2D";
int Texcoord = 0;
>;
sampler gSampler0 = Zen_Sampler(g_Texture0, Wrap);
float4 PS(VS_OUTPUT In) : COLOR
{
  float4 color = (float4)0;
  color = tex2D(gSampler0, In.Tex);
  color.r += 0.5;
  return  color;
}
technique T0
{
    pass P0
    {
      WorldTransform[0] = <gWorld>;
      ViewTransform = <gView>;
      ProjectionTransform = <gProjection>;
      
	  //CullMode = None;
      //FillMode = Wireframe;

      Texture[0] = <g_Texture0>;
      //ColorOP[0] = SelectArg1;
      //ColorArg1[0] = Texture;
      //AlphaOP[0] = SelectArg1;
      //AlphaArg1[0] = Texture;
      //TexCoordIndex[0] = PASSTHRU;

      VertexShader = NULL;
      PixelShader  = NULL;
    }
}
technique T2
{
    pass P0
    {
		//CullMode = None;
		//FillMode = Solid;
		//ZEnable = False;
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();
		//PixelShader = NULL;
    }  
}
