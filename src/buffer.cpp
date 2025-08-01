#include <cassert>
#include <glad/glad.h>
#include "buffer.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>

#define INT2VOIDP(i) (const void *)(uintptr_t)(i)

VertexBuffer::VertexBuffer() {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
}

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	glNamedBufferData(id, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::add_data(const std::vector<float> &data) const {
	glNamedBufferData(id,
	    data.size() * sizeof(float),
	    data.data(),
	    GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &id);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept {
	id = other.id;
	other.id = 0; // transfer ownership
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &id); // clean old buffer
		id = other.id;
		other.id = 0;
	}
	return *this;
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : count(count) {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	glNamedBufferData(id, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer() {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
}

void IndexBuffer::add_data(const std::vector<unsigned int> &data) const {
	unsigned int size = data.size() * sizeof(unsigned int);
	glNamedBufferData(id, size, data.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept {
	id = other.id;
	other.id = 0; // transfer ownership
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &id); // clean old buffer
		id = other.id;
		other.id = 0;
	}
	return *this;
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &id);
}

VertexBufferLayout::VertexBufferLayout() : stride(0) {}

std::vector<VertexBufferElement> VertexBufferLayout::get_elements() const {
	return elements;
}

template <> void VertexBufferLayout::push<float>(unsigned int count) {
	VertexBufferElement element = {GL_FLOAT, count, false};
	elements.push_back(element);
	stride += element.size() * count;
}

template <> void VertexBufferLayout::push<unsigned int>(unsigned int count) {
	VertexBufferElement element = {GL_UNSIGNED_INT, count, false};
	elements.push_back(element);
	stride += element.size() * count;
}

template <> void VertexBufferLayout::push<unsigned char>(unsigned int count) {
	VertexBufferElement element = {GL_UNSIGNED_BYTE, count, true};
	elements.push_back(element);
	stride += element.size() * count;
}

unsigned int VertexBufferElement::size() const {
	switch (type) {
	case GL_FLOAT:
		return 4;
	case GL_UNSIGNED_INT:
		return 4;
	case GL_UNSIGNED_BYTE:
		return 1;
	}
	assert(false);
	return 0;
}

VertexArray::VertexArray() {
	glCreateVertexArrays(1, &id);
	glBindVertexArray(id);
	spdlog::debug("VAO: VAO CREATED: {}", id);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &id);
}

void VertexArray::add_buffer(const VertexBuffer &vb,
    const VertexBufferLayout &layout) {

	spdlog::debug("VAO: ADD BUFFER!");

	glVertexArrayVertexBuffer(id, 0, vb.id, 0, layout.stride);

	const auto &elements = layout.get_elements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto &element = elements[i];
		spdlog::debug("VAO: FORMAT LAYOUT {}", 0);

		glEnableVertexArrayAttrib(id, 0);

		glVertexArrayAttribFormat(id,
		    0,
		    element.count,
		    element.type,
		    element.normalized,
		    0);
		/*std::cout << "count: " << element.count << std::endl;
		std::cout << "type: " << element.type << std::endl;
		std::cout << "normalized: " << (element.normalized ? GL_TRUE : GL_FALSE)
		          << std::endl;
		std::cout << "stride: " << layout.stride << std::endl;
		std::cout << "offset: " << offset << std::endl;*/
		glVertexArrayAttribBinding(id, 0, 0);

		offset += element.count * element.size();
	}
}

void VertexArray::add_buffer(const IndexBuffer &ib) {
	glVertexArrayElementBuffer(id, ib.id);
}

void VertexArray::bind() const {
	glBindVertexArray(id);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
