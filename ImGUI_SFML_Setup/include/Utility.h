#pragma once
// Windows Includes
#include <Windows.h>
#include <iostream>
#include <functional>
#include <tchar.h>

namespace KeilerDev
{
	namespace GUIProgram
	{
		class Utility
		{
		public:
			/// <summary>Opens the default windows explorer window allowing the user to select a file to load.
			/// Notice: This method only returns the path (as char*) of the selected file. Any work that needs to be done,
			/// like opening and reading the file or deserialization, needs to be done seperately.</summary>
			/// <returns>The full path of the selected file</returns>
			static char* OpenExplorer()
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
			///<summary>Opens the default windows explorer window allowing the user to select a save name and location.
			///This method is only meant to let the user determine a save location and file name. File creation (including concatenating the file extension if unspecified)
			///is NOT handled by this script!</summary>
			///<returns>The full path including name of the file the user specified</returns>
			static char* SaveExplorer()
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
		};
	}
}

