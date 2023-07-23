use winit::platform::windows::HWND;
use windows::core::HSTRING;

mod mfplay_sys;

type HResult = mfplay_sys::hresult_t;

struct Mfplay {
    core: *mut mfplay_sys::mfplay
}

impl Mfplay {
    pub fn new(url: &str, hwnd_video: HWND) -> Result<Mfplay, HResult> {
        unsafe {
            let mut core: *mut mfplay_sys::mfplay = std::ptr::null_mut();
            let hr = mfplay_sys::create_mfplay(HSTRING::from(url).as_ptr(), std::mem::transmute(hwnd_video), &mut core);
            todo!();
        }
    }
}

impl Drop for Mfplay {
    fn drop(&mut self) {
        unsafe {
            ((*(self.core)).vtable_.as_ref().unwrap().mfplay_dispose)(self.core);
        }
    }
}