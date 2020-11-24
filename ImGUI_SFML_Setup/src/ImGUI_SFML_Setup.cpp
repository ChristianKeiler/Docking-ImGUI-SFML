// ImGUI_SFML_Setup.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../include/GUIProgram.h"

// IMGUI Includes
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"
#include "imgui_stdlib.h"

void header()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load")) GUIProgram::GUIProgram::openExplorer();
			if (ImGui::MenuItem("SaveAs")) GUIProgram::GUIProgram::saveExplorer();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void body()
{
	bool docked = true;
	ImGui::Begin("Test", &docked);
	if (ImGui::Button("Button", ImVec2(50, 20)))
	{
		std::cout << "Button pressed" << std::endl;
	}
	ImGui::End();
}

int main()
{
	GUIProgram::GUIProgram program = GUIProgram::GUIProgram::get();
	program.setProgramName("Test");

	//program.addCustomFont("fonts/arial.ttf", 16.0f);
	//program.addCustomFont("fonts/alfredo.ttf", 10.0f);

	// I suggest setting header and body using either of 2 styles
	// compact using a single lambda expression
	program.setHeader([]() { header(); });
	
	std::function<void()> customBody = []() { body(); };
	program.setBody(customBody);
	
	program.run();
	return 0;
}
