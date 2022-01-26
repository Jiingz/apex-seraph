#include <windows.h>

#include <loader/app/application.h>


#ifdef main
    #undef main
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    loader::Application application;
    return application.Run();
}
