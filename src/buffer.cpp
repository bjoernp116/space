#include <cassert>
#include <glad/glad.h>
#include "buffer.h"
#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "texture.h"
#include <spdlog/spdlog.h>
#include <vector>

#define INT2VOIDP(i) (const void *)(uintptr_t)(i)

VertexBuffer::VertexBuffer() {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	spdlog::debug("VB{0}: created", id);
}

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	glNamedBufferData(id, size, data, GL_STATIC_DRAW);
	for (int i = 0; i < size; i += 1)
		spdlog::debug("VB{0}, index {1}: {2}",
		    id,
		    i,
		    reinterpret_cast<const float *>(data)[i]);

	spdlog::debug("VB{0}: created and filled, vertecies: {1}", id, size);
}

void VertexBuffer::add_data(const std::vector<glm::vec3> &data) const {
	glNamedBufferData(id,
	    data.size() * sizeof(glm::vec3),
	    (void *)data.data(),
	    GL_STATIC_DRAW);
	spdlog::debug("VB{3}: Vertex size: {0} * {1} = {2}",
	    data.size(),
	    sizeof(glm::vec3),
	    data.size() * sizeof(glm::vec3),
	    id);
}

void VertexBuffer::add_data(const std::vector<float> &data) const {
	glNamedBufferData(id,
	    data.size() * sizeof(float),
	    (void *)data.data(),
	    GL_STATIC_DRAW);
	spdlog::debug("VB{3}: Vertex size: {0} * {1} = {2}",
	    data.size(),
	    sizeof(glm::vec3),
	    data.size() * sizeof(glm::vec3),
	    id);
}

VertexBuffer::~VertexBuffer() {
	spdlog::warn("VB{0}: deleted", id);
	glDeleteBuffers(1, &id);
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept {
	id = other.id;
	other.id = 0; // transfer ownership
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
	spdlog::warn("VB{0}: deleted", id);
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
	spdlog::debug("IB{0}: created and filled, indecies: {1}", id, count);
}

IndexBuffer::IndexBuffer() {
	glCreateBuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	spdlog::debug("IB{0}: created", id);
}

void IndexBuffer::add_data(const std::vector<unsigned int> &data) const {
	unsigned int size = data.size() * sizeof(glm::uvec3);
	glNamedBufferData(id, size, data.data(), GL_STATIC_DRAW);
	spdlog::debug("IB{3}: Index size: {0} * {1} = {2}",
	    data.size(),
	    sizeof(glm::uvec3),
	    data.size() * sizeof(glm::uvec3),
	    id);
}

void IndexBuffer::add_data(const std::vector<glm::uvec3> &data) const {
	unsigned int size = data.size() * sizeof(glm::uvec3);
	glNamedBufferData(id, size, data.data(), GL_STATIC_DRAW);
	spdlog::debug("IB{3}: Index size: {0} * {1} = {2}",
	    data.size(),
	    sizeof(glm::uvec3),
	    data.size() * sizeof(glm::uvec3),
	    id);
}

void IndexBuffer::add_data(const std::vector<glm::uvec2> &data) const {
	unsigned int size = data.size() * sizeof(glm::uvec2);
	glNamedBufferData(id, size, data.data(), GL_STATIC_DRAW);
	spdlog::debug("IB{3}: Index size: {0} * {1} = {2}",
	    data.size(),
	    sizeof(glm::uvec2),
	    data.size() * sizeof(glm::uvec2),
	    id);
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
	spdlog::warn("IB{0}: deleted", id);
	glDeleteBuffers(1, &id);
}

FrameBuffer::FrameBuffer() {
	glCreateFramebuffers(1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	spdlog::debug("FB{0}: created!", id);
}

void FrameBuffer::attach_texture(Texture texture) const {

	glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, texture.id, 0);
	if (glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER) !=
	    GL_FRAMEBUFFER_COMPLETE) {
		spdlog::error("FB{0}: failed to attach TEX{1}!", id, texture.id);
	} else {
		spdlog::debug("FB{0}: attached TEX{1}!", id, texture.id);
	}
}

void FrameBuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	// spdlog::debug("FB{0}: bound!", id);
}

void FrameBuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// spdlog::debug("FB{0}: unbound!", id);
}

FrameBuffer::~FrameBuffer() {
	spdlog::warn("FB{0}: deleted", id);
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
	spdlog::debug("VAO{0}: created", id);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &id);
}

void VertexArray::add_buffer(const VertexBuffer &vb,
    const VertexBufferLayout &layout) {

	spdlog::debug("VAO{0}: <- VB{1}", id, vb.id);

	glVertexArrayVertexBuffer(id, 0, vb.id, 0, layout.stride);

	const auto &elements = layout.get_elements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto &element = elements[i];
		spdlog::debug("VAO{0}: FORMAT LAYOUT {1}", id, 0);

		glEnableVertexArrayAttrib(id, i);

		glVertexArrayAttribFormat(id,
		    i,
		    element.count,
		    element.type,
		    element.normalized,
		    offset);
		/*std::cout << "count: " << element.count << std::endl;
		std::cout << "type: " << element.type << std::endl;
		std::cout << "normalized: " << (element.normalized ? GL_TRUE : GL_FALSE)
		          << std::endl;
		std::cout << "stride: " << layout.stride << std::endl;
		std::cout << "offset: " << offset << std::endl;*/
		glVertexArrayAttribBinding(id, i, 0);

		offset += element.count * element.size();
	}
}

void VertexArray::add_buffer(const IndexBuffer &ib) {
	spdlog::debug("VAO{0}: <- IB{1}", id, ib.id);
	glVertexArrayElementBuffer(id, ib.id);
}

void VertexArray::bind() const {
	glBindVertexArray(id);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
