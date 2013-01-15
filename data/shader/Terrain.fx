#include "std.fxh"
texture gLayer0;
texture gLayer1;
texture gLayer2;
sampler gSamplerLayer0 = Zen_Sampler(gLayer0, Wrap);
sampler gSamplerLayer1 = Zen_Sampler(gLayer1, Wrap);
sampler gSamplerLayer2 = Zen_Sampler(gLayer2, Wrap);

//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_Input
{
    float4 Position   : POSITION;   // vertex position 
	float2 TexCoord   : TEXCOORD0;  // vertex texture coordinate
	float4 Blend	  : COLOR0;		// diffuse
};

//-----------------------------------------------------------------------------
struct VS_Output
{
    float4 Position   : POSITION;   // vertex position
    float2 TexCoord   : TEXCOORD0;  // vertex texture coordinate
    float4 Blend	  : COLOR0;		// diffuse
};

//-----------------------------------------------------------------------------
// Name: RenderSceneVS
// Type: Vertex shader                                      
// Desc: This shader computes standard transform and lighting
//-----------------------------------------------------------------------------
VS_Output RenderSceneVS(VS_Input ip)
{
    VS_Output op;
    // Transform the position from object space to homogeneous projection space
    float4 p = mul(ip.Position, gWorld);
	p = mul(p, gView);
	op.Position = mul(p, gProjection);
	op.TexCoord = ip.TexCoord;
	op.Blend = ip.Blend;
    return op;    
}

//-----------------------------------------------------------------------------
// Pixel shader output structure
//-----------------------------------------------------------------------------
struct PS_Output
{
    float4 RGBColor : COLOR0;  // Pixel color
};


//-----------------------------------------------------------------------------
// Name: RenderScenePS                                        
// Type: Pixel shader
// Desc: This shader outputs the pixel's color 
//-----------------------------------------------------------------------------
PS_Output RenderScenePS( VS_Output ip ) 
{ 
    PS_Output op;

    // Lookup mesh texture and modulate it with diffuse
	float4 c = (float4)0;
    c += ip.Blend.r*tex2D(gSamplerLayer0, ip.TexCoord);
	c += ip.Blend.g*tex2D(gSamplerLayer1, ip.TexCoord);
	c += ip.Blend.b*tex2D(gSamplerLayer2, ip.TexCoord);

    op.RGBColor = c;
    return op;
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

      Texture[0] = <gLayer0>;
      ColorOP[0] = SelectArg1;
      ColorArg1[0] = Texture;
      AlphaOP[0] = SelectArg1;
      AlphaArg1[0] = Texture;
      TexCoordIndex[0] = PASSTHRU;

      VertexShader = NULL;
      PixelShader  = NULL;
    }
	pass P1
    {
      WorldTransform[0] = <gWorld>;
      ViewTransform = <gView>;
      ProjectionTransform = <gProjection>;
      
	  CullMode = None;
      FillMode = Wireframe;

      Texture[0] = <gLayer0>;
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
		AlphaBlendEnable = False;
		CullMode = None;
		FillMode = Solid;
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader  = compile ps_2_0 RenderScenePS();
    }
}


