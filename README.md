# IPS Patcher in CPP
Some random IPS patcher made in C++ *for fun*.

For the explanation of the IPS format, check out [FORMAT.md](FORMAT.md); thanks to [justsolve.archiveteam.org](http://justsolve.archiveteam.org/wiki/IPS_(binary_patch_format)).

# Usage
The program has a few arguments you can pass to it, like:

`-m`: Specifies the mode of the program.
- `apply`/`a` to apply an IPS patch.
- `create`/`c` to create an IPS patch.

`-p`: Specifies the patch to apply.
- Only works in `apply`/`a` mode.
- Expects a filename.

`-a`: Specifies the file on which the patch will be applied.
- Only works in `apply`/`a` mode.
- Expects a filename.

`-c`: Specifies the source file for a patch.
- Only works in `create`/`c` mode.
- Expects a filename.

`-t`: Specifies the target file for a patch.
- Only works in `create`/`c` mode.
- Expects a filename.
- Changes in this file will be the ones *in the patch*.

`-o`: Specifies the ouput IPS file.
- Only works in `create`/`c` mode.
- Expects a filename.
- Will create the file or will wipe all its content if it exists already.
- All the spotted differences until the **cap of 16 MB** between the `source` and `target` files will be written in it.


# Compiling
## Windows
I haven't tested *yet*, but you could *supposedly* just run:
```shell
$ make
```

Assuming you have `make` in your `$PATH` environment variable, as well as `g++`.

## Linux
Should be the same process as on Windows, run:
```shell
$ make -j$(nproc)
```

# Notes
In case you make changes to the headers (they aren't dependencies to the targets), first run these commands before rebuilding:
```shell
$ make clean -j$(nproc)
```
