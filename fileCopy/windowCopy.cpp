#include "copyFunctions.hpp"

int fileCopyPart(char* inFileName, char* outFileName) {

	SYSTEM_INFO sysinfo;
	::GetSystemInfo(&sysinfo);
	DWORD view = sysinfo.dwAllocationGranularity; // 65536 bytes = 64KB

	HANDLE hFile_1 = ::CreateFile(inFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hFile_2 = ::CreateFile(outFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(hFile_1, NULL);

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

	for (unsigned long long offset = 0; offset < fileSize; offset += view) {

		if (offset + view > fileSize) { // if over 64KB block
			view = static_cast<int>(fileSize - offset);
		}

		PVOID pView_1 = ::MapViewOfFile(hMap_1, FILE_MAP_READ, 0, offset, view);
		PVOID pView_2 = ::MapViewOfFile(hMap_2, FILE_MAP_READ | FILE_MAP_WRITE, 0, offset, view);
		const char *data_1 = static_cast<const char*>(pView_1);
		char *data_2 = static_cast<char*>(pView_2);

		if (data_1 != NULL) {
			std::memcpy(data_2, data_1, view); // copy
		}
		::UnmapViewOfFile(pView_1);
		::UnmapViewOfFile(pView_2);
	}

	::CloseHandle(hMap_1);
	::CloseHandle(hMap_2);
	::CloseHandle(hFile_1);
	::CloseHandle(hFile_2);

	return 0;
}
