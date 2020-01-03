#pragma once
#include <cstdint>
#include <glm/vec4.hpp>

namespace card {
	class FrameBufferObject {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int width;
			int height;

			std::uint32_t frameBufferId;
			std::uint32_t colorTextureId;
			std::uint32_t depthRenderBufferId;

			std::uint32_t multisamples;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			FrameBufferObject(int width, int height, int samples = 0);
			FrameBufferObject(const FrameBufferObject&) = delete;
			FrameBufferObject& operator=(const FrameBufferObject&) = delete;
			~FrameBufferObject();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void setupColorTexture();
			void setupDepthBuffer();

		public:
			void bind() const;
			void unbind() const;

			void clear();
			void clear(glm::vec4 color);

			int getColorTexture() const;
			int getWidth() const;
			int getHeight() const;

			void resolveToFbo(FrameBufferObject& outputFbo);
			void resolveToScreen();
	};
}
