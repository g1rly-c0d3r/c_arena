# Memory Management made easier.

Small arena implementation for the C language. Zero dependancies.
Inspired by Nic Barker's [youtube video](https://youtu.be/hI9aN8ZG4vg) on arenas,
and Ryan Fluery's [blog post](https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator).

## Installation
Installation is handled by `make`. Running 
```bash
make 
```
will compile and build the shared library. Running 
```bash
sudo make install
```
will install the library. The default location is `/usr/local/lib/`. To install to a different location, prepend `INSTALL_DIR=<your/dir/>`
to `make install`

## Usage
Please refer to the [docs](./docs/docs.md) for usage instructions. 

## Development

`test.c` is the binary for testing, if you would like to add a test or test case,
please look in that file.

If you would like to add functionality, then please create tests for it.
When writing tests, please follow the style already in place for printing whether the test passes.
