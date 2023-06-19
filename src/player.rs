use std::ffi::{OsStr, OsString};
use std::ptr::{NonNull, null, null_mut};
use windows::core::{ComInterface, IUnknown, HRESULT, HSTRING, PCWSTR, PWSTR};
use windows::imp::{CreateEventW, SetEvent};
use windows::Win32::Foundation::{BOOL, E_NOTIMPL, FALSE, HANDLE, HWND, S_OK};
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

#[windows::core::implement(IMFAsyncCallback)]
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
    pub fn new(hwndVideo: HWND, hwndEvent: HWND, url: &str) -> Result<Player, HRESULT> {
        unsafe {
            let closeEvent = CreateEventW(null(), FALSE.0, FALSE.0, null());
            let session = MFCreateMediaSession::<Option<&IMFAttributes>>(None)?;
            let sourceResolver = MFCreateSourceResolver()?;
            let mut objectType: MF_OBJECT_TYPE = MF_OBJECT_MEDIASOURCE;
            let mut mediaSourceUnk: Option<IUnknown> = None;
            sourceResolver.CreateObjectFromURL::<_, Option<&IPropertyStore>>(
                &HSTRING::from(url),
                MF_RESOLUTION_MEDIASOURCE.0 as u32,
                None,
                &mut objectType,
                &mut mediaSourceUnk,
            )?;
            let mediaSource = mediaSourceUnk.unwrap().cast::<IMFMediaSource>()?;

            let topology = MFCreateTopology()?;
            let sourcePd = mediaSource.CreatePresentationDescriptor()?;
            let sourceStreams = sourcePd.GetStreamDescriptorCount()?;
            for i in 0..sourceStreams {
                let mut sourceSd: Option<IMFStreamDescriptor> = None;
                let mut selected: BOOL = Default::default();
                sourcePd.GetStreamDescriptorByIndex(i, &mut selected, &mut sourceSd)?;
                let sourceSd = sourceSd.unwrap();
                if selected.as_bool() {
                    let sourceNode = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE)?;
                    sourceNode.SetUnknown(&MF_TOPONODE_SOURCE, &mediaSource)?;
                    sourceNode.SetUnknown(&MF_TOPONODE_PRESENTATION_DESCRIPTOR, &sourcePd)?;
                    sourceNode.SetUnknown(&MF_TOPONODE_STREAM_DESCRIPTOR, &sourceSd)?;

                    let streamId = sourceSd.GetStreamIdentifier()?;
                    let mediaTypeHandler = sourceSd.GetMediaTypeHandler()?;
                    let majorType = mediaTypeHandler.GetMajorType()?;
                    let outputNode = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE)?;
                    if majorType == MFMediaType_Audio {
                        outputNode.SetObject(&MFCreateAudioRendererActivate()?)?;
                    } else if majorType == MFMediaType_Video {
                        outputNode.SetObject(&MFCreateVideoRendererActivate(hwndVideo)?)?;
                    }
                    topology.AddNode(&sourceNode)?;
                    topology.AddNode(&outputNode)?;
                    sourceNode.ConnectOutput(0, &outputNode, 0)?;
                }
            }

            session.SetTopology(0, &topology)?;

            Ok(Player {
                session,
                source: mediaSource,
                videoDisplay: None,
                hwndVideo,
                hwndEvent,
                state: PlayerState::Closed,
                closeEvent,
            })
        }
    }
}


impl IMFAsyncCallback_Impl for Player {
    fn GetParameters(&self, _: *mut u32, _: *mut u32) -> Result<(), windows::core::Error> {
        Err(windows::core::Error::from(E_NOTIMPL))
    }
    fn Invoke(&self, result: Option<&IMFAsyncResult>) -> Result<(), windows::core::Error> {
        unsafe {
            let event = self.session.EndGetEvent(result)?;
            let meType = MF_EVENT_TYPE(event.GetType()? as i32);
            if (meType == MESessionClosed) {
                SetEvent(self.closeEvent);
            } else {
                self.session.BeginGetEvent(&self.cast::<IMFAsyncCallback>()?, None)?;
            }
            Ok(())
        }
    }
}
