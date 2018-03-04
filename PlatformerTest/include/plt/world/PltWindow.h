/**
	@author Brian Hession

	Handles the SFML window for PLT
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#pragma once
#endif

#ifndef PLT_WINDOW_PLTWINDOW_H
#define PLT_WINDOW_PLTWINDOW_H

#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/String.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <map>

namespace plt {
	namespace world {
	
		class PltWindow : public sf::RenderWindow {
		public:
			PltWindow(sf::VideoMode mode, const sf::String &title,
				sf::Uint32 style = sf::Style::Default,
				const sf::ContextSettings &settings = sf::ContextSettings());
			PltWindow(sf::WindowHandle handle,
				const sf::ContextSettings& settings = sf::ContextSettings());
			void setFont(sf::Font *font);
			void showFps(bool showFps);
			int getFps() const;
			void keepAspectRatio(bool keepRatio);
			void putView(const sf::String &key, const sf::View &view);
			const sf::View& getView(const sf::String &key) const;
			void removeView(const sf::String &key);
			void setView(const sf::String &key);
			void setView(const sf::View &view);
			void resetView();
			const sf::View& getCurrentView() const;
			void display();
		protected:
			virtual void onCreate();
			virtual void onResize();
		private:
			void calculateViewSize(sf::Vector2u nSize);
			sf::Vector2u m_videoSize;
			sf::View m_view;
			std::map<sf::String, sf::View> m_views;
			bool m_keepAspectRatio;

			// Frame rate viewer
			void updateFps();
			void drawFps();
			bool m_showFps;
			sf::Clock m_clock;
			sf::Time m_period;
			int m_lastFps;
			int m_curFps;
			sf::Font *m_font;
		};
	}
}

#endif // PLT_WINDOW_PLTWINDOW_H