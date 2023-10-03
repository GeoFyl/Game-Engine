#pragma once
#include "pch.h"
#ifdef _WIN32||_WIN64
#include <Windows.h>
#endif

#include "Grengine.h"

int main(int argc, char* argv[]) {
    std::unique_ptr<Grengine::Core> engine(new Grengine::Core(argc, argv));

    return engine->Run();
}

//We're probably on Windows!
//Let's just handle everything through Main, just in case
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    return main(__argc, __argv);
}