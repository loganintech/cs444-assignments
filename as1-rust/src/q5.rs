use std::process::Command;
use std::thread;

fn main() {
    let handle = thread::spawn(move || {
        let mut execl = Command::new("ls")
            .arg("-l")
            .arg("-F")
            .arg("-h")
            .env_clear()
            .spawn()
            .expect("Command failed to run.");

        if let Err(e) = execl.wait() {
            eprintln!("Execl had an error: {:?}", e);
        }

        let mut execlp = Command::new("ls")
            .arg("-l")
            .arg("-F")
            .arg("-h")
            .spawn()
            .expect("Command failed to run.");

        if let Err(e) = execlp.wait() {
            eprintln!("Execlp had an error: {:?}", e);
        }

        let mut execv = Command::new("ls")
            .args(&["-l", "-F", "-h"])
            .env_clear()
            .spawn()
            .expect("Command failed to run.");

        if let Err(e) = execv.wait() {
            eprintln!("Execv had an error: {:?}", e);
        }

        let mut execvp = Command::new("ls")
            .args(&["-l", "-F", "-h"])
            .spawn()
            .expect("Command failed to run.");

        if let Err(e) = execvp.wait() {
            eprintln!("Execvp had an error: {:?}", e);
        }
    });

    handle.join().unwrap();
}
