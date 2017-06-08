#include "copyFunctions.hpp"

int fileCopyFull(char* inFileName, char* outFileName) {

	HANDLE hFile_1 = ::CreateFile(inFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile_2 = ::CreateFile(outFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = ::GetFileSize(hFile_1, NULL);

	if (hFile_1 == INVALID_HANDLE_VALUE) {
		int erCode = GetLastError();
		std::cerr << "Error opening file, code " << erCode << "\n";
		return erCode;
	}
	if (hFile_2 == INVALID_HANDLE_VALUE) {
		int erCode = GetLastError();
		std::cerr << "Error creating file, code " << erCode << "\n";
		return erCode;
	}

	HANDLE hMap_1 = ::CreateFileMapping(hFile_1, NULL, PAGE_READONLY, 0, 0, NULL);
	HANDLE hMap_2 = ::CreateFileMapping(hFile_2, NULL, PAGE_READWRITE, 0, fileSize, NULL);

	if (hMap_1 == NULL) {
		int erCode = GetLastError();
		std::cerr << "Error creating file mapping #1, code " << erCode << "\n";
		::CloseHandle(hFile_1);
		::CloseHandle(hFile_2);
		return erCode;
	}
	if (hMap_2 == NULL) {
		int erCode = GetLastError();
		std::cerr << "Error creating file mapping #2, code " << erCode << "\n";
		::CloseHandle(hFile_1);
		::CloseHandle(hFile_2);
		return erCode;
	}

	PVOID pView_1 = ::MapViewOfFile(hMap_1, FILE_MAP_READ, 0, 0, 0);
	PVOID pView_2 = ::MapViewOfFile(hMap_2, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

	const char *data_1 = static_cast<const char*>(pView_1);
	char *data_2 = static_cast<char*>(pView_2);

	std::memcpy(data_2, data_1, fileSize); // copy

	::UnmapViewOfFile(pView_1);
	::UnmapViewOfFile(pView_2);
	::CloseHandle(hMap_1);
	::CloseHandle(hFile_1);
	::CloseHandle(hMap_2);
	::CloseHandle(hFile_2);

	return 0;
}