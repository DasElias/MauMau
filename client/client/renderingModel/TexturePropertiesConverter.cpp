#include "TexturePropertiesConverter.h"
#include <shared/utils/Logger.h>

namespace card {
	GLenum convertMinFilter(TextureMinFilter val) {
		switch(val) {
			case TextureMinFilter::NEAREST:
				return GL_NEAREST;
			case TextureMinFilter::LINEAR:
				return GL_LINEAR;
			case TextureMinFilter::NEAREST_MIPMAP_NEAREST:
				return GL_NEAREST_MIPMAP_NEAREST;
			case TextureMinFilter::NEAREST_MIPMAP_LINEAR:
				return GL_NEAREST_MIPMAP_LINEAR;
			case TextureMinFilter::LINEAR_MIPMAP_NEAREST:
				return GL_LINEAR_MIPMAP_NEAREST;
			case TextureMinFilter::LINEAR_MIPMAP_LINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;
			default:
				log(LogSeverity::WARNING, "Unknown value for TextureMinFilter in convertMinFilter");
				return 0;
		}
	}
	GLenum convertMagFilter(TextureMagFilter val) {
		switch(val) {
			case TextureMagFilter::NEAREST:
				return GL_NEAREST;
			case TextureMagFilter::LINEAR:
				return GL_LINEAR;
			default:
				log(LogSeverity::WARNING, "Unknown value for TextureMagFilter in convertMagFilter");
				return 0;
		}
	}
	GLenum convertWrapValue(TextureWrap val) {
		switch(val) {
			case TextureWrap::CLAMP_TO_BORDER:
				return GL_CLAMP_TO_BORDER;
			case TextureWrap::CLAMP_TO_EDGE:
				return GL_CLAMP_TO_EDGE;
			case TextureWrap::MIRRORED_REPEAT:
				return GL_MIRRORED_REPEAT;
			case TextureWrap::REPEAT:
				return GL_REPEAT;
			case TextureWrap::MIRROR_CLAMP_TO_EDGE:
				return GL_MIRROR_CLAMP_TO_EDGE;
			default:
				log(LogSeverity::WARNING, "Unknown value for TextureWrap in convertWrapValue");
				return 0;
		}
	}
}