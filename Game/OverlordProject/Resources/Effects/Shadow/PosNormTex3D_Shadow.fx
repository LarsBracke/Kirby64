float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gWorldViewProj_Light;

float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4 gLightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
float gLightIntensity = 1.f;

float gShadowMapBias = 0.005f;
float2 gShadowMapSize = float2(1280.0f, 720.0f);
float gShadowHardness = 0.5f;

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerComparisonState cmpSampler
{
    Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;

    ComparisonFunc = LESS_EQUAL;
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
    float4 worldPos : COLOR0;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
    output.pos = mul(float4(input.pos, 1.f), gWorldViewProj);
    output.worldPos = mul(float4(input.pos, 1.f), gWorld);
    output.normal = mul(input.normal, (float3x3)gWorld);
	output.normal = normalize(output.normal);
    output.lPos = mul(float4(input.pos, 1.f), gWorldViewProj_Light);
    output.texCoord = input.texCoord;

	return output;
}

float2 texOffset(int u, int v)
{
    return float2(u * 1.0f / gShadowMapSize.x, v * 1.0f / gShadowMapSize.y);
}

float EvaluateShadowMap(float4 lpos)
{
    lpos.xyz /= lpos.w;

    if (lpos.x < -1.f || lpos.x > 1.f ||
		lpos.y < -1.f || lpos.y > 1.f ||
		lpos.z < 0.f || lpos.z > 1.f)
    {
        return 0.0f;
    }

    lpos.x = lpos.x / 2.f + 0.5f;
    lpos.y = lpos.y / -2.f + 0.5f;

    lpos.z -= gShadowMapBias;

	float total = 0;
    float x = 0, y = 0;
    for (y = -1.5; y <= 1.5; y += 1.0) // 4x4 neighborhood
    {
        for (x = -1.5; x <= 1.5; x += 1.0)
        {
            total +=
				gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
        }
    }
    float shadowFactor  = total / 16.0;
    shadowFactor = clamp(shadowFactor, gShadowHardness, 1.0f);
	return shadowFactor;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	//Shadow
	float shadowValue = EvaluateShadowMap(input.lPos);

    // Diffuse
	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
    diffuseColor *= gLightColor;

    float diffuseStrength = saturate(dot(-input.normal, gLightDirection));
    diffuseStrength = diffuseStrength * 0.5 + 0.5;
    diffuseStrength = saturate(diffuseStrength);
    diffuseColor *= diffuseStrength * gLightIntensity;

	float3 rgb = diffuseColor.rgb;
	float alpha = diffuseColor.a;

    // Final color
    float4 finalColor = float4(rgb * shadowValue, alpha);
    return finalColor;
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

