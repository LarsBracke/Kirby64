#include "stdafx.h"
#include "SpriteFontLoader.h"
#include <sstream>

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout
	char id[3]{ };
	id[0] = pReader->Read<char>();
	id[1] = pReader->Read<char>();
	id[2] = pReader->Read<char>();
	const unsigned int version = pReader->Read<byte>();

	if (id[0] != 'B' || id[1] != 'M' || id[2] != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font\n");
		return nullptr;
	}

	if (version < 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported\n");
		return nullptr;
	}

	//Valid .fnt file >> Start Parsing!
	SpriteFontDesc fontDesc{};
	unsigned int blockId{ };
	unsigned int blockSize{ };
	std::wstring fontName{ };
	unsigned int pageCount{ };
	std::wstring pageName{ };

	//**********
	// BLOCK 0 *
	//**********
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<ULONG>();
	fontDesc.fontSize = pReader->Read<USHORT>();
	pReader->MoveBufferPosition(12);
	fontName = pReader->ReadNullString();

	//**********
	// BLOCK 1 *
	//**********
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<ULONG>();

	pReader->MoveBufferPosition(4);

	fontDesc.textureWidth = pReader->Read<USHORT>();
	fontDesc.textureHeight = pReader->Read<USHORT>();

	pageCount = pReader->Read<USHORT>();
	if (pageCount > 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only one texture per font is allowed!");
		return nullptr;
	}

	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<ULONG>();

	pageName = pReader->ReadNullString();
	std::wstring path =  ContentManager::GetFullAssetPath(L"SpriteFonts/");
	path.append(pageName);
	fontDesc.pTexture = ContentManager::Load<TextureData>(path);

	//**********
	// BLOCK 3 *
	//**********
	blockId = pReader->Read<byte>();
	blockSize = pReader->Read<ULONG>();

	const unsigned int charCount = blockSize / 20;
	for (unsigned int count = 0; count < charCount; ++count)
	{
		FontMetric fontMetric{ };

		fontMetric.character = static_cast<wchar_t>(pReader->Read<ULONG>());
		const USHORT xPos = pReader->Read<USHORT>();
		const USHORT yPos = pReader->Read<USHORT>();
		fontMetric.width = pReader->Read<USHORT>();
		fontMetric.height = pReader->Read<USHORT>();
		fontMetric.offsetX = pReader->Read<USHORT>();
		fontMetric.offsetY = pReader->Read<USHORT>();
		fontMetric.advanceX = pReader->Read<USHORT>();
		fontMetric.page = pReader->Read<byte>();

		const byte map = pReader->Read<byte>();		// 0000 0001 == 1 | 0000 0010 == 2 | 0000 0100 == 4 | 0000 1000 == 8
		switch (map)								// Format: 0000 ARGB
		{											// channel 0 == Red | channel 1 == Green | channel 2 == blue | channel 3 == alpha
		case 1: // B
			fontMetric.channel = 2; break;
		case 2: // G
			fontMetric.channel = 1; break;
		case 4: // R
			fontMetric.channel = 0; break;
		case 8: // A
			fontMetric.channel = 3; break;
		}
		
		const XMFLOAT2 texCoord
		{
			static_cast<float>(xPos) / static_cast<float>(fontDesc.textureWidth),
			static_cast<float>(yPos) / static_cast<float>(fontDesc.textureHeight)
		};
		fontMetric.texCoord = texCoord;

		fontDesc.metrics[fontMetric.character] = fontMetric;
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
