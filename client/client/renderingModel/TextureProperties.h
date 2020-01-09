#pragma once

namespace card {
	enum class TextureMinFilter {
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR
	};
	enum class TextureMagFilter {
		NEAREST,
		LINEAR,
	};
	enum class TextureWrap {
		CLAMP_TO_BORDER,
		CLAMP_TO_EDGE,
		MIRRORED_REPEAT,
		REPEAT,
		MIRROR_CLAMP_TO_EDGE
	};
}