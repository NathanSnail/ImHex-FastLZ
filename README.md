# FastLZ ImHex Plugin

## Build and install

Run the following commands
```sh
export IMHEX_SDK_PATH=/usr/share/imhex/sdk
cmake CMakeLists.txt 
make
cp ./fastlz_plugin.hexplug ~/.local/share/imhex/plugins
```
Now when you run imhex you can use `fastlz::*` functions.

## Features

```rs
// Write a FastLZ compressed file of the form
struct FastLZFile {
    u32 compressed_size;
    u32 decompressed_size;
    u8 data[];
}
// to the file
fn compress_to_section(data: [u8], section: std::mem::section) -> ();
// Reads a FastLZ compressed file starting at addr of the above form to the provided section, returns the new address the cursor is at after reading
fn decompress_to_section(compressed: std::mem::section, addr: std::mem::section, output: std::mem::section) -> u128 /* new addr */;
// Returns the main section, not sure why this isn't in the standard library
fn main_section() -> u128 /* main section id */;
```

## Example usage

```rs
struct PLZ {
    padding[4];
    u32 w;
    u32 h;
    padding[4];
};

struct Bitmap<auto w, auto h> {
    u8 data[w * h * 4];
} [[hex::visualize("bitmap", data, w, h)]];

import std.mem;

PLZ plz @ 0;
std::mem::Section decompressed_section = std::mem::create_section("decompressed_section");
fastlz::decompress_to_section(fastlz::main_section(), $, decompressed_section);
Bitmap<plz.w, plz.h> image @ 0 in decompressed_section;
```
