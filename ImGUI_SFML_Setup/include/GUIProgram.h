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

namespace GUIProgram
{
	class GUIProgram
	{
	// public methods
	public:
		static GUIProgram* Get();

		///<summary>Starts the GUIProgram</summary>
		void run();

		///<summary>Starts the GUIProgram</summary>
		static void Run() { GUIProgram::Get()->run(); }

		///<summary>Ends the GUIProgram</summary>
		void quit() { exit(); }
		///<summary>Ends the GUIProgram</summary>
		static void Quit() { GUIProgram::Get()->exit(); }

		// public Setters
		///<summary>Sets the name of the generated window</summary>
		///<param name='programName'>The name that should be displayed on the top of the Window</param>
		void setProgramName(const std::string programName) { m_programName = programName; }
		///<summary>Sets the name of the generated window</summary>
		///<param name='programName'>The name that should be displayed on the top of the Window</param>
		static void SetProgramName(const std::string programName) { GUIProgram::Get()->m_programName = programName; }

		///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
		///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
		void setHeader(const std::function<void()> headerFunction) { m_header = headerFunction; }
		///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
		///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
		static void SetHeader(const std::function<void()> headerFunction) { GUIProgram::Get()->m_header = headerFunction; }

		///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
		///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
		void setBody(const std::function<void()> bodyFunction) { m_body = bodyFunction; }
		///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
		///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
		static void SetBody(const std::function<void()> bodyFunction) { GUIProgram::Get()->m_body = bodyFunction; }

		///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
		///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method.
		///Notice: Functionality of multi-font usage is currently untested. It should work, if it doesn't (and if you absolutely need it right away, 
		///please implement it yourself) let me know so I can fix it for future releases</summary>
		///<param name='fontPath'>The path your font is located at</param>
		///<param name='fontSize'>The size your font should be (in pixels)</param>
		void addCustomFont(const std::string fontPath, const float fontSize) { m_fontList.insert(std::make_pair(fontPath, fontSize)); }
		///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
		///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method.
		///Notice: Functionality of multi-font usage is currently untested. It should work, if it doesn't (and if you absolutely need it right away, 
		///please implement it yourself) let me know so I can fix it for future releases</summary>
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

	// private methods
	private:
		GUIProgram() = default;
		void exit() { m_exitCalled = true; }

		std::string extractFileNameFromPath(std::string path, char seperator = '/');
		std::wstring string2wstring(const std::string& s);

	// private variables
	private:
		int m_width = 640;
		int m_height = 480;

		std::string m_programName;
		std::string m_currentFileName;

		std::map<std::string, float> m_fontList = std::map<std::string, float>();
		std::map<std::string, ImFont*> m_fontSelector = std::map<std::string, ImFont*>();

		std::function<void()> m_header = []() {};
		std::function<void()> m_body = []() {};

		bool m_exitCalled = false;
	};
}

