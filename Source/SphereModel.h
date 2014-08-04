#pragma once

#include "model.h"

class SphereModel : public Model
{
public:
	SphereModel(Model* parent = NULL);
	~SphereModel(void);

	virtual void Update(float dt);
	void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	void Initialize(Model* p);
	
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int mSphereNumVertices;
};

