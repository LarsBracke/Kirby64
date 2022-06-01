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
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
    uint2 dimensions = (uint2) 0;
    gTexture.GetDimensions(dimensions.x, dimensions.y);
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
    float dx = 1.0f / dimensions.x;
    float dy = 1.0f / dimensions.y;
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
    
    float3 verticalColor = (float3)gTexture.Sample(samPoint, input.TexCoord);
    for (int countY = 1; countY < 6; ++countY)
    {        
        float up = input.TexCoord.y + (countY * dy);
        float down = input.TexCoord.y - (countY * dx);
        float3 upColor = (float3) gTexture.Sample(samPoint, float2(input.TexCoord.x, up));
        float3 downColor = (float3) gTexture.Sample(samPoint, float2(input.TexCoord.x, down));
        verticalColor += ((upColor + downColor) / 2.0f);
    }
    verticalColor /= 5.0f;
    
    float3 horizontalColor = (float3) gTexture.Sample(samPoint, input.TexCoord);
    for (int countX = 1; countX < 6; ++countX)
    {
        float left = input.TexCoord.x - (countX * dx);
        float right = input.TexCoord.x + (countX * dx);
        float3 leftColor = (float3) gTexture.Sample(samPoint, float2(left, input.TexCoord.y));
        float3 rightColor = (float3) gTexture.Sample(samPoint, float2(right, input.TexCoord.y));
        horizontalColor += ((rightColor + leftColor) / 2.0f);
    }
    horizontalColor /= 5.0f;
    
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
    // Step 5: return the final color
    float4 finalColor = float4((verticalColor + horizontalColor) / 2.0f,  1.0f);
    return finalColor;
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