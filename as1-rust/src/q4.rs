use std::thread;

fn main() {
    let handle = thread::spawn(move || println!("hello"));

    handle.join().unwrap();
    println!("goodbye");
}
