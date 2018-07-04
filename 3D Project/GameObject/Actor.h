#pragma once
#include <pch.h>
class Actor;
class ActorComponent;


class Actor: public IActor
{
	friend class ActorFactory;
public:
	typedef std::vector<std::unique_ptr<IActor>> ActorList;
	typedef std::map<ComponentId, std::unique_ptr<ActorComponent>> ActorComponents;


protected:
	ActorList				m_Children;
	Actor*					m_pParent;
	string					m_Name;
	string					m_Tag;
	ActorState				m_State;
	std::unique_ptr<TransformComponent>		m_TransformComponent;
	ActorId					m_id;					// unique id for the actor
	ActorComponents			m_components;	// all components this actor has

public:
	Actor(ActorId id);
	virtual ~Actor();

	virtual bool		Init(const tinyxml2::XMLElement* pData)override;
	virtual void		PostInit(void)override;
	virtual void		Destroy(void)override;

	virtual void		VSetName(string name)override { m_Name = name; }
	virtual string		VGetName() override {	return m_Name;	};
	virtual void		VSetTag(string tag)override { m_Tag=tag; };
	virtual string		VGetTag()override { return m_Tag; };
	virtual void		VSetTransform(const mat4& toWorld)override;
	virtual const mat4&	VGetTransform()const override;
	virtual mat4		VGetGlobalTransform()override;
	virtual HRESULT		VOnUpdate(Scene *, float elapsedMs)override;
	virtual HRESULT		VPostUpdate(Scene *)override;
	virtual HRESULT		VPreRender(Scene *pScene);
	virtual bool		VIsVisible(Scene *pScene) const override;
	virtual HRESULT		VRenderChildren(Scene *pScene)override;
	virtual HRESULT		VPostRender(Scene *pScene)override;
	virtual HRESULT		VRender(Scene *pScene)override;
	virtual void		VSetState(ActorState state)override;
	virtual ActorState	VGetState();
	virtual bool		VAddChild(std::unique_ptr<Actor> kid)override;
	virtual bool		VRemoveChild(ActorId id)override;
	virtual IActor*		VGetChild(int index)override;
	virtual IActor*		VGetChild(const string& name)override;
	virtual IActor*		VGetParent()override;
	virtual ActorId		VGetId(void) const override;

	template<class ComponentType>ComponentType* GetComponent(ComponentId id);
	template<class ComponentType>ComponentType* GetComponent(const char*  name)const ;

	template<class ComponentType>ComponentType* RemoveComponent(const char*  name);
	const ActorComponents*						GetComponents() { return &m_components; }
	TransformComponent*							GetTransform();

protected:

	void AddComponent(ActorComponent* pComponent);
	void SetTransformComponent(TransformComponent* pTC);
	
	
};

template<class ComponentType>
inline ComponentType * Actor::GetComponent(ComponentId id)
{
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second.get());
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}

template<class ComponentType>
inline ComponentType * Actor::GetComponent(const char * name)const
{
	ComponentId id = ActorComponent::GetIdFromName(name);
	ActorComponents::const_iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second.get());
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}

template<class ComponentType>ComponentType* Actor::RemoveComponent(const char*  name)
{
	ComponentId id = ActorComponent::GetIdFromName(name);
	ActorComponents::iterator findIt = m_components.find(id);
	if (findIt != m_components.end())
	{
		ActorComponent* pBase(findIt->second.release());
		m_components.erase(id);
		ComponentType* pWeakSub = static_cast<ComponentType*>(pBase);
		return pWeakSub;  // return the weak pointer
	}
	else
	{
		return nullptr;
	}
}