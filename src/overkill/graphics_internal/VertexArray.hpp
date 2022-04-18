#pragma once
#include <vector>
#include <unordered_map>
#include <set>

#include "VertexLayout.hpp"

namespace OK
{

class VertexArray
{
public:
	VertexArray();

	inline bool valid() const { return id != 0; }
	inline GLuint ID() const { return id; }

    inline void bind() const
    {
        glBindVertexArray(id);
    }
    static inline void unbind()
    {
        glBindVertexArray(0);
    }
    inline void clear()
    {
        glDeleteVertexArrays(1, &id);
    }

private:
	GLuint id = 0;

	//std::unordered_map<GLuint> buffers();
    //std::unordered_map<VertexLayout*, std::set<VertexBuffer>> layouts;
	//list layouts, let buffers subscribe to one of them
	//std::vector<VertexLayout> layouts; 
};

}
