#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "TextureProperties.h"

namespace card {
	GLenum convertMinFilter(TextureMinFilter val);
	GLenum convertMagFilter(TextureMagFilter val);
	GLenum convertWrapValue(TextureWrap val);

}