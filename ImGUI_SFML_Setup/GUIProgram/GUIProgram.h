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

namespace KeilerDev
{
	namespace GUIProgram
	{
		class GUIProgram
		{
		public:
			static GUIProgram* Get()
			{
				static GUIProgram program;

				return &program;
			}

			///<summary>Starts the GUIProgram</summary>
			void run()
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
			///<summary>Starts the GUIProgram</summary>
			static void Run() { GUIProgram::Get()->run(); }

			///<summary>Ends the GUIProgram</summary>
			void quit() { m_exitCalled = true; }
			///<summary>Ends the GUIProgram</summary>
			static void Quit() { GUIProgram::Get()->quit(); }

			///<summary>Sets the name of the generated window</summary>
			///<param name='programName'>The name that should be displayed on the top of the Window</param>
			void setProgramName(const std::string programName) { m_programName = programName; }
			///<summary>Sets the name of the generated window</summary>
			///<param name='programName'>The name that should be displayed on the top of the Window</param>
			static void SetProgramName(const std::string programName) { GUIProgram::Get()->setProgramName(programName); }

			///<summary>Sets the path from where to load the programs icon from. No icon is used if this (or the static version of this) function is never called.</summary>
			///<param name='iconPath'>The path from where to load the icon</param>
			void setProgramIcon(const std::string iconPath) { m_iconPath = iconPath; }
			///<summary>Sets the path from where to load the programs icon from. No icon is used if this (or the non-static version of this) function is never called.</summary>
			///<param name='iconPath'>The path from where to load the icon</param>
			static void SetProgramIcon(const std::string iconPath) { GUIProgram::Get()->setProgramIcon(iconPath); }

			///<summary>Sets which kind of window you want to use (Fullscreen, windowed, etc). Use the sf::Style flags to construct the window style.
			/// Default is windowed with a titlebar, a resize and a close button</summary>
			///<param name='windowStyle'>The integer resembling the desired style</param>
			void setProgramWindowStyle(const int windowStyle) { m_windowStyle = windowStyle; }
			///<summary>Sets which kind of window you want to use (Fullscreen, windowed, etc). Use the sf::Style flags to construct the window style.
			/// Default is windowed with a titlebar, a resize and a close button</summary>
			///<param name='windowStyle'>The integer resembling the desired style</param>
			static void SetProgramWindowStyle(const int windowStyle) { GUIProgram::Get()->setProgramWindowStyle(windowStyle); }

			///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
			///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
			void setHeader(const std::function<void()>& headerFunction) { m_header = headerFunction; }
			///<summary>Sets the method the GUIProgram should use to build the headerbar</summary>
			///<param name='headerFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own header</param>
			static void SetHeader(const std::function<void()>& headerFunction) { GUIProgram::Get()->setHeader(headerFunction); }

			///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
			///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
			void setBody(const std::function<void()>& bodyFunction) { m_body = bodyFunction; }
			///<summary>Sets the method the GUIProgram should use to build the entire content</summary>
			///<param name='bodyFunction'>Use lambdas or wrap your header method into an std::function&lt;void()&gt; to define your own body/content</param>
			static void SetBody(const std::function<void()>& bodyFunction) { GUIProgram::Get()->setBody(bodyFunction); }

			///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
			///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method.</summary>
			///<param name='fontPath'>The path your font is located at</param>
			///<param name='fontSize'>The size your font should be (in pixels)</param>
			void addCustomFont(const std::string fontPath, const float fontSize) { m_fontList.insert(std::make_pair(fontPath, fontSize)); }
			///<summary>Adds a custom font to the GUIProgram. The first font will be used as default font for the program.
			///To change fonts (e.g. if you want a different font for headerbars and content elements) use the PushFont method.</summary>
			///<param name='fontPath'>The path your font is located at</param>
			///<param name='fontSize'>The size your font should be (in pixels)</param>
			static void AddCustomFont(const std::string fontPath, const float fontSize) { GUIProgram::Get()->addCustomFont(fontPath, fontSize); }

			///<summary>Changes the font for any ImGUI code executed after calling this method. Make sure to use PopFont() if you previously called this method</summary>
			///<param name='fontName'>The name of the font you want to use (excluding the file ending. E.g. If you added "arial.ttf" the fontName would be "arial"</param>
			void pushFont(const std::string fontName)
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

					std::cout << "[Font Error] " << fontName << "does not exist in the font atlas." << std::endl;
					return;
				}
				ImGui::PushFont(m_fontSelector[fontName]);
			}
			///<summary>Changes the font for any ImGUI code executed after calling this method. Make sure to use PopFont() if you previously called this method</summary>
			///<param name='fontName'>The name of the font you want to use (excluding the file ending. E.g. If you added "arial.ttf" the fontName would be "arial"</param>
			static void PushFont(const std::string fontName) { GUIProgram::Get()->pushFont(fontName); }

			///<summary>Ends the current font change</summary>
			void popFont()
			{
				ImGui::PopFont();
			}
			///<summary>Ends the current font change</summary>
			static void PopFont() { GUIProgram::Get()->popFont(); }

			///<summary>Changes the style of an active window at runtime</summary>
			///<param name='windowStyle'>The integer resembling the desired style</param>
			void changeProgramWindowStyle(const int windowStyle)
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
			///<summary>Changes the style of an active window at runtime</summary>
			///<param name='windowStyle'>The integer resembling the desired style</param>
			static void ChangeProgramWindowStyle(const int windowStyle) { GUIProgram::Get()->changeProgramWindowStyle(windowStyle); }

			// private methods
		private:
			GUIProgram() = default;

			void writeFontWarning(const std::string fontPath, const std::string fontName)
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
			void setIcon()
			{
				if (m_iconPath.empty()) return;

				sf::Image icon;
				icon.loadFromFile(m_iconPath);
				sf::Vector2u iconSize = icon.getSize();
				m_window.setIcon(iconSize.x, iconSize.y, icon.getPixelsPtr());

			}
			void adjustResolutionOnFullscreen()
			{
				m_width = sf::VideoMode::getDesktopMode().width;
				m_height = sf::VideoMode::getDesktopMode().height;
			}

			std::string extractFileNameFromPath(const std::string path, const char seperator = '/')
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

