#pragma once

#define WIN32_LEAN_AND_MEAN
#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <atlbase.h>
extern ATL::CAtlWinModule _Module;
#include <atlwin.h>

#include <memory>

class main_window : public ATL::CWindowImpl<main_window>,
                    public std::enable_shared_from_this<main_window> {
public:
    DECLARE_WND_CLASS("simple_mf_play");
    std::shared_ptr<main_window> _self;

private:
    BEGIN_MSG_MAP(main_window)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

    void OnFinalMessage(HWND) override;

    LRESULT OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    main_window();
    ~main_window();
};