# Pong

A Pong clone written in C++ using OpenGL and GLFW. It is a testbed for implementing game optimizations.

![Screenshot](.github/screenshot.png)

---

### Build Requirements

* CMake (≥ 3.16)
* C++17-capable compiler (MSVC, Clang, or GCC)

---

### Build (Linux / macOS)

```bash
git clone https://github.com/TasinFarhanMC/pong
cd pong
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
````

### Build (Windows, MSVC)

Use CMake GUI or:

```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

> Output binaries will be placed in `bin/`.

---

## Controls

| Key     | Action              |
| ------- | -----------------   |
| `W / S` | Move left paddle    |
| `↑ / ↓` | Move right paddle   |
| `ENTER` | Start after a score |
| `ESC`   | Exit                |

---

## Assets

Assets (fonts, shaders, textures) are located in the `assets/` folder and must be next to the binary.

---

## Prebuilt Releases

Download prebuilt versions:

Released:
* [Release Page](https://github.com/TasinFarhanMC/pong/releases) for full catalogue of release
* [Linux Standalone x64](https://github.com/TasinFarhanMC/pong/releases/latest/download/Pong_Linux_Standalone_x64.zip)
* [Windows Standalone x64](https://github.com/TasinFarhanMC/pong/releases/latest/download/Pong_Windows_Standalone_x64.zip)

InDev:

* [Linux Standalone x64](https://nightly.link/TasinFarhanMC/pong/workflows/ci.yaml/main/binaries-ubuntu-latest-clang.zip)
* [Windows Standalone x64](https://nightly.link/TasinFarhanMC/pong/workflows/ci.yaml/main/binaries-windows-latest-cl.zip)

---

## License

MIT License © 2025 [Tasin Farhan](https://github.com/TasinFarhanMC)
