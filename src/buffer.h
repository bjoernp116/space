#pragma once

#include <vector>

class VertexBuffer {
  public:
	unsigned int id;

	VertexBuffer(const void *data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

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

	void bind() const;
	void unbind() const;

	// Delete copy constructor and copy assignment
	IndexBuffer(const IndexBuffer &) = delete;
	IndexBuffer &operator=(const IndexBuffer &) = delete;

	// Implement move constructor and move assignment if needed
	IndexBuffer(IndexBuffer &&other) noexcept;
	IndexBuffer &operator=(IndexBuffer &&other) noexcept;
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
	void bind() const;
	void unbind() const;
};
