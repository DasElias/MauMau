#pragma once
#include <array>
#include "shaderPrograms/VertexFragmentShaderProgram.h"
#include "../renderingModel/DataVertexArrayObject.h"
#include "../renderingModel/InstancedRenderingVbo.h"
#include "../model/particles/ParticleList.h"
#include "../renderingModel/Viewport.h"
#include "../renderingModel/ProjectionMatrix.h"

namespace card {
	class ParticleRenderer {
        class ParticleShader : public VertexFragmentShaderProgram {
            // ----------------------------------------------------------------------
            // --------------------------------FIELDS--------------------------------
            // ----------------------------------------------------------------------
            private:
                uint32_t location_projectionMatrix;
                uint32_t location_textureRowsCount;

            // ----------------------------------------------------------------------
            // -----------------------------CONSTRUCTORS-----------------------------
            // ----------------------------------------------------------------------
            public:
                ParticleShader();

            // ----------------------------------------------------------------------
            // -------------------------------METHODS--------------------------------
            // ----------------------------------------------------------------------
            public:
                void loadProjectionMatrix(glm::mat4x4 projectionMatrix);
                void loadTextureRowsCount(int rows);

        };




        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static std::vector<float> const VERTICES;
            static uint32_t const MAX_PARTICLES = 10000;
            // 16 floats for modelview-matrix, 4 for texture offset and 1 for texture blend
            static uint32_t const DATA_SIZE_PER_PARTICLE = 16 + 4 + 1;
            
        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            ParticleShader shader;
            DataVertexArrayObject quadVao;
            InstancedRenderingVbo vbo;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ParticleRenderer();
            ParticleRenderer(const ParticleRenderer&) = delete;

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void render(const ParticleList& particles, Viewport& viewport, ProjectionMatrix& projectionMatrix);

        private:
            void updateVboData(const std::vector<Particle>& particles, Viewport& viewport);
            void pushMatrixDataInVector(std::vector<float>& data, glm::mat4 matrix);

	};
}