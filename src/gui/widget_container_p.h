//#pragma once
//
//#include "sprite_p.h"
//#include "button_p.h"
//#include "viewport_p.h"
//#include <kit/gui/widget_container.h>
//#include <map>
//#include <list>
//
//namespace kit
//{
//	namespace gui
//	{
//		class WidgetContainerP : virtual public WidgetContainer, virtual public WidgetP
//		{
//		public:
//
//		private:
//			class WidgetInfo
//			{
//			public:
//				WidgetInfo (OwnPtr<WidgetP>);
//
//				OwnPtr<WidgetP> widget;
//				bool active;
//				bool visible;
//			};
//
//			void addWidget (OwnPtr<WidgetP> widget);
//
//			std::function<void (Event const &)> handleContainerEventFunction;
//			std::function<void ()> updateWidgetBoundsFunction;
//			Recti bounds;
//			std::list<WidgetInfo> widgetInfos;
//			std::map<Ptr<Widget>, std::list<WidgetInfo>::iterator> widgetLookup;
//		};
//	}
//}
//
