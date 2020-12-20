#pragma once
// Windows Includes
#include <Windows.h>
#include <iostream>
#include <functional>
#include <tchar.h>

namespace GUIProgram
{
	class Utility
	{
	public:
		/// <summary>Opens the default windows explorer window allowing the user to select a file to load.
		/// Notice: This method only returns the path (as char*) of the selected file. Any work that needs to be done,
		/// like opening and reading the file or deserialization, needs to be done seperately.</summary>
		/// <returns>The full path of the selected file</returns>
		static char* OpenExplorer();
		///<summary>Opens the default windows explorer window allowing the user to select a save name and location.
		///This method is only meant to let the user determine a save location and file name. File creation (including concatenating the file extension if unspecified)
		///is NOT handled by this script!</summary>
		///<returns>The full path including name of the file the user specified</returns>
		static char* SaveExplorer();
	};
}

