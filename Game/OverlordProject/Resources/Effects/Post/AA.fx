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
    uint2 dimensions = (uint2) 0;
    gTexture.GetDimensions(dimensions.x, dimensions.y);

    float dx = 1.0f / dimensions.x;
    float dy = 1.0f / dimensions.y;

    float4 topLeft = gTexture.Sample(samPoint, float2(input.TexCoord.x - dx, input.TexCoord.y + dy));
    float4 topRight = gTexture.Sample(samPoint, float2(input.TexCoord.x + dx, input.TexCoord.y + dy));
    float4 bottomLeft = gTexture.Sample(samPoint, float2(input.TexCoord.x - dx, input.TexCoord.y - dy));
    float4 bottomRight = gTexture.Sample(samPoint, float2(input.TexCoord.x + dx, input.TexCoord.y - dy));
    
    float4 center = gTexture.Sample(samPoint, input.TexCoord);
    
    float4 finalColor = (center + topLeft + topRight + bottomLeft + bottomRight) / 5.0f;
    return float4((float3) finalColor, 1.0f);
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