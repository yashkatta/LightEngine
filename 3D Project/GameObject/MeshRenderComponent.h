#pragma once
#include "pch.h"

// This component using to render model for actor
// Store vector of IMesh 
class MeshRenderComponent : public ActorComponent
{
private:
	vector<IMesh*>			m_MeshList;
	Material				m_Material;
	Shader					*m_pShader;
public:
	static const char* Name;
	virtual const char* VGetName() const { return Name; }
	MeshRenderComponent(void) {}
	MeshRenderComponent(const vector<IMesh*>& v) { m_MeshList = v; }
	virtual void VPostInit(void);
	virtual bool VInit(tinyxml2::XMLElement* pData);
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument*p);

	// Question need to answer
	// Let this component render or just return m_MeshList for other sub-system
	// For easy now just render it
	// Request mesh and texture
	void Render(Scene*);
	// Get Material for this render
	Material GetMaterial();
};