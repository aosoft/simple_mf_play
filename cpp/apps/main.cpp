#include <windows.h>

#include <mfplay_lib.h>
#include "main_window.h"

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
    auto win = std::make_shared<main_window>();
    if (win->Create(nullptr, ATL::CWindow::rcDefault, "", WS_OVERLAPPEDWINDOW) == nullptr) {
        return 0;
    }
    win->ShowWindow(SW_SHOW);

    while (true) {
        MSG msg;
        if (!::GetMessageW(&msg, nullptr, 0, 0)) {
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}