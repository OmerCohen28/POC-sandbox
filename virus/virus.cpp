#include <iostream>
#include <Windows.h>

int main()
{
	Sleep(2000);
    HANDLE file = CreateFileA(
		"..\\test.txt",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		4,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (file == INVALID_HANDLE_VALUE) {
		std::cout << "nope\n";
	}
	char* buf = new char[100] {"yohohohoho"};
	std::cout << WriteFile(file, buf, 100, NULL, NULL) << '\n';
	std::cout << true;


	//LoadLibraryA("D:\\c++ programs\\POC test\\inlineHook\\Debug\\inlineHook.dll");
	/*file = CreateFileA(
		"..\\test.txt",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		4,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (file == INVALID_HANDLE_VALUE) {
		std::cout << "nope\n";
	}
	std::cout << WriteFile(file, buf, 20, NULL, NULL) << '\n';
	std::cout << true;*/
}

