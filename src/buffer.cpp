#include <cassert>
#include <glad/glad.h>
#include "buffer.h"
#include "utils.h"
#include <iostream>
#include <vector>

#define INT2VOIDP(i) (const void *)(uintptr_t)(i)

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
	glGenBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	GL_ERR();
	glBindBuffer(GL_ARRAY_BUFFER, id);
	GL_ERR();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	GL_ERR();
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
	GL_ERR();
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GL_ERR();
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &id);
	GL_ERR();
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
	glGenBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	GL_ERR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	GL_ERR();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	    count * sizeof(unsigned int),
	    data,
	    GL_STATIC_DRAW);
	GL_ERR();
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	GL_ERR();
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL_ERR();
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
	GL_ERR();
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
	glGenVertexArrays(1, &id);
	GL_ERR();
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &id);
	GL_ERR();
}

void VertexArray::bind() const {
	glBindVertexArray(id);
	GL_ERR();
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
	GL_ERR();
}

void VertexArray::add_buffer(const VertexBuffer &vb,
    const VertexBufferLayout &layout) {
	bind();
	vb.bind();

	const auto &elements = layout.get_elements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto &element = elements[i];
		glEnableVertexAttribArray(i);
		GL_ERR();
		std::cout << "count: " << element.count << std::endl;
		std::cout << "type: " << element.type << std::endl;
		std::cout << "normalized: " << (element.normalized ? GL_TRUE : GL_FALSE)
		          << std::endl;
		std::cout << "stride: " << layout.stride << std::endl;
		std::cout << "expected stride: " << 3 * sizeof(float) << std::endl;
		std::cout << "offset: " << offset << std::endl;
		glVertexAttribPointer(i,
		    element.count,
		    element.type,
		    element.normalized ? GL_TRUE : GL_FALSE,
		    layout.stride,
		    INT2VOIDP(offset));
		GL_ERR();
		offset += element.count * element.size();
	}
}
