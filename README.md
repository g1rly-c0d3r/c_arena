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
will install the library. The default location is `/usr/local/lib/`

