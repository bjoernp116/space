
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <string>

#include "obj.h"
#include "spdlog/spdlog.h"
#include "../utils.h"

const std::string MESH_PATH = "./meshes/";

template <typename T> std::vector<T> ObjectFile::get_vertecies() const {
	const unsigned int size = vertecies.size();
	const T *ptr = reinterpret_cast<const T *>(vertecies.data());

	return std::vector<T>(ptr, ptr + size);
}
template std::vector<float> ObjectFile::get_vertecies<float>() const;
template std::vector<glm::vec3> ObjectFile::get_vertecies<glm::vec3>() const;

template <typename T> std::vector<T> ObjectFile::get_normals() const {
	const unsigned int size = normals.size();
	const T *ptr = reinterpret_cast<const T *>(normals.data());

	return std::vector<T>(ptr, ptr + size);
}

template std::vector<float> ObjectFile::get_normals<float>() const;
template std::vector<glm::vec3> ObjectFile::get_normals<glm::vec3>() const;

template <typename T> std::vector<T> ObjectFile::get_indecies() const {
	const unsigned int size = indecies.size();
	const T *ptr = reinterpret_cast<const T *>(indecies.data());

	return std::vector<T>(ptr, ptr + size);
}
template std::vector<unsigned int>
    ObjectFile::get_indecies<unsigned int>() const;
template std::vector<glm::uvec3> ObjectFile::get_indecies<glm::uvec3>() const;

ObjectFile::ObjectFile(const char *p) : File(MESH_PATH + p) {
	for (std::string s : read_lines()) {
		std::vector<std::string> tokens = split(s, ' ');
		if (tokens.size() < 3) {
			continue;
		}
		if (s.find("#") != std::string::npos) {
			continue;
		}
		std::vector<std::string> ft;
		std::copy_if(tokens.begin(),
		    tokens.end(),
		    std::back_inserter(ft),
		    [](std::string str) { return str.size() != 0; });

		if (ft.at(0) == "v") {
			if (ft.size() != 4) {
				spdlog::error("Expected vertex with 3 params, got {}",
				    ft.size());
			}
			vertecies.emplace_back(std::atof(ft.at(1).data()),
			    std::atof(ft.at(2).data()),
			    std::atof(ft.at(3).data()));
		}

		if (ft.at(0) == "vn") {
			if (ft.size() != 4) {
				spdlog::error("Expected vertex normal with 3 params, got {}",
				    ft.size());
			}
			normals.emplace_back(std::atof(ft.at(1).data()),
			    std::atof(ft.at(2).data()),
			    std::atof(ft.at(3).data()));
		}

		if (ft.at(0) == "f") {
			face_size = ft.size() - 1;
			if (ft.size() != 3 && ft.size() != 4) {
				spdlog::error("Expected index with 2-3 params, got {}",
				    ft.size() - 1);
			}
            glm::uvec3 pos(
                    std::atoi(ft.at(1).data()),
                    std::atoi(ft.at(2).data()),
                    std::atoi(ft.at(3).data())
                    );
            
			indecies.push_back(pos);
		}
	}
}

unsigned int ObjectFile::get_face_size() const {
	return face_size;
}
