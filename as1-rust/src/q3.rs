use std::error::Error;
use std::fs::File;
use std::io::{BufWriter, Write};
use std::result::Result;
use std::sync::{Arc, Mutex};
use std::thread;

fn main() -> Result<(), Box<dyn Error>> {
    let lock = Arc::new(Mutex::new(BufWriter::new(File::create("JUNK.txt")?)));

    let lock2 = lock.clone();
    lock.lock().unwrap().write_all("before fork\n".as_bytes())?;

    let handle = thread::spawn(move || {
        let mut locked = lock2.lock().unwrap();
        for _ in 0..10 {
            locked.write_all("child\n".as_bytes()).unwrap();
        }
    });

    handle.join().unwrap();
    let mut lock = lock.lock().unwrap();
    for _ in 0..10 {
        lock.write_all("parent\n".as_bytes())?;
    }

    Ok(())
}
