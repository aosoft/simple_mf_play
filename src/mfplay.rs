use winit::platform::windows::HWND;
use windows::core::HSTRING;

mod mfplay_sys;

type HResult = mfplay_sys::hresult_t;

pub struct Mfplay {
    core: *mut mfplay_sys::mfplay
}

impl Mfplay {
    pub fn new(url: &str, hwnd_video: HWND) -> Result<Mfplay, HResult> {
        unsafe {
            let mut core: *mut mfplay_sys::mfplay = std::ptr::null_mut();
            let hr = mfplay_sys::create_mfplay(HSTRING::from(url).as_ptr(), std::mem::transmute(hwnd_video), &mut core);
            if hr < 0 {
                Err(hr)
            } else {
                Ok(Mfplay{ core })
            }
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

pub struct MfRuntimeInitializer();

impl MfRuntimeInitializer {
    pub fn new() -> MfRuntimeInitializer {
        unsafe {
            mfplay_sys::mfplay_initialize();
            MfRuntimeInitializer()
        }
    }
}

impl Drop for MfRuntimeInitializer {
    fn drop(&mut self) {
        unsafe {
            mfplay_sys::mfplay_finalize();
        }
    }
}