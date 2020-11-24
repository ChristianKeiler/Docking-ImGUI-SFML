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
		static GUIProgram& get();
		void run();

		/// <summary>Opens the default windows explorer window allowing the user to select a file to load.</summary>
		/// <returns>The full path of the selected file</returns>
		static char* openExplorer();
		///<summary>Opens the default windows explorer window allowing the user to select a save name and location.
		///This method is only meant to let the user determine a save location and file name. File creation (including concatenating the file extension if unspecified)
		///is NOT handled by this script!</summary>
		///<returns>The full path including name of the file the user specified</returns>
		static char* saveExplorer();

		// public Setters
		void setProgramName(const std::string programName) { m_programName = programName; }
		///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
		///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
		void setHeader(const std::function<void()> headerFunction) { m_header = headerFunction; }
		///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
		///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
		void setBody(const std::function<void()> bodyFunction) { m_body = bodyFunction; }

		///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
		///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method</summary>
		///<param name='fontPath'>The path your font is located at</param>
		///<param name='fontSize'>The size your font should be (in pixels)</param>
		void addCustomFont(const std::string fontPath, const float fontSize) { m_fontList.insert(std::make_pair(fontPath, fontSize)); }

		///<summary>Changes the font for any ImGUI code executed after calling this method. Make sure to use PopFont() if you previously called this method</summary>
		///<param name='fontName'>The name of the font you want to use (excluding the file ending. E.g. If you added "arial.ttf" the fontName would be "arial"</param>
		void PushFont(const std::string fontName);
		///<summary>Ends the current font change</summary>
		void PopFont();

	// private methods
	private:
		GUIProgram() = default;

		std::string extractFileNameFromPath(std::string path, char seperator = '/');
		std::wstring string2wstring(const std::string& s);

	// private variables
	private:
		float m_width = 640.0f;
		float m_height = 480.0f;

		std::string m_programName;
		std::string m_currentFileName;

		std::map<std::string, float> m_fontList = std::map<std::string, float>();
		std::map<std::string, ImFont*> m_fontSelector = std::map<std::string, ImFont*>();

		std::function<void()> m_header = []() {};
		std::function<void()> m_body = []() {};
	};
}

