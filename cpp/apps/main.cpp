#include <windows.h>

#include <mfplay_lib.h>

struct mfplay_runtime_initializer {
    mfplay_runtime_initializer()
    {
        mfplay_initialize();
    }
    ~mfplay_runtime_initializer()
    {
        mfplay_finalize();
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    mfplay_runtime_initializer initializer;

    return 0;
}