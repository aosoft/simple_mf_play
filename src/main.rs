mod mfplayer;

use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};
use winit::platform::windows::WindowExtWindows;

fn main() {
    let x = mfplayer::MfRuntimeInitializer::new();

    let event_loop = EventLoop::new();
    let window = WindowBuilder::new().build(&event_loop).unwrap();

    let p = mfplayer::Mfplay::new("C:\\WORK\\Media\\Video\\test.mp4", window.hwnd()).unwrap();

    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                window_id,
            } if window_id == window.id() => *control_flow = ControlFlow::Exit,
            Event::WindowEvent {
                event: WindowEvent::Resized(size),
                window_id,
            } if window_id == window.id() => p.resize(size.width as i32, size.height as i32).unwrap(),
            _ => (),
        }
    });
}
