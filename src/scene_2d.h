#pragma once

#include "frame_2d.h"
#include "model_2d.h"
#include <set>
#include <memory>

class Event;
class Camera2D;
class Controller;

class Entity2D
{
public:
	Model2D model;
	Frame2D frame;
};

class Scene2D
{
public:
	Scene2D();

	void addCamera(std::shared_ptr<Camera2D> camera);

	void removeCamera(std::shared_ptr<Camera2D> camera);

	void addEntity(std::shared_ptr<Entity2D> entity);

	void removeEntity(std::shared_ptr<Entity2D> entity);

	void addController(std::shared_ptr<Controller> controller);

	void removeController(std::shared_ptr<Controller> controller);
		
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
	std::set<std::shared_ptr<Controller>> controllers;
};

