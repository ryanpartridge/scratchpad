use std::error::Error;
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::{thread, time};

fn main() -> Result<(), Box<dyn Error>> {
    println!("Hello, world!");

    let finish_up = Arc::new(AtomicBool::new(false));
    signal_hook::flag::register(signal_hook::consts::SIGTERM, Arc::clone(&finish_up))?;
    while !finish_up.load(Ordering::Relaxed) {
        println!("Waiting for the end");
        thread::sleep(time::Duration::from_secs(2));
    }
    println!("All done!");

    Ok(())
}
