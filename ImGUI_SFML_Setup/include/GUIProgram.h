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
		static GUIProgram& init(std::string programName);
		void run();

		/// <summary>Opens the default windows explorer window allowing the user to select a file to load.</summary>
		/// <returns>The full path of the selected file</returns>
		static char* openExplorer();
		static char* saveExplorer();

	// public Setters
	public:
		void addCustomFont(const std::string fontPath)
		{
			m_fontList.push_back(fontPath);
		}
		void setFontSize(const float value) { m_fontSize = value; }
		void setHeader(const std::function<void()> headerFunction) { m_header = headerFunction; }
		void setBody(const std::function<void()> bodyFunction) { m_body = bodyFunction; }

	// private methods
	private:
		GUIProgram() = default;

	// private variables
	private:
		std::string m_programName;
		std::string m_currentFileName;

		std::vector<std::string> m_fontList = std::vector<std::string>();

		bool m_useCustomFonts = false;

		float m_fontSize = 14.0f;

		std::function<void()> m_header = []() {};
		std::function<void()> m_body = []() {};
	};
}

