# include "incls/_precompiled.incl"
# include "incls/_main.cpp.incl"

int vm_main(int argc, char* argv[]);

#ifdef _WINDOWS
int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int cmdShow) {
	// Save all parameters
	hInstance = hInst;
	hPrevInstance = hPrevInst;
	nCmdShow = cmdShow;
	os::set_args(__argc, __argv);
	return vm_main(__argc, __argv);
}
#else
#if defined(__LINUX__) || defined(WIN32)
int main(int argc, char* argv[]) {
	os::set_args(argc, argv);
	return vm_main(argc, argv);
}
#endif
#endif
