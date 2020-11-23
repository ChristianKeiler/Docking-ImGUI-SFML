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

int main()
{
	GUIProgram::GUIProgram program = GUIProgram::GUIProgram::init("Test");
	program.setHeader([]() {header(); });
	program.run();
	return 0;
}
