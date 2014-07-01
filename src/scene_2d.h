#pragma once

#include <set>
#include <memory>

namespace kit
{
	class Event;
	class Camera2D;
	class Frame2D;
	class Model2D;

	namespace scene
	{
		class Controller;
	}

	class Entity2D
	{
	public:
		Entity2D(std::shared_ptr<Model2D> model, std::shared_ptr<Frame2D> frame) { this->model = model; this->frame = frame; }
		std::shared_ptr<Model2D> model;
		std::shared_ptr<Frame2D> frame;
	};

	class Scene2D
	{
	public:
		Scene2D();

		void addCamera(std::shared_ptr<Camera2D> camera);

		void removeCamera(std::shared_ptr<Camera2D> camera);

		void addEntity(std::shared_ptr<Entity2D> entity);

		void removeEntity(std::shared_ptr<Entity2D> entity);

		void addController(std::shared_ptr<scene::Controller> controller);

		void removeController(std::shared_ptr<scene::Controller> controller);
		
		// Called by Camera.
		void handleEvent(Event const & event);

		// Called by Camera.
		void render(std::shared_ptr<Camera2D> camera);

	private:
		class Compare
		{
		public:
			bool operator ()(std::shared_ptr<Entity2D> const & entity0, std::shared_ptr<Entity2D> const & entity1);
		};

		Scene2D(Scene2D const &);
		Scene2D & operator =(Scene2D const &);

		std::set<std::shared_ptr<Camera2D>> cameras;
		std::set<std::shared_ptr<Entity2D>, Compare> entities;
		std::set<std::shared_ptr<scene::Controller>> controllers;
	};
}

