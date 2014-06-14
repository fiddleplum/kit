#include "object.h"

namespace kit
{
	namespace scene
	{
		namespace internal
		{
			Vector3f const & Object::getPosition () const
			{
				return frame.getPosition();
			}

			void Object::setPosition (Vector3f position)
			{
				frame.setPosition(position);
			}

			Quaternionf const & Object::getOrientation () const
			{
				return frame.getOrientation();
			}

			void Object::setOrientation (Quaternionf orientation)
			{
				frame.setOrientation(orientation);
			}

			Vector3f Object::getScale () const
			{

			}

			void Object::setScale (Vector3f scale)
			{
			}

			void Object::setModel (std::string const & filename)
			{
			}

			void Object::setModelAsSprite (std::string const & texture, Recti const & textureCoords)
			{
			}
		}
	}
}

