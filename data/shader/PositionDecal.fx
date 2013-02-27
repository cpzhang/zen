#include "std.fxh"

texture gTexture;
sampler gDecalSampler = Zen_Sampler(gTexture, Clamp);
float3 gCenter = {0,0,0};
float gRadius = 10;
// World * View * Projection matrix
//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
	float4 WorldPos	  : TEXCOORD0;
};


//-----------------------------------------------------------------------------
// Name: RenderSceneVS
// Type: Vertex shader                                      
// Desc: This shader computes standard transform and lighting
//-----------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION)
{
    VS_OUTPUT Output;
    // Transform the position from object space to homogeneous projection space
    float4 p = mul(vPos, gWorld);
	Output.WorldPos = p;
	p = mul(p, gView);
	//p.z += 10.5;
	Output.Position = mul(p, gProjection);
	Output.Position.y += 0.01;
    return Output;    
}

float4 gAmbient = {1.0, 0.0, 0.0, 1.0};
//-----------------------------------------------------------------------------
// Pixel shader output structure
//-----------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//-----------------------------------------------------------------------------
// Name: RenderScenePS                                        
// Type: Pixel shader
// Desc: This shader outputs the pixel's color 
//-----------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output = (PS_OUTPUT)0;
	float3 v = In.WorldPos - gCenter;
	float2 coord;  
	coord.x = v.x / gRadius + 0.5;
	coord.y = v.z / gRadius + 0.5;
	//
	if(coord.x < 1 && coord.y < 1 && coord.x>0 && coord.y>0)
		Output.RGBColor = tex2D(gDecalSampler, coord);

    return Output;
}



//-----------------------------------------------------------------------------
// Name: RenderScene
// Type: Technique                                     
// Desc: Renders scene to render target
//-----------------------------------------------------------------------------
technique T0
{
    pass P0
    {
      WorldTransform[0] = <gWorld>;
      ViewTransform = <gView>;
      ProjectionTransform = <gProjection>;
      
	  CullMode = None;
      FillMode = Wireframe;

      Texture[0] = <gTexture>;
      ColorOP[0] = SelectArg1;
      ColorArg1[0] = Texture;
      AlphaOP[0] = SelectArg1;
      AlphaArg1[0] = Texture;
      TexCoordIndex[0] = PASSTHRU;

      VertexShader = NULL;
      PixelShader  = NULL;
    }
}

technique T2
{
    pass P0
    {          
		AlphaBlendEnable = True;
		SrcBlend = SrcColor;
		DestBlend = InvSrcColor;
		CullMode = None;
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader  = compile ps_2_0 RenderScenePS();
    }
	 pass P1
    {
      AlphaBlendEnable = false;
      SrcBlend = One;
      DestBlend = Zero;
    }
}


