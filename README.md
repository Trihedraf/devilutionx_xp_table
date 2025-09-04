# DevilutionX XP HTML Table Generator

This program calculates the average experience per hitpoint from each dungeon
level for each character level from 1 to 50, and prints it out as an HTML-table.

## Building

### Linux

Building for linux using `cmake`

```sh
cmake -Bbuild -S.
cmake --build build -j $(getconf _NPROCESSORS_ONLN)

```

Compiled program is in `build/` and is `diabloxp_devx`

### Windows

Building for Windows in Linux or in WSL requires the packages `cmake` and `mingw-w64`

```sh
 cmake -Bbuild  -S. -DCMAKE_TOOLCHAIN_FILE=CMake/platforms/mingw.toolchain.cmake
 cmake --build build -j $(getconf _NPROCESSORS_ONLN)
```

Compiled program is in `build/` and is `diabloxp_devx.exe`

## Running

### Linux

```sh
./diabloxp_devx > xp.html
```

### Windows

```pwsh
.\diabloxp_devx > xp.html
```

## Credits

- Original version made for vanilla Diablo by "Willy Nyland, aka Soul Toucher"
