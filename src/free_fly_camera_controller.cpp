#include "free_fly_camera_controller.h"
#include "scene/camera_internal.h"
#include <kit/event.h>

namespace kit
{
	FreeFlyCameraController::FreeFlyCameraController()
	{
		lookSensitivity = .05f;
		moveSensitivity = 1.0f;
		moving = Vector3i::zero();
		yaw = 0;
		pitch = 0;
		looking = false;
	}

	float FreeFlyCameraController::getMoveSensitivity() const
	{
		return moveSensitivity;
	}
	
	void FreeFlyCameraController::setMoveSensitivity(float newMoveSensitivity)
	{
		moveSensitivity = newMoveSensitivity;
	}

	float FreeFlyCameraController::getLookSensitivity() const
	{
		return lookSensitivity;
	}

	void FreeFlyCameraController::setLookSensitivity(float newLookSensitivity)
	{
		lookSensitivity = newLookSensitivity;
	}

	void FreeFlyCameraController::setCamera(std::shared_ptr<Camera> camera)
	{
		this->camera = camera;
	}

	void FreeFlyCameraController::handleEvent(Event const & event)
	{
		switch(event.type)
		{
		case Event::Keyboard:
			{
				KeyboardEvent const & kEvent = event.as<KeyboardEvent>();
				switch(kEvent.key)
				{
				case KeyboardEvent::W:
					{
						if(kEvent.pressed)
						{
							moving[1]++;
						}
						else
						{
							moving[1]--;
						}
						break;
					}
				case KeyboardEvent::S:
					{
						if(kEvent.pressed)
						{
							moving[1]--;
						}
						else
						{
							moving[1]++;
						}
						break;
					}
				case KeyboardEvent::D:
					{
						if(kEvent.pressed)
						{
							moving[0]++;
						}
						else
						{
							moving[0]--;
						}
						break;
					}
				case KeyboardEvent::A:
					{
						if(kEvent.pressed)
						{
							moving[0]--;
						}
						else
						{
							moving[0]++;
						}
						break;
					}
				case KeyboardEvent::E:
					{
						if(kEvent.pressed)
						{
							moving[2]++;
						}
						else
						{
							moving[2]--;
						}
						break;
					}
				case KeyboardEvent::Q:
					{
						if(kEvent.pressed)
						{
							moving[2]--;
						}
						else
						{
							moving[2]++;
						}
						break;
					}
				}
				break;
			}
		case Event::MouseButton:
			{
				MouseButtonEvent const & mbEvent = event.as<MouseButtonEvent>();
				if(mbEvent.button == MouseButtonEvent::Right)
				{
					looking = mbEvent.pressed;
				}
				break;
			}
		case Event::MouseMove:
			{
				MouseMoveEvent const & mmEvent = event.as<MouseMoveEvent>();
				if(looking)
				{
					yaw += lookSensitivity * mmEvent.relative[0];
					pitch -= lookSensitivity * mmEvent.relative[1];
				}
				break;
			}
		case Event::Update:
			{
				if(camera == nullptr)
				{
					return;
				}
			
				camera->setPosition(camera->getPosition() + moveSensitivity * (camera->getOrientation().getAxis(0) * (float)moving[0] + camera->getOrientation().getAxis(1) * (float)moving[1] + camera->getOrientation().getAxis(2) * (float)moving[2]));
				camera->setOrientation(Quaternionf(yaw, pitch, 0));
				break;
			}
		}
	}
}

