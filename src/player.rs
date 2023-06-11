use std::ffi::{OsStr, OsString};
use std::ptr::{null, null_mut};
use windows::core::{ComInterface, IUnknown, HRESULT, HSTRING, PCWSTR, PWSTR};
use windows::imp::CreateEventW;
use windows::Win32::Foundation::{FALSE, HANDLE, HWND, S_OK};
use windows::Win32::Media::MediaFoundation::*;
use windows::Win32::UI::Shell::PropertiesSystem::IPropertyStore;

struct MFInitializer();

impl MFInitializer {
    pub fn new() -> Result<MFInitializer, windows::core::Error> {
        unsafe {
            match MFStartup(MF_VERSION, 0) {
                Ok(()) => Ok(MFInitializer {}),
                Err(e) => Err(e),
            }
        }
    }
}

impl Drop for MFInitializer {
    fn drop(&mut self) {
        unsafe {
            MFShutdown();
        }
    }
}

enum PlayerState {
    Closed = 0,
    Ready,
    OpenPending,
    Started,
    Paused,
    Stopped,
    Closing,
}

struct Player {
    session: IMFMediaSession,
    source: IMFMediaSource,
    videoDisplay: Option<IMFVideoDisplayControl>,
    hwndVideo: HWND,
    hwndEvent: HWND,
    state: PlayerState,
    closeEvent: isize,
}

impl Player {
    pub fn new(video: HWND, event: HWND, url: &str) -> Result<Player, HRESULT> {
        unsafe {
            let closeEvent = CreateEventW(null(), FALSE.0, FALSE.0, null());
            let session = MFCreateMediaSession::<Option<&IMFAttributes>>(None)?;
            let sourceResolver = MFCreateSourceResolver()?;
            let mut objectType: MF_OBJECT_TYPE = MF_OBJECT_MEDIASOURCE;
            let mut mediaSource: Option<IUnknown> = None;
            sourceResolver.CreateObjectFromURL::<_, Option<&IPropertyStore>>(
                &HSTRING::from(url),
                MF_RESOLUTION_MEDIASOURCE.0 as u32,
                None,
                &mut objectType,
                &mut mediaSource,
            )?;

            Ok(Player {
                session,
                source: mediaSource.unwrap().cast::<IMFMediaSource>()?,
                videoDisplay: None,
                hwndVideo: video,
                hwndEvent: event,
                state: PlayerState::Closed,
                closeEvent,
            })
        }
    }
}
