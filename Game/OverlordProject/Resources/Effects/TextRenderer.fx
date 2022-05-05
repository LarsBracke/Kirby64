//INCOMPLETE!

float4x4 gTransform : WORLDVIEWPROJECTION;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

RasterizerState BackCulling
{
	CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	int Channel : TEXCOORD2; //Texture Channel
	float3 Position : POSITION; //Left-Top Character Quad Starting Position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Left-Top Character Texture Coordinate on Texture
	float2 CharSize: TEXCOORD1; //Size of the character (in screenspace)
};

struct GS_DATA
{
	float4 Position : SV_POSITION; //HOMOGENEOUS clipping space position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Texcoord of the vertex
	int Channel : TEXCOORD1; //Channel of the vertex
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{
	//Create a new GS_DATA object
	//Fill in all the fields
	//Append it to the TriangleStream
	GS_DATA data = (GS_DATA)0;
	data.Position = mul(float4(pos,1), gTransform);
	data.Color = col;
	data.TexCoord = texCoord;
	data.Channel = channel;
	
	triStream.Append(data);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	float3 position = vertex[0].Position;
	float4 color = vertex[0].Color;
	float2 texCoord = vertex[0].TexCoord;
    int channel = vertex[0].Channel;
	
	float2 characterSize = vertex[0].CharSize;
	float2 characterTextureSize = characterSize / gTextureSize;

	//1. Vertex Left-Top
	CreateVertex(triStream, position, color, texCoord, channel);

	//2. Vertex Right-Top
	position.x += characterSize.x;
	texCoord.x += characterTextureSize.x;
	
	CreateVertex(triStream, position, color, texCoord, channel);

	//3. Vertex Left-Bottom
	position.x -= characterSize.x;
	position.y += characterSize.y;
	texCoord.x -= characterTextureSize.x;
	texCoord.y += characterTextureSize.y;
	
	CreateVertex(triStream, position, color, texCoord, channel);

	//4. Vertex Right-Bottom
	position.x += characterSize.x;
	texCoord.x += characterTextureSize.x;
	
	CreateVertex(triStream, position, color, texCoord, channel);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{	
    float4 color = gSpriteTexture.Sample(samPoint, input.TexCoord)[input.Channel] * input.Color;
	return color;
}

// Default Technique
technique10 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
