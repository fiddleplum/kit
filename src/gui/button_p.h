//#pragma once
//
//#include "sprite_p.h"
//#include <kit/gui/button.h>
//#include "widget_p.h"
//
//namespace kit
//{
//	namespace gui
//	{
//		class ButtonP : virtual public Button, virtual public WidgetP
//		{
//		public:
//			ButtonP ();
//
//			Recti getBounds () const override;
//
//			void setPosition (Vector2i position) override;
//
//			void setMaxSize (Vector2i maxSize) override;
//
//			void handleEvent (Event const & event) override;
//
//			void render (Vector2i windowSize) override;
//
//			void setTexture (Ptr<Texture> texture);
//
//			void setTextureBounds (Recti bounds) override;
//
//			void setType (Type type) override;
//
//			void setShotInterval (float interval) override;
//
//			void setHoverFunction (std::function<void ()> hoverFunction) override;
//
//			void setUnhoverFunction (std::function<void ()> unhoverFunction) override;
//
//			void setPressFunction (std::function<void ()> pressFunction) override;
//
//			void setUnpressFunction (std::function<void ()> unpressFunction) override;
//
//		private:
//			void setSpriteTextureBoundsFromState ();
//
//			std::function<void ()> hoverFunction;
//			std::function<void ()> unhoverFunction;
//			std::function<void ()> pressFunction;
//			std::function<void ()> unpressFunction;
//			Type type;
//			float shotInterval;
//			bool hovered;
//			bool pressed;
//			bool toggled;
//			float timePressed;
//			OwnPtr<SpriteP> sprite;
//			Recti defaultTextureBounds;
//		};
//	}
//}
//
