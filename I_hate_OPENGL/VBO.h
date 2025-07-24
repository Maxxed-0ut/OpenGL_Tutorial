#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VertexBuffer
{
private:

	unsigned int m_RendererID;
public:

	VertexBuffer(const void* data, unsigned int size);

	~VertexBuffer();

	void Bind() const;

	void Unbind() const;


};

#endif
