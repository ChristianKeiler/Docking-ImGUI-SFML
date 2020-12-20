// ImGUI_SFML_Setup.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma once
#include <iostream>
#include "../include/GUIProgram.h"
#include "../include/Utility.h"

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
	// GUIProgram is based on a Singleton implementation
	// There are 2 ways to use the class
	// Either get the classes instance through GUIProgram::get()
	// or use the static methods instead
	// nonstatic and static methods share method names.
	// nonstatic methods start with a lowercase letter, static methods start with an uppercase letter
	// Example:
	//	nonstatic:
	//		GUIProgram::GUIProgram* program = GUIProgram::GUIProgram::get();
	//		program->setProgramName("Example");
	//	static:
	//		GUIProgram::GUIProgram::SetProgramName("Example");

	GUIProgram::GUIProgram* program = GUIProgram::GUIProgram::Get();
	program->setProgramName("Test");

	// I suggest setting header and body using either of 2 styles
	// compact using a single lambda expression
	program->setHeader([]() { header(); });
	
	// or saving the lambda into an std::function<void> variable and passing it to the setter
	std::function<void()> customBody = []() { body(); };
	program->setBody(customBody);
	
	//program->run();
	GUIProgram::GUIProgram::Run();
	return 0;
}
