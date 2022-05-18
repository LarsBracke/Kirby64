#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

using namespace MathHelper;

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO_W9(L"Implement Destructor")
	m_pVertexBuffer->Release();
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Initialize")

	// Create particle material
	if (m_pParticleMaterial == nullptr)
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();

	// Create vertex-buffer
	CreateVertexBuffer(sceneContext);

	// Load particle texture
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement CreateVertexBuffer")

	// Clear the buffer
	if (m_pVertexBuffer != nullptr)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}

	// Create new buffer
	D3D11_BUFFER_DESC desc{ };
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(VertexParticle) * m_MaxParticles;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	const HRESULT result = sceneContext.d3dContext.pDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer);
	HANDLE_ERROR(result);
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Update")

	// Calculate particle interval
	const float particleInterval = ((m_EmitterSettings.minEnergy + m_EmitterSettings.maxEnergy) / 2.f) / m_ParticleCount;
	m_LastParticleSpawn += sceneContext.pGameTime->GetElapsed();

	// Reset active particles
	m_ActiveParticles = 0;

	// Mapping the vertex-buffer
	D3D11_MAPPED_SUBRESOURCE resource{ };
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	VertexParticle* pBuffer{ static_cast<VertexParticle*>(resource.pData) };

	// Spawn and update particles
	const float elapsedTime = sceneContext.pGameTime->GetElapsed();
	for (unsigned int count = 0; count <= m_ParticleCount; ++count)
	{
		Particle& particle = m_ParticlesArray[count];
		if (particle.isActive)
			UpdateParticle(particle, elapsedTime);

		if (!particle.isActive && m_LastParticleSpawn >= particleInterval)
		{
			m_LastParticleSpawn = 0.f;
			SpawnParticle(particle);
		}

		if (particle.isActive)
		{
			pBuffer[m_ActiveParticles] = particle.vertexInfo;
			++m_ActiveParticles;
		}
	}

	// Unmap the vertex-buffer
	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	//TODO_W9(L"Implement UpdateParticle")

	if (!p.isActive)
		return;

	// Decrease particle energy
	p.currentEnergy -= elapsedTime;
	if (p.currentEnergy <= 0.f)
	{
		p.currentEnergy = 0;
		p.isActive = false;
		return;
	}

	// Calculate life percentage
	const float lifePercentage = p.currentEnergy / p.totalEnergy;

	// Update particle variables
	XMFLOAT3 newPosition{ };
	newPosition.x = p.vertexInfo.Position.x + (m_EmitterSettings.velocity.x * elapsedTime);
	newPosition.y = p.vertexInfo.Position.y + (m_EmitterSettings.velocity.y * elapsedTime);
	newPosition.z = p.vertexInfo.Position.z + (m_EmitterSettings.velocity.z * elapsedTime);
	p.vertexInfo.Position = newPosition;

	XMFLOAT4 newColor{ m_EmitterSettings.color };
	newColor.w *= lifePercentage * 2.f;
	p.vertexInfo.Color = newColor;

	float newSize{ };
	const float sizeDelta{ (1 - lifePercentage) * p.sizeChange };
	if (p.sizeChange < 1.f)
		newSize = p.initialSize - sizeDelta;
	else if (p.sizeChange > 1.f)
		newSize = p.initialSize + sizeDelta;
	p.vertexInfo.Size = newSize;
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	//TODO_W9(L"Implement SpawnParticle")

	// Activate particle
	p.isActive = true;
	p.totalEnergy = randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	// Initialize position;
	XMVECTOR randomDirection{ XMVectorSet(1,0,0,1) };
	const XMMATRIX randomRotationMatrix{ XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI)) };
	randomDirection = XMVector3TransformNormal(randomDirection, randomRotationMatrix);

	const XMFLOAT3 origin{ p.vertexInfo.Position };
	const float randomDistance{ randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius) };
	XMVECTOR newPosition{ XMVectorSet(origin.x, origin.y, origin.z, 1.f) };
	newPosition = (XMLoadFloat3(&GetTransform()->GetWorldPosition()) + randomDirection * randomDistance);
	XMStoreFloat3(&p.vertexInfo.Position, newPosition);

	// Initialize size
	const float randomSize{ randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize) };
	p.initialSize = randomSize;
	p.vertexInfo.Size = randomSize;

	const float randomSizeChange{ randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale) };
	p.sizeChange = randomSizeChange;

	// Initialize rotation
	const float randomRotation{ randF(-XM_PI, XM_PI) };
	p.vertexInfo.Rotation = randomRotation;

	// Initialize color
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement PostDraw")

	// Set shader variables
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	// Set input layout
	const auto techniqueContext = m_pParticleMaterial->GetTechniqueContext();
	auto* pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);


	// Set vertex-buffer
	constexpr auto strides = static_cast<UINT>(sizeof(VertexParticle));
	constexpr auto offsets = static_cast<UINT>(0);
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offsets);

	// Drawing
	D3DX11_TECHNIQUE_DESC desc{ };
	techniqueContext.pTechnique->GetDesc(&desc);
	const UINT passCount = desc.Passes;

	for (UINT pass = 0; pass < passCount; ++pass)
	{
		techniqueContext.pTechnique->GetPassByIndex(pass)->Apply(0, pDeviceContext);
		pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}