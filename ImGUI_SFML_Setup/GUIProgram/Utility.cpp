#pragma once
#include "Utility.h"

namespace KeilerDev
{
	namespace GUIProgram
	{
		char* Utility::OpenExplorer()
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

		char* Utility::SaveExplorer()
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
	}
}