#pragma once

#include <vector>
#include "glm/fwd.hpp"
#include "texture.h"

class VertexBuffer {
  public:
	unsigned int id;

	VertexBuffer(const void *data, unsigned int size);
	VertexBuffer();

	~VertexBuffer();

	void add_data(const std::vector<float> &data) const;
	void add_data(const std::vector<glm::vec3> &data) const;

	// Delete copy constructor and copy assignment
	VertexBuffer(const VertexBuffer &) = delete;
	VertexBuffer &operator=(const VertexBuffer &) = delete;

	// Implement move constructor and move assignment if needed
	VertexBuffer(VertexBuffer &&other) noexcept;
	VertexBuffer &operator=(VertexBuffer &&other) noexcept;
};

class IndexBuffer {
  public:
	unsigned int id;
	unsigned int count;

	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();
	IndexBuffer();

	void add_data(const std::vector<unsigned int> &data) const;
	void add_data(const std::vector<glm::uvec3> &data) const;
	void add_data(const std::vector<glm::uvec2> &data) const;

	// Delete copy constructor and copy assignment
	IndexBuffer(const IndexBuffer &) = delete;
	IndexBuffer &operator=(const IndexBuffer &) = delete;

	// Implement move constructor and move assignment if needed
	IndexBuffer(IndexBuffer &&other) noexcept;
	IndexBuffer &operator=(IndexBuffer &&other) noexcept;
};

class FrameBuffer {
  public:
	unsigned int id;

	FrameBuffer();
	~FrameBuffer();
	void attach_texture(Texture texture) const;
	void bind() const;
	void unbind() const;
};

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;

	unsigned int size() const;
};

class VertexBufferLayout {
  private:
	std::vector<VertexBufferElement> elements;

  public:
	unsigned int stride;

	VertexBufferLayout();
	std::vector<VertexBufferElement> get_elements() const;

	template <typename T> void push(unsigned int count);
};

class VertexArray {
  public:
	unsigned int id;
	VertexArray();
	~VertexArray();

	void add_buffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
	void add_buffer(const IndexBuffer &ib);
	void bind() const;
	void unbind() const;
};
