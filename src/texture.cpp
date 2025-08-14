
#include "texture.h"
#include <spdlog/spdlog.h>
#include <cassert>

Texture::Texture(GLint wrap) {
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	assert(id != 0 && "Failed to generate VertexBuffer ID");
	spdlog::debug("TEX{0}: created!", id);
}
Texture Texture::operator=(Texture other) noexcept {
	if (this != &other) {
		spdlog::debug("TEX{0}: transfered from TEX{1}!", id, other.id);
		glDeleteTextures(1, &id); // clean old buffer
		id = other.id;
		other.id = 0;
	}
	return *this;
}
void Texture::set_size(unsigned int w, unsigned int h) {
	width = w;
	height = h;

	glTextureStorage2D(id, 1, GL_RGBA8, w, h);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	spdlog::debug("TEX{0}: width {1}, height {2}", id, width, height);
}

ImVec2 Texture::get_size() const {
	return ImVec2(width, height);
}

void Texture::set_image(
    unsigned char *data, unsigned int width, unsigned int height) const {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D,
	    0,
	    GL_RGB,
	    width,
	    height,
	    0,
	    GL_RGB,
	    GL_UNSIGNED_BYTE,
	    data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
