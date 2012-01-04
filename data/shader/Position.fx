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
};	// View * Projection matrix
texture lay0;
// World * View * Projection matrix
//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
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
	p = mul(p, gView);
	//p.z += 10.5;
	Output.Position = mul(p, gProjection);
    return Output;    
}

float4 gAmbient = {1.0, 0.0, 0.0, 1.0};
float4 gMeshColor = {0.0, 1.0, 0.0, 1.0};
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
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = gAmbient;

    return Output;
}
PS_OUTPUT RenderScenePS_Mesh( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = gMeshColor;

    return Output;
}


//-----------------------------------------------------------------------------
// Name: RenderScene
// Type: Technique                                     
// Desc: Renders scene to render target
//-----------------------------------------------------------------------------
technique T0
{
    pass P0//µØ±í
    {
      WorldTransform[0] = <gWorld>;
      ViewTransform = <gView>;
      ProjectionTransform = <gProjection>;
      
	  CullMode = None;
      FillMode = Wireframe;

      Texture[0] = <lay0>;
      ColorOP[0] = SelectArg1;
      ColorArg1[0] = Texture;
      AlphaOP[0] = SelectArg1;
      AlphaArg1[0] = Texture;
      TexCoordIndex[0] = PASSTHRU;

      VertexShader = NULL;
      PixelShader  = NULL;
    }
	pass P1//Íø¸ñ
    {
      WorldTransform[0] = <gWorld>;
      ViewTransform = <gView>;
      ProjectionTransform = <gProjection>;
      
	  CullMode = None;
      FillMode = Wireframe;

      Texture[0] = <lay0>;
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
		CullMode = None;
		FillMode = Wireframe;
		VertexShader = compile vs_2_0 RenderSceneVS();
		PixelShader  = compile ps_2_0 RenderScenePS();
    }
// 	pass P1
//     {          
// 		CullMode = None;
// 		FillMode = Wireframe;
// 		VertexShader = compile vs_2_0 RenderSceneVS();
// 		PixelShader  = compile ps_2_0 gMeshColor();
//     }
}
