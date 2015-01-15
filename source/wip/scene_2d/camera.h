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
		virtual void setPosition(Coord2f) override;
		virtual void setOrientation(float) override;
		Coord2f getAppPosition(Coord2f worldPosition) const;
		Coord2f getWorldPosition(Coord2f appPosition) const;
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