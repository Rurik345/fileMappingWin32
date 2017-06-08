#include "copyFunctions.hpp"

int main() {

	while (true) {
		int choice, result;
		std::cout << "Choose the type of file mapping (1 - in one take, 2 - in chunks, 3 - exit): ";
		std::cin >> choice;

		switch (choice) {
		case 1:
			result = fileCopyFull("fileToCopy.txt", "copy.txt");
			if (result != 0) {
				return result;
			}
			break;

		case 2:
			result = fileCopyPart("fileToCopy.txt", "copy.txt");
			if (result != 0) {
				return result;
			}
			break;

		case 3:
			return 0;
		}
	}

	return 0;
}
