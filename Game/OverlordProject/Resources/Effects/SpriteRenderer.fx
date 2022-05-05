
float4x4 gTransform : WorldViewProjection;
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

DepthStencilState NoDepth
{
    DepthEnable = FALSE;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0;
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
    if (rotation != 0)
    {
		//Step 3.
		//Do rotation calculations
		//Transform to origin
		//Rotate
		//Retransform to initial position
        
		pos -= float3(offset.x, offset.y, 0.0f);
		pos += float3(pivotOffset.x, pivotOffset.y, 0);
		float3 originalPos = pos;
		pos.x = (originalPos.x * rotCosSin.x) - (originalPos.y * rotCosSin.y);
		pos.y = (originalPos.y * rotCosSin.x) + (originalPos.x * rotCosSin.y);
		pos += float3(offset.x, offset.y, 0.0f);
		
    }
    else
    {
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		
		pos += float3(pivotOffset.x, pivotOffset.y, 0);

    }
    
	//Geometry Vertex Output
    GS_DATA geomData = (GS_DATA) 0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
    float4 transformData = vertex[0].TransformData;
    float4 transformData2 = vertex[0].TransformData2;
    
	//Given Data (Vertex Data)
    float3 position = float3(transformData.x, transformData.y, transformData.z); //Extract the position data from the VS_DATA vertex struct
    float2 offset = float2(transformData.x, transformData.y); //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float rotation = transformData.w; //Extract the rotation data from the VS_DATA vertex struct
    float2 pivot = float2(transformData2.x, transformData2.y); //Extract the pivot data from the VS_DATA vertex struct
    float2 scale = float2(transformData2.z,transformData2.w); //Extract the scale data from the VS_DATA vertex struct
    float2 texCoord = float2(0, 0); //Initial Texture Coordinate
	
	//...

	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// LB----------RB

    //float3 vertexPosition = float3(position.x, position.y, position.z);
    //float3 vertexPosition2 = float3(position.x + (scale.x * gTextureSize.x), position.y, position.z);
    //float3 vertexPosition3 = float3(position.x, position.y - (scale.y * gTextureSize.y), position.z);
    //float3 vertexPosition4 = float3(position.x + (scale.x * gTextureSize.x), position.y - (scale.y * gTextureSize.y), position.z);

	float2 size = float2(scale.x * gTextureSize.x, scale.y * gTextureSize.y);
	float2 halfSize = size/2.0f;
	
    float3 vertexPosition = position + float3(halfSize.x, -halfSize.y, 0.0f);
    float3 vertexPosition2 = position + float3(halfSize.x, halfSize.y, 0.0f);
    float3 vertexPosition3 = position + float3(-halfSize.x, -halfSize.y, 0.0f);
    float3 vertexPosition4 = position + float3(-halfSize.x, halfSize.y, 0.0f);
    
    float2 textureCoordinates = float2(1, 0);
    float2 textureCoordinates2 = float2(1, 1);
    float2 textureCoordinates3 = float2(0, 0);
    float2 textureCoordinates4 = float2 (0, 1);

	float offsetX = (pivot.x * size.x) - halfSize.x;
	float offsetY = (pivot.y * size.y) - halfSize.y;
	
    float2 pivotOffset = float2(-offsetX, -offsetY);
    float2 pivotOffset2 = float2(-offsetX, offsetY);
    float2 pivotOffset3 = float2(offsetX, -offsetY);
    float2 pivotOffset4 = float2(offsetX, offsetY);

    float2 rotCosSin = (float2)0;
    if (rotation != 0)
    {
        rotCosSin.x = cos(rotation);
        rotCosSin.y = sin(rotation);
    }
    
    float4 color = vertex[0].Color;

	//VERTEX 1 [LT]
    CreateVertex(triStream, vertexPosition, color, textureCoordinates, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 2 [RT]
    CreateVertex(triStream, vertexPosition2, color, textureCoordinates2, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 3 [LB]
    CreateVertex(triStream, vertexPosition3, color, textureCoordinates3, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!
    
	//VERTEX 4 [RB]
    CreateVertex(triStream, vertexPosition4, color, textureCoordinates4, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
}

// Default Technique
technique10 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        //SetDepthStencilState(NoDepth, 0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
