#pragma once
#include <glad/glad.h>
#include "imgui.h"

class Texture {
  public:
	unsigned int id;

	Texture(GLint wrap = GL_REPEAT);

	Texture operator=(Texture other) noexcept;

	void set_image(
	    unsigned char *data, unsigned int width, unsigned int height) const;
	void set_size(unsigned int w, unsigned int h);

	ImVec2 get_size() const;

  private:
	unsigned int width;
	unsigned int height;
};
