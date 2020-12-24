// ImGUI_SFML_Setup.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>
#include "GUIProgram.h"
#include "Utility.h"

// IMGUI Includes
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

using namespace KeilerDev;

void header()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load")) GUIProgram::Utility::OpenExplorer();
			if (ImGui::MenuItem("Save As")) GUIProgram::Utility::SaveExplorer();
			if (ImGui::MenuItem("Quit")) GUIProgram::GUIProgram::Quit();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Fullscreen")) GUIProgram::GUIProgram::ChangeProgramWindowStyle(sf::Style::Fullscreen);
			if (ImGui::MenuItem("Default")) GUIProgram::GUIProgram::ChangeProgramWindowStyle(sf::Style::Default);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void body()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, sf::Color::Black);
	ImGui::Begin("Test");
	if (ImGui::Button("Button", ImVec2(50, 20)))
	{
		std::cout << "Button pressed" << std::endl;
	}
	ImGui::End();
	ImGui::PopStyleColor();
}

int main()
{
	GUIProgram::GUIProgram* program = GUIProgram::GUIProgram::Get();
	program->setProgramName("Test");

	program->setProgramIcon("fonts/icon.png");

	program->setProgramWindowStyle(sf::Style::Titlebar | sf::Style::Close);

	// I suggest setting header and body using either of 2 styles
	// compact using a single lambda expression
	program->setHeader([]() { header(); });
	
	// or saving the lambda into an std::function<void> variable and passing it to the setter
	std::function<void()> customBody = []() { body(); };
	program->setBody(customBody);
	
	program->run();
	return 0;
}
