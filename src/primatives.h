#pragma once

#include "mesh.h"

class Box : public Mesh {
  public:
	Box();
	const std::string class_name() const override;
};
