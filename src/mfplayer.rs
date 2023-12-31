use winit::platform::windows::HWND;
use windows::core::HSTRING;

mod mfplayer_sys;

type HResult = mfplayer_sys::hresult_t;

pub struct Mfplay {
    core: *mut mfplayer_sys::mfplayer
}

impl Mfplay {
    pub fn new(url: &str, hwnd_video: HWND) -> Result<Mfplay, HResult> {
        unsafe {
            let mut core: *mut mfplayer_sys::mfplayer = std::ptr::null_mut();
            let hr = mfplayer_sys::create_mfplayer(HSTRING::from(url).as_ptr(), std::mem::transmute(hwnd_video), &mut core);
            if hr < 0 {
                Err(hr)
            } else {
                Ok(Mfplay{ core })
            }
        }
    }

    pub fn resize(&self, width: i32, height: i32) -> Result<(), HResult> {
        unsafe {
            ((*(self.core)).vtable_.as_ref().unwrap().mfplayer_resize_video)(self.core, width, height);
            Ok(())
        }
    }
}

impl Drop for Mfplay {
    fn drop(&mut self) {
        unsafe {
            ((*(self.core)).vtable_.as_ref().unwrap().mfplayer_dispose)(self.core);
        }
    }
}

pub struct MfRuntimeInitializer();

impl MfRuntimeInitializer {
    pub fn new() -> MfRuntimeInitializer {
        unsafe {
            mfplayer_sys::mfplayer_initialize();
            MfRuntimeInitializer()
        }
    }
}

impl Drop for MfRuntimeInitializer {
    fn drop(&mut self) {
        unsafe {
            mfplayer_sys::mfplayer_finalize();
        }
    }
}