# Scaling Flag Remover VR

A Fallout 4 VR port of **Scaling Flag Remover by GELUXRUM**. It removes the `PC Level Mult` flag from affected NPC records and assigns a fixed level using the higher of each record's configured minimum and maximum calculation levels.

**The original Scaling Flag Remover is required. Install the original mod first, then install this port after it so the VR DLL overwrites the original DLL.**

## Requirements

- Fallout 4 VR **1.2.72.0**.
- **F4SEVR 0.6.21** and **VR Address Library for F4SEVR**.
- [Original Scaling Flag Remover by GELUXRUM](https://www.nexusmods.com/fallout4/mods/73976), with its listed requirements.

## Installation

1. Install the original Scaling Flag Remover and its requirements.
2. Install this port as a separate mod in Mod Organizer 2.
3. Place the port **below the original in MO2's left pane**, allowing it to overwrite `F4SE\Plugins\GLXRM_ScalingFlagRemover.dll`.
4. Launch the game through F4SEVR.

MO2's left pane controls file-overwrite priority. Keep the original mod's other files installed.

For manual installation, install the original mod first and replace its DLL with the VR DLL at:

```text
Data\F4SE\Plugins\GLXRM_ScalingFlagRemover.dll
```

## Configuration and SCOURGE

**There is no INI to create or configure.** The plugin processes NPC records when game data becomes ready.

[SCOURGE VR](https://github.com/brunocatani/Scourge-VR) requires Scaling Flag Remover to be successfully loaded by F4SEVR. Install the original mods, then let their respective VR ports overwrite the native DLLs.

## Logs and support

The log is:

```text
Documents\My Games\Fallout4VR\F4SE\GLXRM_ScalingFlagRemover.log
```

It reports `Removed PC Level Mult from ... NPC records` after processing the loaded records. The count depends on the installed plugins. The same-session `f4sevr.log` records whether `GLXRM_ScalingFlagRemover.dll` loaded successfully.

For [VR-port issues](https://github.com/brunocatani/ScalingFlagRemover-VR/issues), include both logs and the affected NPC or mod details. Direct VR-port bug reports to this repository.

## Building from source

Install Visual Studio 2022 with Desktop development with C++, a Windows SDK, CMake 3.25 or newer, vcpkg, and CommonLibF4VR. Set `VCPKG_ROOT` to your vcpkg checkout and `COMMON_LIB_F4VR_PATH` to CommonLibF4VR's `CommonLibF4` subdirectory.

```powershell
cmake --preset fast
cmake --build --preset fast
```

The preset builds Release with compiler concurrency capped at two processes. The DLL is written to `build-fast/Release/GLXRM_ScalingFlagRemover.dll`. Put machine-specific settings and optional deployment paths in an ignored `CMakeUserPresets.json`.

## Credits and permission

- **GELUXRUM** — original creator of Scaling Flag Remover. Thank you for creating the mod and granting permission for this VR port. Please support the original mod and its author.
- **brunocatani** — VR adaptation and maintenance.
- The **F4SEVR** and **CommonLibF4VR** contributors.

The original mod remains a mandatory separate download.

## License

This VR port is distributed under the **GNU General Public License, version 3**. See [LICENSE](LICENSE).

The original code's **Copyright (c) 2023 GELUXRUM** and MIT license notice are preserved in [LICENSE.upstream](LICENSE.upstream). Third-party source files retain their own copyright and license notices.
