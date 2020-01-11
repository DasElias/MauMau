#include "MauMauCardStackMisalignmentGenerator.h"
#include <shared\utils\MathUtils.h>
#include <iostream>
#include <shared/utils/MathUtils.h>

namespace card {
	MauMauCardStackMisalignmentGenerator::MauMauCardStackMisalignmentGenerator(std::size_t precomputeNIndicies) :
			gen(getRandomDevice()()),
			dis(-1, 1) {

		randomNumbers.reserve(precomputeNIndicies);
		for(int i = 0; i < precomputeNIndicies; i++) {
			randomNumbers.push_back(dis(gen));
		}
	}
	glm::vec3 MauMauCardStackMisalignmentGenerator::computeRotationMisalignment(std::size_t index) {
		if(index >= randomNumbers.size()) randomNumbers.reserve(index);

		while(index >= randomNumbers.size()) {
			randomNumbers.push_back(dis(gen));
		}

		float rot = randomNumbers[index];
		rot *= PI/24;
	/*	if(index % 10 == 0) {
			rot = PI / 64;
		} else if(index % 5 == 0) {
			rot = PI / 32;
		} else if(index % 3 == 0) {
			rot = -PI / 64;
		} else if(index % 2 == 0) {
			rot = -PI / 32;
		}*/
		return glm::vec3(0, 0, rot);
	}
}