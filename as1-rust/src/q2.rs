use std::thread;

fn main() {
    let mut xx = 100;
    let handle = thread::spawn(move || {
        println!("Child XX: {}", xx);
        xx = 777;
        println!("Child XX: {}", xx);
    });

    println!("Parent XX: {}", xx);
    xx = 900;
    println!("Parent XX: {}", xx);

    handle.join().unwrap();
}
