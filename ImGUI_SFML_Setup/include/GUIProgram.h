#pragma once
// SFML Includes
#include <SFML/Graphics.hpp>

// IMGUI Includes
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

// Windows Includes
#include <Windows.h>
#include <iostream>
#include <functional>
#include <tchar.h>
#include "Utility.h"

namespace KeilerDev
{
	namespace GUIProgram
	{
		class GUIProgram
		{
		public:
			static GUIProgram* Get();

			///<summary>Starts the GUIProgram</summary>
			void run();

			///<summary>Starts the GUIProgram</summary>
			static void Run() { GUIProgram::Get()->run(); }

			///<summary>Ends the GUIProgram</summary>
			void quit() { m_exitCalled = true; }
			///<summary>Ends the GUIProgram</summary>
			static void Quit() { GUIProgram::Get()->m_exitCalled = true; }

			///<summary>Sets the name of the generated window</summary>
			///<param name='programName'>The name that should be displayed on the top of the Window</param>
			void setProgramName(const std::string programName) { m_programName = programName; }
			///<summary>Sets the name of the generated window</summary>
			///<param name='programName'>The name that should be displayed on the top of the Window</param>
			static void SetProgramName(const std::string programName) { GUIProgram::Get()->m_programName = programName; }

			///<summary>Sets the path from where to load the programs icon from. No icon is used if this (or the static version of this) function is never called.</summary>
			///<param name='iconPath'>The path from where to load the icon</param>
			void setProgramIcon(const std::string iconPath) { m_iconPath = iconPath; }
			///<summary>Sets the path from where to load the programs icon from. No icon is used if this (or the non-static version of this) function is never called.</summary>
			///<param name='iconPath'>The path from where to load the icon</param>
			static void SetProgramIcon(const std::string iconPath) { GUIProgram::Get()->m_iconPath = iconPath; }

			///<summary>Sets which kind of window you want to use (Fullscreen, windowed, etc). Use the sf::Style flags to construct the window style.
			/// Default is windowed with a titlebar, a resize and a close button</summary>
			///<param name='windowStyle'>The integer resembling the desired style</param>
			void setProgramWindowStyle(const int windowStyle) { m_windowStyle = windowStyle; }
			///<summary>Sets which kind of window you want to use (Fullscreen, windowed, etc). Use the sf::Style flags to construct the window style.
			/// Default is windowed with a titlebar, a resize and a close button</summary>
			///<param name='windowStyle'>The integer resembling the desired style</param>
			static void SetProgramWindowStyle(const int windowStyle) { GUIProgram::Get()->m_windowStyle = windowStyle; }

			///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
			///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
			void setHeader(const std::function<void()>& headerFunction) { m_header = headerFunction; }
			///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
			///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
			static void SetHeader(const std::function<void()>& headerFunction) { GUIProgram::Get()->m_header = headerFunction; }

			///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
			///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
			void setBody(const std::function<void()>& bodyFunction) { m_body = bodyFunction; }
			///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
			///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
			static void SetBody(const std::function<void()>& bodyFunction) { GUIProgram::Get()->m_body = bodyFunction; }

			///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
			///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method.</summary>
			///<param name='fontPath'>The path your font is located at</param>
			///<param name='fontSize'>The size your font should be (in pixels)</param>
			void addCustomFont(const std::string fontPath, const float fontSize) { m_fontList.insert(std::make_pair(fontPath, fontSize)); }
			///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
			///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method.</summary>
			///<param name='fontPath'>The path your font is located at</param>
			///<param name='fontSize'>The size your font should be (in pixels)</param>
			static void AddCustomFont(const std::string fontPath, const float fontSize) { GUIProgram::Get()->m_fontList.insert(std::make_pair(fontPath, fontSize)); }

			///<summary>Changes the font for any ImGUI code executed after calling this method. Make sure to use PopFont() if you previously called this method</summary>
			///<param name='fontName'>The name of the font you want to use (excluding the file ending. E.g. If you added "arial.ttf" the fontName would be "arial"</param>
			void pushFont(const std::string fontName);
			///<summary>Changes the font for any ImGUI code executed after calling this method. Make sure to use PopFont() if you previously called this method</summary>
			///<param name='fontName'>The name of the font you want to use (excluding the file ending. E.g. If you added "arial.ttf" the fontName would be "arial"</param>
			static void PushFont(const std::string fontName) { GUIProgram::Get()->pushFont(fontName); }

			///<summary>Ends the current font change</summary>
			void popFont();
			///<summary>Ends the current font change</summary>
			static void PopFont() { GUIProgram::Get()->popFont(); }

			///<summary>Changes the style of an active window at runtime</summary>
			//<param name='windowStyle'>The integer resembling the desired style</param>
			void changeProgramWindowStyle(const int windowStyle);
			///<summary>Changes the style of an active window at runtime</summary>
			//<param name='windowStyle'>The integer resembling the desired style</param>
			static void ChangeProgramWindowStyle(const int windowStyle) { GUIProgram::Get()->changeProgramWindowStyle(windowStyle); }

			// private methods
		private:
			GUIProgram() = default;

			void writeFontWarning(std::string fontPath, std::string fontName);
			void setIcon();
			void adjustResolutionOnFullscreen();

			std::string extractFileNameFromPath(const std::string path, const char seperator = '/');

			// private variables
		private:
			sf::RenderWindow m_window;
			int m_windowStyle = sf::Style::Default;

			int m_width = 640;
			int m_height = 480;

			std::string m_programName;
			std::string m_currentFileName;

			std::string m_iconPath;

			std::map<std::string, float> m_fontList = std::map<std::string, float>();
			std::map<std::string, bool> m_fontWarnings = std::map<std::string, bool>();
			std::map<std::string, ImFont*> m_fontSelector = std::map<std::string, ImFont*>();

			std::function<void()> m_header = []() {};
			std::function<void()> m_body = []() {};

			bool m_programStarted = false;
			bool m_exitCalled = false;
		};
	}
}

