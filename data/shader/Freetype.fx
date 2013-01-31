float4   g_diffuse;
// Vertex shader input structure
struct VS_INPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 TexCoord   : TEXCOORD0;  // vertex texture coordinate
};

// Vertex shader output structure
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position
    float2 TexCoord   : TEXCOORD0;  // vertex texture coordinate
};

texture g_MeshTexture;              // Color texture for mesh
sampler MeshTextureSampler =
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//-----------------------------------------------------------------------------
// Pixel shader output structure
//-----------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

PS_OUTPUT RenderScenePS( VS_INPUT In ) 
{ 
    PS_OUTPUT Output;
    // Lookup mesh texture and modulate it with diffuse
    float4 t = tex2D(MeshTextureSampler, In.TexCoord);
    Output.RGBColor = float4(g_diffuse.x, g_diffuse.y, g_diffuse.z, t.w * g_diffuse.w);

    return Output;
}

//-----------------------------------------------------------------------------
// Name: RenderScene
// Type: Technique                                     
// Desc: Renders scene to render target
//-----------------------------------------------------------------------------
technique RenderScene
{
    pass P0
    {
     /* The default value is D3DCULL_CCW. */
      CullMode = None;
      AlphaBlendEnable = true;
      SrcBlend = SrcAlpha;
      DestBlend = InvSrcAlpha;
  
     VertexShader = null;//compile vs_2_0 RenderSceneVS();
      
      PixelShader  = compile ps_2_0 RenderScenePS();
      FillMode = Solid;
    }
}
