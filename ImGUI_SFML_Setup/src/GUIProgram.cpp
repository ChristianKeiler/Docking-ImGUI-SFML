#pragma once
#include "../include/GUIProgram.h"

namespace KeilerDev
{
	namespace GUIProgram
	{
		GUIProgram* GUIProgram::GUIProgram::Get()
		{
			static GUIProgram program;

			return &program;
		}

		void GUIProgram::run()
		{
			m_programStarted = true;

			m_window.create(sf::VideoMode(m_width, m_height), m_programName, m_windowStyle);

			setIcon();

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

			ImGui::SFML::Init(m_window, m_fontList.empty());

			if (!m_fontList.empty())
			{
				io.Fonts->Clear();
				for (auto f : m_fontList)
				{
					ImFont* atlas = io.Fonts->AddFontFromFileTTF(f.first.c_str(), f.second);
					assert(atlas != NULL);
					m_fontSelector.insert(std::make_pair(extractFileNameFromPath(f.first), atlas));
				}
			}

			ImGui::SFML::UpdateFontTexture();

			sf::Clock deltaClock;

			while (m_window.isOpen())
			{
				if (m_exitCalled)
				{
					m_window.close();
					break;
				}

				sf::Event event;
				while (m_window.pollEvent(event))
				{
					ImGui::SFML::ProcessEvent(event);

					if (event.type == sf::Event::Closed)
					{
						m_window.close();
					}
				}

				ImGui::SFML::Update(m_window, deltaClock.restart());

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

				m_window.clear();
				ImGui::SFML::Render(m_window);
				if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
				{
					ImGui::UpdatePlatformWindows();
					ImGui::RenderPlatformWindowsDefault();
				}
				m_window.display();
			}

			ImGui::SFML::Shutdown();
		}

		std::string GUIProgram::extractFileNameFromPath(const std::string path, const char seperator)
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


		void GUIProgram::pushFont(const std::string fontName)
		{
			if (m_fontSelector.find(fontName) == m_fontSelector.end())
			{
				bool substringExists = false;
				for (auto font : m_fontSelector)
				{
					if (fontName.find(font.first) != std::string::npos)
					{
						writeFontWarning(fontName, font.first);
						ImGui::PushFont(font.second);
						return;
					}
				}

				std::cout << "[Font Error] " << fontName << "does not exist in the font atlas."<< std::endl;
				return;
			}
			ImGui::PushFont(m_fontSelector[fontName]);
		}
		void GUIProgram::popFont()
		{
			ImGui::PopFont();
		}

		void GUIProgram::changeProgramWindowStyle(const int windowStyle)
		{
			if (m_programStarted)
			{
				if (windowStyle & sf::Style::Fullscreen)
				{
					adjustResolutionOnFullscreen();
				}
				m_window.create(sf::VideoMode(m_width, m_height), m_programName, windowStyle);
				setIcon();
				m_windowStyle = windowStyle;
			}
		}

		void GUIProgram::writeFontWarning(const std::string fontPath, const std::string fontName)
		{
			if (m_fontWarnings.empty())
			{
				for (auto name : m_fontSelector)
				{
					m_fontWarnings.insert(std::make_pair(name.first, false));
				}
			}
			
			if (!m_fontWarnings[fontName])
			{
				m_fontWarnings[fontName] = true;
				std::cout << "[Font Warning] \'" << fontPath << "\' found as \'" << fontName << "\'! Use \'" << fontName << "\' instead" << std::endl;
			}
		}

		void GUIProgram::setIcon()
		{
			if (m_iconPath.empty()) return;

			sf::Image icon;
			icon.loadFromFile(m_iconPath);
			sf::Vector2u iconSize = icon.getSize();
			m_window.setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());

		}

		void GUIProgram::adjustResolutionOnFullscreen()
		{
			m_width = sf::VideoMode::getDesktopMode().width;
			m_height = sf::VideoMode::getDesktopMode().height;
		}
	}
}
