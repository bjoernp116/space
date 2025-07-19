#include "buffer.h"
#include "mesh.h"

Mesh::Mesh(const void *v_data,
    unsigned int v_size,
    const unsigned int *i_data,
    unsigned int i_count)
: vb(v_data, v_size), ib(i_data, i_count) {

}
