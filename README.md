# 🎮 Jonkler Tanks

<div align="center">

![Tank Game](https://img.shields.io/badge/Game-Tank%20Battle-blue?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-green?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C-orange?style=for-the-badge)

</div>

## 📋 Table of Contents
1. [📦 Dependencies](#-dependencies)
   - 1.1. [🐧 Linux Dependencies](#-linux-dependencies)
      - 1.1.1. [📦 Using Distro Package Manager](#-using-distro-package-manager)
      - 1.1.2. [🔧 Using vcpkg](#-using-vcpkg-linux)
   - 1.2. [🪟 Windows Dependencies](#-windows-dependencies)
      - 1.2.1. [🔧 Using vcpkg](#-using-vcpkg-windows)
2. [🔨 Building](#-building)
   - 2.1. [📥 Cloning the Repository](#-cloning-the-repository)
   - 2.2. [🐧 Linux Build Instructions](#-linux-build-instructions)
   - 2.3. [🪟 Windows Build Instructions](#-windows-build-instructions)
3. [🤝 Contribution](#-contribution)
4. [🤖 Adding a bot](#-adding-your-own-bot)

---

## 📦 Dependencies

> ⚠️ **Important**: Install dependencies before building the project!

### 🐧 Linux Dependencies

#### 📦 Using Distro Package Manager
Install the required dependencies using your distribution's package manager.

**🦎 Arch / Manjaro:**
```bash
sudo pacman -S sdl2-compat sdl2_mixer sdl2_ttf sdl2_image gcc cmake make
```

**🐧 Ubuntu / Debian:**
```bash
sudo apt-get update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev gcc cmake make
```

**🎩 Fedora:**
```bash
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel mpg123-devel gcc cmake make
```

#### 🔧 Using vcpkg (Linux)
Alternatively, use vcpkg to manage dependencies.

1. **📥 Install vcpkg:**
   ```bash
   git clone https://github.com/microsoft/vcpkg
   cd vcpkg
   ./bootstrap-vcpkg.sh -disableMetrics
   ./vcpkg integrate install
   ```
   
2. **⚙️ Set up environment variables:**
   ```bash
   echo "export VCPKG_ROOT=$(pwd)" >> ~/.bashrc
   source ~/.bashrc
   ```
   
   **Add vcpkg to PATH (Optional):**
   ```bash
   echo 'export PATH='$(pwd)':$PATH' >> ~/.bashrc
   source ~/.bashrc
   ```

3. **📦 Install SDL dependencies:**

   **🌍 Install globally:**
   ```bash
   ./vcpkg install sdl2 sdl2-image sdl2-mixer[mpg123] sdl2-ttf
   ```
   
   **📁 OR install locally:**
   ```bash
   # Run in jonkler-tanks directory (if vcpkg was added to PATH)
   vcpkg install
   ```

4. **🔧 Fix for Arch systemd build error** (if encountered):
   ```bash
   sudo pacman -S base-devel meson ninja python python-jinja util-linux util-linux-libs acl libpwquality libseccomp tpm2-tss
   ```

### 🪟 Windows Dependencies

#### 🔧 Using vcpkg (Windows)
To manage dependencies on Windows, use vcpkg.

1. **📥 Install vcpkg:**
   ```bash
   git clone https://github.com/microsoft/vcpkg
   cd vcpkg
   .\bootstrap-vcpkg.bat -disableMetrics
   .\vcpkg integrate install
   ```

2. **📦 Install SDL dependencies:**
   ```bash
   .\vcpkg install sdl2 sdl2-image sdl2-mixer[mpg123] sdl2-ttf
   ```

---

## 🔨 Building

> ⚠️ **Prerequisites**: Make sure you have installed all [dependencies](#-dependencies) before building!

### 📥 Cloning the Repository
To start, clone the repository with this command:

```bash
git clone https://github.com/tap-jf/jonkler-tanks
```

### 🐧 Linux Build Instructions
Follow these steps to build the project on Linux:

1. **📁 Go to the project directory:**
   ```bash
   cd jonkler-tanks
   ```

2. **🏗️ Create a build directory and enter it:**
   ```bash
   mkdir build && cd build
   ```

3. **⚙️ Run CMake:**
   ```bash
   cmake ..
   ```

4. **🔨 Build the project:**
   ```bash
   make -j4
   ```

### 🪟 Windows Build Instructions
Follow these steps to build the project on Windows 10/11:

1. **📁 Go to the project directory:**
   ```bash
   cd jonkler-tanks
   ```

2. **🏗️ Create a build directory and enter it:**
   ```bash
   mkdir build && cd build
   ```

3. **⚙️ Run CMake with the vcpkg toolchain and generate files for VS22:**
   ```bash
   cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=[ABSOLUTE PATH TO VCPKG]/scripts/buildsystems/vcpkg.cmake
   ```

4. **🔨 Build in Visual Studio:**
   - Open the generated `.sln` file in Visual Studio
   - Build the solution (Ctrl+Shift+B)

5. **📁 Copy media folder (Important!):**
   After successful build, copy the `media` folder to the output directory:
   ```bash
   # From the project root directory
   xcopy /E /I media build\Debug\media
   ```
   Or manually copy the `jonkler-tanks/media` folder to `jonkler-tanks/Debug/media` or `jonkler-tanks/Release/media`

---

## 🤝 Contribution

Here's how you can contribute to the project:

1. **🍴 Fork the Repository**:
   Go to the [Jonkler Tanks repository](https://github.com/tap-jf/jonkler-tanks) and click "Fork" to create a copy in your GitHub account.

2. **🎨 Format Code with clang-format**:
   Use the provided `.clang-format` file to keep the code style consistent. Run this command to format your code:
   ```bash
   clang-format -i <file>
   ```
   Learn more about `clang-format` in the [official wiki](https://clang.llvm.org/docs/ClangFormat.html). Make sure to format all changed files.

3. **📝 Submit a Pull Request**:
   - Create a new branch for your changes:
     ```bash
     git checkout -b feature/your-feature-name
     ```
   - Commit your changes with clear commit messages.
   - Push your branch to your forked repository:
     ```bash
     git push origin feature/your-feature-name
     ```
   - Open a pull request on the main repository, explaining your changes and linking any relevant issues.

---

## 🤖 Adding your own bot

Want to add your own bot? Follow this comprehensive guide:

👉 **[Bot Development Guide](https://github.com/tap-jf/jonkler-tanks/blob/dev/botHowTo.md)**

---

<div align="center">


Made with 🍻 by the Pivstar games

</div>
