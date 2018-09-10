#include "pch.h"
#include "SkeletonMaterial.h"

Light::render::SkeletonMaterial::SkeletonMaterial(IContext * pContext):m_pModelUniform(nullptr),m_pMVPUniform(nullptr)
{
	auto pRenderer = pContext->GetSystem<RenderDevice>();
	auto pResources = pContext->GetSystem<resources::IResourceManager>();

	auto pVertexShader = pResources->VGetVertexShader("Skeleton");
	auto pPixelShader = pResources->VGetPixelShader("Default");

	m_Pipeline = std::unique_ptr<Pipeline>(pRenderer->CreatePipeline(pVertexShader, pPixelShader));
	this->GetUniform();
}

void Light::render::SkeletonMaterial::Apply(RenderDevice * renderer, const float * model, const float * mvp)
{
	renderer->SetPipeline(m_Pipeline.get());
	if(m_uTex) m_uTex->SetAsInt(UNIT_DEFAULT);
	m_uCubeTex->SetAsInt(UNIT_SKYBOX);
	renderer->SetTexture(UNIT_SKYBOX,renderer->GetSkyBoxTexture());
	m_uCameraPos->SetAsVec3(glm::value_ptr(renderer->VGetCurrentCamera()->GetPosition()));
	if (m_pModelUniform) m_pModelUniform->SetAsMat4(model);
	m_pMVPUniform->SetAsMat4(mvp);

}

MaterialType Light::render::SkeletonMaterial::GetType()
{
	static std::size_t type = typeid(SkeletonMaterial).hash_code();
	return type;
}

void Light::render::SkeletonMaterial::SetPipeline(Pipeline * pipeline)
{
	m_Pipeline.reset();
	m_Pipeline.reset(pipeline);
	this->GetUniform();

}

std::shared_ptr<Light::render::Material> Light::render::SkeletonMaterial::Clone()
{
	return std::shared_ptr<Material>(new SkeletonMaterial());
}

void Light::render::SkeletonMaterial::GetUniform()
{
	assert(m_Pipeline != nullptr);
	m_pModelUniform = m_Pipeline->GetParam(uMODEL);
	m_pMVPUniform = m_Pipeline->GetParam(uMVP);
	m_uTex = m_Pipeline->GetParam(uTex);
	m_uCubeTex = m_Pipeline->GetParam(uCubeTex);
	m_uCameraPos = m_Pipeline->GetParam(uCameraPos);

}
