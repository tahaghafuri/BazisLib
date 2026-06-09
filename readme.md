BazisLib Overview
=================

BazisLib is a legacy C++ framework that provides a convenient multi-platform abstraction layer for the following APIs:
* Thread and synchronization API
* File access API
* Atomic operations
* Reference counting
* Sockets (Windows Kernel not supported)
* FS path manipulation
* Configuration storage

BazisLib also provides some convenience classes that simplify driver development for Windows and MacOS in C++.

BazisLib can be compiled for the following targets:
* Windows user-mode using Visual Studio compiler
* Windows kernel-mode using WDK 7.x or 8.x
* Linux or MacOS user-mode using GCC or Clang
* MacOS kernel-mode

Repository layout:
* `bzscore` - core portability primitives, strings, buffers, files, paths, sockets and synchronization.
* `bzshlp` - higher-level helpers for drivers, storage, serialization, logging and platform integrations.
* `bzsnet` - network helpers such as buffered sockets and Base64 encoding/decoding.
* `examples` - sample projects using BazisLib.
* `tests` - Visual Studio-based regression and cross-platform test projects.
* `_installer` - installer metadata and release notes.

The best way to start exploring BazisLib is to open the `tests\BigCrossPlatformTest\BigCrossPlatformTest.sln` solution and build/run the Windows configuration. Other configurations of the solution target POSIX user-mode, Windows kernel-mode and MacOS kernel-mode environments. The `tests\CommonFunctionalityTests\CommonFunctionalityTests.sln` solution contains focused regression tests for shared helpers.

Recent maintenance
------------------

The `bzsnet` Base64 decoder now rejects invalid alphabet characters and malformed padding instead of silently treating some invalid bytes as `A`. Regression coverage for ANSI, wide-character, whitespace-tolerant and invalid-input decoding lives in `tests\CommonFunctionalityTests\Base64Test.cpp`.
