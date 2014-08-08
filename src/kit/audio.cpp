#include "audio.h"
#include "object_cache.h"
#include <SDL_mixer.h>

namespace kit
{
	namespace audio
	{
		class Resource
		{
		public:
			Resource(std::string const & filename)
			{
				m = Mix_LoadWAV(filename.c_str());
				if(!m)
				{
					throw std::runtime_error("Could not load " + filename);
				}
			}

			~Resource()
			{
				Mix_FreeChunk(m);
			}

			void play()
			{
				Mix_PlayChannel(-1, m, 0);
			}

		private:
			Mix_Chunk * m;
		};

		ObjectCache<Resource> resourceCache;

		void initialize()
		{
			int flagsInitted = Mix_Init(MIX_INIT_OGG);
			if((flagsInitted & MIX_INIT_OGG) == 0)
			{
				throw std::runtime_error("Could not initialize ogg audio");
			}
			if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 4096) == -1)
			{
				throw std::runtime_error("Could not open audio for mixer");
			}
			Mix_AllocateChannels(8);
		}

		void finalize()
		{
			resourceCache.clean();
			Mix_CloseAudio();
			Mix_Quit();
		}

		void play(std::string const & filename)
		{
			Ptr<Resource> resource = resourceCache.get(filename, filename);
			resource->play();			
		}
	}
}

