//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState gDepthStencilState
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState gRasterizerState
{
    CullMode = BACK;
};

float4 gLineColor = float4(0.f, 0.f, 0.f, 1.f);

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
    output.Position = float4(input.Position, 1.0f);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    // Texture dimensions
    uint2 dimensions = (uint2) 0;
    gTexture.GetDimensions(dimensions.x, dimensions.y);
    
    float dx = 1.0f / dimensions.x;
    float dy = 1.0f / dimensions.y;
    
    // Sample default color
    float4 defaultColor = gTexture.Sample(samPoint, input.TexCoord + float2(0, 0));
    
    // Sample neighboring colors
    float e11 = gTexture.Sample(samPoint, input.TexCoord + float2(-dx, -dy));
    float e12 = gTexture.Sample(samPoint, input.TexCoord + float2(0, -dy));
    float e13 = gTexture.Sample(samPoint, input.TexCoord + float2(dx, -dy));

    float e21 = gTexture.Sample(samPoint, input.TexCoord + float2(-dx, 0));
    float e22 = gTexture.Sample(samPoint, input.TexCoord + float2(0, 0));
    float e23 = gTexture.Sample(samPoint, input.TexCoord + float2(dx, 0));

    float e31 = gTexture.Sample(samPoint, input.TexCoord + float2(-dx, dy));
    float e32 = gTexture.Sample(samPoint, input.TexCoord + float2(0, dy));
    float e33 = gTexture.Sample(samPoint, input.TexCoord + float2(dx, dy));

    // Line detection
    float t1 = e13 + e33 + (2 * e23) - e11 - (2 * e21) - e31;
    float t2 = e31 + (2 * e32) + e33 - e11 - (2 * e12) - e13;
    
    if (((t1 * t1) + (t2 * t2)) > 0.8f)
        return gLineColor;
    else
        return float4((float3)defaultColor, 1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}