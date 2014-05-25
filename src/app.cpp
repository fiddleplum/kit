#include "app.h"
#include "shader.h"
#include "texture.h"
#include "open_gl.h"
#include "gui\widget.h"
#include <fstream>

namespace kit
{
	namespace App
	{
		float lastTime = 0.0f;
		bool running = false;
		std::shared_ptr<app::Widget> widget = nullptr;
		std::shared_ptr<ResourceManager<Texture>> textureManager = nullptr;
		std::shared_ptr<ResourceManager<Shader>> shaderManager = nullptr;

		void handleOSEvents();
		void finishRender();

		void log(std::string const & text)
		{
			std::fstream logFile ("log.txt", std::ios::app);
			logFile << text << std::endl;		
		}

		void quit()
		{
			running = false;
		}

		std::shared_ptr<app::Widget> getWidget()
		{
			return widget;
		}

		void setWidget(std::shared_ptr<app::Widget> newWidget)
		{
			widget = newWidget;
			if(widget != nullptr)
			{
				widget->setMaxSize(getSize());
			}
		}

		std::shared_ptr<ResourceManager<Texture>> getTextureManager()
		{
			return textureManager;
		}

		std::shared_ptr<ResourceManager<Shader>> getShaderManager()
		{
			return shaderManager;
		}

		void start(std::vector<std::string> const & params)
		{
			try
			{
				// Initialize variables.
				glInitialize();
				textureManager = std::make_shared<ResourceManager<Texture>>();
				shaderManager = std::make_shared<ResourceManager<Shader>>();

				// Call user-defined startup function.
				onStartup(params); 

				// Do loop.
				running = true;
				lastTime = getTime();
				while(running)
				{
					float newTime = getTime();
					float deltaTime = newTime - lastTime; // calculate the last frame's duration
					lastTime = newTime;
					// Process Events
					if(running)
					{
						handleOSEvents();
					}
					// Update Frame
					if(running)
					{
						handleEvent(UpdateEvent());
					}
					// Render Everything
					if(running)
					{
						render();
					}
				}
			}
			catch(std::runtime_error const & err)
			{
				showMessage(err.what());
			}

			widget.reset(); // clear the widget to release resources
			onShutdown(); // calls user-defined function

			textureManager.reset();
			shaderManager.reset();
		}

		void handleEvent(Event const & event)
		{
			if(event.type == Event::Resize)
			{
				if(getWidget() != nullptr)
				{
					getWidget()->setMaxSize(getSize());
				}
			}
			else
			{
				handleAppEvent(event);
				if(widget != nullptr)
				{
					widget->handleEvent(event);
				}
			}
		}

		void render()
		{
			glDisable(GL_DEPTH_TEST);
			glDepthFunc(GL_GREATER);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glClearColor(0, 0, 0, 1);
			glClearDepth(0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Vector2i windowSize = getSize();
			glViewport(0, 0, windowSize[0], windowSize[1]);
			if(widget != nullptr)
			{
				widget->render();
			}

			finishRender();
		}
	}
}

