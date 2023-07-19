use cmake;
use std::env;
use std::path::PathBuf;

fn main() {
    let dst = cmake::build("cpp");
    println!("cargo:rustc-link-search=native={}", dst.display());
    println!("cargo:rustc-link-lib=static=mfplay");

    let bindings = bindgen::Builder::default()
        .header("cpp/mfplay.h")
        .clang_args(["-xc++"])
        .vtable_generation(true)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
