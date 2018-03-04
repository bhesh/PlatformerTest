/**
	@author Brian Hession

	Handles the SFML window for PLT
*/

#include "stdafx.h"

#include "plt/world/PltWindow.h"

#include <iostream>

namespace plt {
	namespace world {

		// Public

		PltWindow::PltWindow(sf::VideoMode mode, const sf::String &title,
			sf::Uint32 style, const sf::ContextSettings &settings) :
			sf::RenderWindow(mode, title, style, settings) {
			onCreate();
		}

		PltWindow::PltWindow(sf::WindowHandle handle,
			const sf::ContextSettings& settings) :
			sf::RenderWindow(handle, settings) {
			onCreate();
		}

		void PltWindow::setFont(sf::Font *font) {
			m_font = font;
		}

		void PltWindow::showFps(bool showFps) {
			m_showFps = showFps;
		}

		int PltWindow::getFps() const {
			return m_lastFps;
		}

		void PltWindow::keepAspectRatio(bool keepRatio) {
			m_keepAspectRatio = keepRatio;
		}

		void PltWindow::putView(const sf::String &key, const sf::View &view) {
			m_views[key] = view;
		}

		const sf::View& PltWindow::getView(const sf::String &key) const {
			if (m_views.find(key) == m_views.end())
				throw std::logic_error("Key not found");
			return m_views.at(key);
		}

		void PltWindow::removeView(const sf::String &key) {
			if (m_views.find(key) == m_views.end())
				throw std::logic_error("Key not found");
			m_views.erase(key);
		}

		void PltWindow::setView(const sf::String &key) {
			if (m_views.find(key) == m_views.end())
				throw std::logic_error("Key not found");
			setView(m_views.at(key));
		}

		void PltWindow::setView(const sf::View &view) {
			sf::RenderWindow::setView(view);
		}

		void PltWindow::resetView() {
			setView(m_view);
		}

		const sf::View& PltWindow::getCurrentView() const {
			return sf::RenderWindow::getView();
		}

		void PltWindow::display() {
			if (m_showFps && m_font != NULL)
				drawFps();
			sf::RenderWindow::display();
			updateFps();
		}

		// Protected

		void PltWindow::onCreate() {
			sf::RenderWindow::onCreate();
			m_videoSize = getSize();
			sf::Vector2f vSize(static_cast<float>(m_videoSize.x),
				static_cast<float>(m_videoSize.y));
			m_view = sf::View(sf::FloatRect(0, 0, vSize.x, vSize.y));
			setView(m_view);
			m_lastFps = 0;
			m_curFps = 0;
			updateFps();
		}

		void PltWindow::onResize() {
			sf::RenderWindow::onResize();
			sf::Vector2u nSize = getSize();
			if (m_keepAspectRatio)
				calculateViewSize(nSize);
			setView(m_view);
		}

		// Private

		void PltWindow::calculateViewSize(sf::Vector2u nSize) {
			unsigned int a = m_videoSize.x * nSize.y;
			unsigned int b = m_videoSize.y * nSize.x;
			if (a > b) {
				float p = static_cast<float>(b) / static_cast<float>(a);
				m_view.setViewport(sf::FloatRect(0, (1.f - p) / 2.f, 1.f, p));
			}
			else if (b > a) {
				float p = static_cast<float>(a) / static_cast<float>(b);
				m_view.setViewport(sf::FloatRect((1.f - p) / 2.f, 0, p, 1.f));
			}
		}

		void PltWindow::updateFps() {
			sf::Time dt = m_clock.restart();
			m_period += dt;
			if (m_period.asMilliseconds() >= 1000) {
				m_lastFps = m_curFps;
				m_curFps = 0;
				m_period = sf::Time::Zero;
			}
			++m_curFps;
		}

		void PltWindow::drawFps() {
			char fpsBuf[255];
			snprintf(fpsBuf, sizeof(fpsBuf), "FPS: %d", m_lastFps);
			sf::Text fpsMsg;
			fpsMsg.setFont(*m_font);
			fpsMsg.setString(fpsBuf);
			fpsMsg.setCharacterSize(24);
			fpsMsg.setFillColor(sf::Color::Red);
			fpsMsg.setPosition(sf::Vector2f(4, -12.f));
			resetView();
			draw(fpsMsg);
		}
	}
}