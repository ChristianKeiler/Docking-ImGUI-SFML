#pragma once
#include "../include/GUIProgram.h"

namespace GUIProgram
{
	GUIProgram& GUIProgram::GUIProgram::get()
	{
		static GUIProgram program;

		return program;
	}

	void GUIProgram::run()
	{
		sf::RenderWindow window(sf::VideoMode(m_width, m_width), m_programName);

		//ShowWindow(window.getSystemHandle(), SW_MAXIMIZE);
		HWND windowHandle = ::FindWindow(NULL, string2wstring(m_programName).c_str());
		::SetWindowPos(windowHandle, 0, 0, 0, m_width, m_height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		bool dock = true;

		ImGui::SFML::Init(window, m_fontList.empty());

		if (!m_fontList.empty())
		{
			io.Fonts->Clear();
			for (auto f : m_fontList)
			{
				ImFont* atlas = io.Fonts->AddFontFromFileTTF(f.first.c_str(), f.second);
				m_fontSelector.insert(std::make_pair(extractFileNameFromPath(f.first), atlas));
				assert(atlas != NULL);
			}
		}

		ImGui::SFML::UpdateFontTexture();

		sf::Clock deltaClock;

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				ImGui::SFML::ProcessEvent(event);

				if (event.type == sf::Event::Closed)
				{
					window.close();
				}
			}

			ImGui::SFML::Update(window, deltaClock.restart());

			// additional imgui configuration
			ImGuiWindowFlags windowFlags = 0;

			ImGuiViewport* mainViewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(ImVec2(mainViewport->GetWorkPos().x + 650, mainViewport->GetWorkPos().y + 20), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

			#pragma region Docking

			static bool optFullScreenPersistant = true;
			bool optFullScreen = optFullScreenPersistant;
			static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags windowFlags2 = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (optFullScreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->GetWorkPos());
				ImGui::SetNextWindowSize(viewport->GetWorkSize());
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				windowFlags2 |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				windowFlags2 |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
			}

			if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
				windowFlags2 |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Dockspace", &dock, windowFlags2);
			ImGui::PopStyleVar();

			if (optFullScreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
			}
			else
			{
				std::cout << "Docking is disabled" << std::endl;
			}

			m_header();
			
			ImGui::End();

			#pragma endregion

			m_body();

			window.clear();
			ImGui::SFML::Render(window);
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
			window.display();
		}

		ImGui::SFML::Shutdown();
	}


	char* GUIProgram::openExplorer()
	{
		// ------------------------------------------------
		// file explorer code taken from https://stackoverflow.com/a/43105644
		wchar_t filename[MAX_PATH];

		OPENFILENAMEW ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = _T("All Files\0*.*\0\0");
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = _T("Select a file");
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		// ------------------------------------------------

		if (GetOpenFileName(&ofn))
		{
			char* fn = nullptr;

			std::wcout << "Loading: " << filename << std::endl;

			size_t origsize = wcslen(filename);
			size_t convertedChars = 0;

			char strConcat[] = "(char *)";
			size_t strConcatsize = (strlen(strConcat) + 1) * 2;
			const size_t newsize = origsize * 2;
			fn = new char[newsize + strConcatsize];
			wcstombs_s(&convertedChars, fn, newsize, filename, _TRUNCATE);

			return fn;
		}
		else
		{
			return nullptr;
		}
	}

	char* GUIProgram::saveExplorer()
	{
		// ------------------------------------------------
		// file explorer code taken from https://stackoverflow.com/a/43105644
		wchar_t filename[MAX_PATH];

		OPENFILENAMEW ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = _T("All Files\0*.*\0\0");
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = _T("Select a save location");
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		// ------------------------------------------------

		if (GetSaveFileName(&ofn)) {
			char* fn = nullptr;

			std::wcout << "Saving: " << filename << std::endl;

			size_t origsize = wcslen(filename);
			size_t convertedChars = 0;

			char strConcat[] = "(char *)";
			size_t strConcatsize = (strlen(strConcat) + 1) * 2;
			const size_t newsize = origsize * 2;
			fn = new char[newsize + strConcatsize];
			wcstombs_s(&convertedChars, fn, newsize, filename, _TRUNCATE);

			return fn;
		}
		else {
			return nullptr;
		}
	}

	std::string GUIProgram::extractFileNameFromPath(std::string path, char seperator)
	{
		std::size_t seperatorPosition = path.rfind(seperator);
		std::size_t filetypeDotPosition = path.rfind('.');
		if (seperatorPosition != std::string::npos)
		{
			return path.substr(seperatorPosition + 1, filetypeDotPosition - seperatorPosition - 1);
		}
		else
		{
			return path;
		}
	}


	void GUIProgram::PushFont(std::string fontName)
	{
		ImGui::PushFont(m_fontSelector[fontName]);
	}

	void GUIProgram::PopFont()
	{
		ImGui::PopFont();
	}

	// https://stackoverflow.com/a/27296
	std::wstring GUIProgram::string2wstring(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
}
