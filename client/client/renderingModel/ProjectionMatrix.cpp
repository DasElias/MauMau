#include "ProjectionMatrix.h"

#include <cmath>

#include <shared/utils/MathUtils.h>
#include <egui/input/IOHandler.h>

namespace card {
	ProjectionMatrix::ProjectionMatrix(float fov, float zNear, float zFar) :
			zNear(zNear),
			zFar(zFar) {
		update(fov);
	}

	void ProjectionMatrix::update(float fov) {
		float currentAspectRatio = float(egui::getDisplayHandler().getWidth()) / float(egui::getDisplayHandler().getHeight());

		if(currentAspectRatio != lastAspectRatio || fov != lastFov) {
			lastAspectRatio = currentAspectRatio;
			lastFov = fov;

			projectionMatrix = glm::mat4x4(1);

			//update projection matrix
			float y_scale = float((1.0f / tanf(toRadiands(fov / 2.0f))) * currentAspectRatio);
			float x_scale = y_scale / currentAspectRatio;
			float frustumLength = zFar - zNear;

			projectionMatrix[0][0] = x_scale;
			projectionMatrix[1][1] = y_scale;
			projectionMatrix[2][2] = -((zFar + zNear) / frustumLength);
			projectionMatrix[2][3] = -1;
			projectionMatrix[3][2] = -((2 * zNear * zFar) / frustumLength);
			projectionMatrix[3][3] = 0;
		}
	}

	glm::mat4x4 ProjectionMatrix::getProjectionMatrix() const {
		return projectionMatrix;
	}

}

