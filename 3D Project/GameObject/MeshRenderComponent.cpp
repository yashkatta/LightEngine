#include "pch.h"

const char* MeshRenderComponent::Name = "MeshRenderComponent";

void MeshRenderComponent::VPostInit(void)
{
}

bool MeshRenderComponent::VInit(const tinyxml2::XMLElement* pData)
{
	if(!pData) return false;

	const tinyxml2::XMLElement* pModelPath = pData->FirstChildElement("Model");
	
	const char* pFileName = pModelPath->Attribute("File");
	if (pFileName)
	{
		ModelCache* pModel = m_Context->m_pResources->GetModel(pFileName);
		if (!pModel)
		{
			return false;
		}
		for (size_t i = 0; i < pModel->pMeshs.size(); i++)
		{
			m_MeshList.push_back(pModel->pMeshs[i].get());
		}
	}
	

	const tinyxml2::XMLElement* pShader = pData->FirstChildElement("Shader");
	if (pShader)
	{
		m_pShader = m_Context->m_pResources->GetShader(pShader->Attribute("name"));
		if (m_pShader == nullptr)
			E_ERROR("Can not find shader name: " + string(pShader->Attribute("name")));
	}
	return true;

}

tinyxml2::XMLElement * MeshRenderComponent::VGenerateXml(tinyxml2::XMLDocument * p)
{
	// Nothing here. 
	// Update late.
	return nullptr;
}

void MeshRenderComponent::Render(Scene* pScene)
{
	if (m_MeshList.empty()) return;
	m_pShader->SetupRender(pScene, m_pOwner);

	RenderAPICore* pRender = m_Context->m_pRenderer.get();

	
	m_pShader->SetUniform("gMaterial.Ka", m_Material.Ka);
	m_pShader->SetUniform("gMaterial.Kd", m_Material.Kd);
	m_pShader->SetUniform("gMaterial.Ks", m_Material.Ks);
	m_pShader->SetUniform("gMaterial.exp", m_Material.exp);

	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		pRender->SetTexture(m_MeshList[i]->Tex);

		// ------- Render mesh ----------
		pRender->SetVertexArrayBuffer(m_MeshList[i]->VAO);
		pRender->SetDrawMode(m_MeshList[i]->Topology);
		pRender->DrawElement(m_MeshList[i]->NumIndices, GL_UNSIGNED_INT, 0);
	}

	if (0)
	{
		mat4 globalTransform = m_pOwner->VGetGlobalTransform();
		PVAnimationComponent* ac = m_pOwner->GetComponent<PVAnimationComponent>("PVAnimationComponent");
//		if (ac) ac->DrawSkeleton(pScene->GetDebug(), globalTransform);
	}

}

vector<IMesh*>& MeshRenderComponent::GetMeshList()
{
	return m_MeshList;
}
