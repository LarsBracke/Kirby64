#include "stdafx.h"
#include "MeshFilterLoader.h"

//#define MULTI_TEXCOORD
#define OVM_vMAJOR 1
#ifdef MULTI_TEXCOORD
	#define OVM_vMINOR 61
#else
	#define OVM_vMINOR 1
#endif


MeshFilter* MeshFilterLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if(!pReader->Exists())
		return nullptr;

	//READ OVM FILE
	const int versionMajor = pReader->Read<char>();
	const int versionMinor = pReader->Read<char>();

	if (versionMajor != OVM_vMAJOR || versionMinor != OVM_vMINOR)
	{
		Logger::LogWarning(L"Wrong OVM version\n\tFile: \"{}\" \n\tExpected version {}.{}, not {}.{}.", loadInfo.assetFullPath.filename().wstring(), OVM_vMAJOR, OVM_vMINOR, versionMajor, versionMinor);

		delete pReader;
		return nullptr;
	}

	unsigned int vertexCount = 0;
	unsigned int indexCount = 0;

	const auto pMesh = new MeshFilter();

	for (;;)
	{
		const auto meshDataType = static_cast<MeshDataType>(pReader->Read<char>());
		if(meshDataType == MeshDataType::END)
			break;

		const auto dataOffset = pReader->Read<unsigned int>();

		switch(meshDataType)
		{
		case MeshDataType::HEADER:
			{
				pMesh->m_MeshName = pReader->ReadString();
				vertexCount = pReader->Read<unsigned int>();
				indexCount = pReader->Read<unsigned int>();

				pMesh->m_VertexCount = vertexCount;
				pMesh->m_IndexCount = indexCount;
			}
			break;
		case MeshDataType::POSITIONS:
			{
				pMesh->SetElement(ILSemantic::POSITION);

				for(unsigned int i = 0; i<vertexCount; ++i)
				{
					XMFLOAT3 pos{};
					pos.x = pReader->Read<float>();
					pos.y = pReader->Read<float>();
					pos.z = pReader->Read<float>();
					pMesh->m_Positions.emplace_back(pos);
				}
			}
			break;
		case MeshDataType::INDICES:
			{
				for(unsigned int i = 0; i<indexCount; ++i)
				{
					pMesh->m_Indices.emplace_back(pReader->Read<DWORD>());
				}
			}
			break;
		case MeshDataType::NORMALS:
			{
				pMesh->SetElement(ILSemantic::NORMAL);

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
					XMFLOAT3 normal{};
					normal.x = pReader->Read<float>();
					normal.y = pReader->Read<float>();
					normal.z = pReader->Read<float>();
					pMesh->m_Normals.emplace_back(normal);
				}
			}
			break;
		case MeshDataType::TANGENTS:
			{
				pMesh->SetElement(ILSemantic::TANGENT);

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
					XMFLOAT3 tangent{};
					tangent.x = pReader->Read<float>();
					tangent.y = pReader->Read<float>();
					tangent.z = pReader->Read<float>();
					pMesh->m_Tangents.emplace_back(tangent);
				}
			}
			break;
		case MeshDataType::BINORMALS:
			{
				pMesh->SetElement(ILSemantic::BINORMAL);

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
					XMFLOAT3 binormal{};
					binormal.x = pReader->Read<float>();
					binormal.y = pReader->Read<float>();
					binormal.z = pReader->Read<float>();
					pMesh->m_Binormals.emplace_back(binormal);
				}
			}
			break;
		case MeshDataType::TEXCOORDS:
			{
				pMesh->SetElement(ILSemantic::TEXCOORD);

				constexpr auto amountTexCoords = 1;
#ifdef MULTI_TEXCOORD
				amountTexCoords = pReader->Read<USHORT>();
#endif
				pMesh->m_TexCoordCount = amountTexCoords;
				for (unsigned int i = 0; i<vertexCount*amountTexCoords; ++i)
				{
					XMFLOAT2 tc{};
					tc.x = pReader->Read<float>();
					tc.y = pReader->Read<float>();
					pMesh->m_TexCoords.emplace_back(tc);
				}
			}
			break;
		case MeshDataType::COLORS:
			{
				pMesh->SetElement(ILSemantic::COLOR);

				for (unsigned int i = 0; i<vertexCount; ++i)
				{
					XMFLOAT4 color{};
					color.x = pReader->Read<float>();
					color.y = pReader->Read<float>();
					color.z = pReader->Read<float>();
					color.w = pReader->Read<float>();
					pMesh->m_Colors.emplace_back(color);
				}
			}
			break;
		case MeshDataType::BLENDINDICES:
		{
			//TODO_W7(L"Add BLENDINDICES information");
			pMesh->SetElement(ILSemantic::BLENDINDICES);

			for (unsigned int index = 0; index < vertexCount; ++index)
			{
				XMFLOAT4 indices{ };
				indices.x = pReader->Read<float>();
				indices.y = pReader->Read<float>();
				indices.z = pReader->Read<float>();
				indices.w = pReader->Read<float>();
				pMesh->m_BlendIndices.emplace_back(indices);
			}
		}
		break;
		case MeshDataType::BLENDWEIGHTS:
		{
			//TODO_W7(L"Add BLENDWEIGHTS information");
			pMesh->SetElement(ILSemantic::BLENDWEIGHTS);
			
			for (unsigned int index = 0; index < vertexCount; ++index)
			{
				XMFLOAT4 weights{ };
				weights.x = pReader->Read<float>();
				weights.y = pReader->Read<float>();
				weights.z = pReader->Read<float>();
				weights.w = pReader->Read<float>();
				pMesh->m_BlendWeights.emplace_back(weights);
			}
		}
		break;
		case MeshDataType::ANIMATIONCLIPS:
		{
			//TODO_W7(L"Add ANIMATIONCLIPS information");
			const unsigned int clipCount = pReader->Read<USHORT>();
			if (clipCount > 0)
				pMesh->m_HasAnimations = true;

			for (unsigned int clipIndex = 0; clipIndex < clipCount; ++clipIndex)
			{
				AnimationClip clip{ };

				clip.name = pReader->ReadString();
				clip.duration = pReader->Read<float>();
				clip.ticksPerSecond = pReader->Read<float>();

				const unsigned int keyCount = pReader->Read<USHORT>();
				for (unsigned int keyIndex = 0; keyIndex < keyCount; ++keyIndex)
				{
					AnimationKey key{ };

					key.tick = pReader->Read<float>();
					const unsigned int boneTransformCount = pReader->Read<USHORT>();
					for (unsigned int boneTransformIndex = 0; boneTransformIndex < boneTransformCount; ++boneTransformIndex)
					{
						key.boneTransforms.emplace_back(pReader->Read<XMFLOAT4X4>());
					}

					clip.keys.emplace_back(key);
				}
				pMesh->m_AnimationClips.emplace_back(clip);
			}
		}
		break;
		case MeshDataType::SKELETON:
		{
			//TODO_W7(L"Add SKELETON information");
			const unsigned int boneCount = pReader->Read<USHORT>();
			pMesh->m_BoneCount = static_cast<USHORT>(boneCount);
			pReader->MoveBufferPosition(dataOffset - sizeof(USHORT));
		}
		break;
		default:
			pReader->MoveBufferPosition(dataOffset);
			break;
		}
	}

	delete pReader;

	return pMesh;
}

void MeshFilterLoader::Destroy(MeshFilter* objToDestroy)
{
	SafeDelete(objToDestroy);
}
