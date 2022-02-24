//#pragma once
//#include <glad/glad.h>
//#include <vector>
//#include <unordered_map>
//#include <initializer_list>
//
//namespace OK 
//{
//
//class UniformBuffer
//{
//private:
//    GLuint m_id;
//    std::unordered_map<std::string, GLuint> m_uniforms;
//public:
//
//    UniformBuffer(const char *name, std::initializer_list<std::pair<const GLsizeiptr, const void*>> data);
//
//    GLuint get() const;
//
//    void clean();
//
//    void bind() const;
//    void unbind() const;
//
//    inline GLuint blockSize()
//    {
//        return m_blockLayout.size();
//    }
//
//    GLuint getUniformIndex(const std::string& name) const;
//    void update(const GLintptr index, GLsizeiptr size, const void *data);
//};
//
//}
//
///*
//
//UniformBuffer("Matrices", {
//        {"MVP", glm::mat4},
//        {"NRM", glm::mat3},
//        {"eye", glm::vec3}
//    }
//
//*/