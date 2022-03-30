#include "VertexArray.hpp"

namespace OK 
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
    }
}