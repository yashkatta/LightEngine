#pragma once

class EffectSystem: public ISubSystem
{
private:
	GLuint VAO, VBO;
	Mesh m_QuadMesh;
	std::vector<Sprite> m_SpriteLists;

	std::vector<SpriteAnim*> m_List2;
	Shader* m_pShader;
public:
	EffectSystem() = default;
	~EffectSystem() = default;
	virtual void	Init(Context* c);
	virtual void	ShutDown();

	void			Update(float dt);
	void			Render(Scene* pScene);

	void			AddSprite(Sprite a);
	void			AddSprite(SpriteAnim* a);
};