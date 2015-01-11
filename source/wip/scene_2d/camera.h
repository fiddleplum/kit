#pragma once

#include "Entity.h"
#include <memory>

namespace Scene2D
{
	class Camera : public Entity
	{
	public:
		Camera();
		void setAspectRatio(float);
		void setMaxViewSize(float);
		virtual void setPosition(Vector2f) override;
		virtual void setOrientation(float) override;
		Vector2f getAppPosition(Vector2f worldPosition) const;
		Vector2f getWorldPosition(Vector2f appPosition) const;
		Matrix33f const & getProjection() const;
		Matrix33f const & getView() const;

	private:
		void updateProjection();
		void updateView();

		float aspectRatio;
		float maxViewSize;
		bool projectionNeedsUpdate;
		bool viewNeedsUpdate;
		Matrix44f projection;
		Matrix44f projectionInverse;
		Matrix44f view;
		Matrix44f viewInverse;
	};
}