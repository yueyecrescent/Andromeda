#ifndef ANDROMEDA_APP_TRANSPARENTFULLWINDOWAPPLICATION
#define ANDROMEDA_APP_TRANSPARENTFULLWINDOWAPPLICATION

#include "../graphics/color_rgba.h"
#include "window_application.h"

/**
 * 带有图形和音频的应用，可用于编写客户端
 * GLFW只能在主线程使用，因此主线程负责窗口事件及渲染
 */

namespace andromeda {
	namespace app {
		template<typename Derived>
		class TransparentFullWindowApplication:public andromeda::app::WindowApplication<Derived>
		{
			DefineApplication(Derived)
		public:
			TransparentFullWindowApplication(const char* window_title=nullptr,bool isFullScreen=false) :
					andromeda::app::WindowApplication<Derived>(window_title,andromeda::app::RenderSys::screenWidth/2,andromeda::app::RenderSys::screenHeight/2,{0.0f,0.0f,0.0f,0.0f},isFullScreen)
			{

			}

			~TransparentFullWindowApplication()=default;

			void preinitialize()
			{
				setWindowFramebufferTransparent(true);
			}

			using andromeda::app::WindowApplication<Derived>::operator Window*;
			using andromeda::app::WindowApplication<Derived>::setSynchronizeFPS;
			using andromeda::app::WindowApplication<Derived>::getRenderFPS;
			using andromeda::app::WindowApplication<Derived>::getRenderFPSCount; //获取当前所在帧
			using andromeda::app::WindowApplication<Derived>::setRenderFPSLimit;
			using andromeda::app::WindowApplication<Derived>::setUpdateRateLimit;
			using Application<Derived>::getUpdateRate;
			using Application<Derived>::getUpdateRateCount; //获取当前所在帧
			using andromeda::app::WindowApplication<Derived>::getWindowHeight;
			using andromeda::app::WindowApplication<Derived>::getWindowWidth;
			using andromeda::app::WindowApplication<Derived>::isFullScreen;
			using andromeda::app::WindowApplication<Derived>::setFullScreen;
			using andromeda::app::WindowApplication<Derived>::setBackColor;
			using andromeda::app::WindowApplication<Derived>::setWindowMouseEventPassthrough; //preinitialize()中调用。窗口是否鼠标事件穿透
			using andromeda::app::WindowApplication<Derived>::setWindowFramebufferTransparent;
			//preinitialize()中调用。设置窗口渲染的背景是否透明。注意即使内容透明，事件依旧不会穿透窗口透明部分，它们会被窗口捕获

		};
	}
}

#endif//ANDROMEDA_APP_TRANSPARENTFULLWINDOWAPPLICATION