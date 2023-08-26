#include <windows.h>

#include <mfplayer.h>
#include "main_window.h"

struct mfplay_runtime_initializer {
    mfplay_runtime_initializer()
    {
        mfplayer_initialize();
    }
    ~mfplay_runtime_initializer()
    {
        mfplayer_finalize();
    }
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto source_path = std::wstring(pCmdLine);
    if (source_path.empty()) {
        return 0;
    }
    mfplay_runtime_initializer initializer;
    auto win = std::make_shared<main_window>(std::move(source_path));
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
    win = nullptr;

    return 0;
}