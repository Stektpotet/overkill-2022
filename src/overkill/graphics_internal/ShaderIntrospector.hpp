#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

namespace OK { namespace ShaderIntrospector
{

    //UNIFORM BLOCKS
    auto getActiveBlockCount(const GLuint program)->GLint;
    auto getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex) -> const std::vector<GLint>;
    auto getUnifromBlockName(const GLuint program, const GLint uBlockIndex) -> const std::string;
    auto getUniformBlockIndex(const GLuint program, const std::string& name) -> GLuint;

    //UNIFROMS
    auto getActiveUniformCount(const GLuint program) -> GLint;
    auto getUniformMaxNameLength(const GLuint programId) -> GLsizei; 
    auto getUniformName(const GLuint programId, const GLuint uniformIndex, const GLsizei nameMaxLength) ->std::string;

    //ATTRIBUTES
    auto getActiveAttribCount(const GLuint program) -> GLint;
    auto getAttributeMaxNameLength(const GLuint programId) -> GLsizei;
    auto getAttribName(const GLuint programId, const GLuint attribIndex, const GLsizei nameMaxLength)->std::string;

    auto checkCompileStatus(const GLuint shaderid) -> GLuint;
    auto checkLinkStatus(const GLuint programid) -> GLuint;

}}