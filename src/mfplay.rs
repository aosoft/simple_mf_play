use winit::platform::windows::HWND;

mod mfplay_sys;

type HResult = mfplay_sys::hresult_t;

struct Mfplay {
    core: mfplay_sys::mfplay
}

impl Mfplay {
    pub fn new(url: &str, hwnd_video: HWND) -> Result<Mfplay, HResult> {
        todo!()
    }
}

impl Drop for Mfplay {
    fn drop(&mut self) {
        unsafe {
            (self.core.vtable_.as_ref().unwrap().mfplay_dispose)(&mut self.core);
        }
    }
}