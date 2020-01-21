#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include "ParticleTexture.h"
#include "Particle.h"

namespace card {
	class ParticleList {
        private:
            typedef std::map<ParticleTexture, std::vector<Particle>> ParticleMap;

        public:
            typedef ParticleMap::iterator iterator;
            typedef ParticleMap::const_iterator const_iterator;

        // ----------------------------------------------------------------------
        // ----------------------------STATIC-FIELDS-----------------------------
        // ----------------------------------------------------------------------
        private:
            static float const DENSITY;
            static float const MAX_DISTANCE;

        // ----------------------------------------------------------------------
        // --------------------------------FIELDS--------------------------------
        // ----------------------------------------------------------------------
        private:
            ParticleMap particles;

        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            void update(float delta, glm::vec3 cameraPosition);
            void add(Particle p);

            iterator begin();
            const_iterator begin() const;
            const_iterator cbegin() const;
            iterator end();
            const_iterator end() const;
            const_iterator cend() const;

        private:
            void removeParticleTexturesWithNoParticles();
           

	};
}