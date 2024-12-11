# Derived Attribute Framework

This is a basic plugin template using CommonLibSSE-NG.

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
```bat
git clone --recurse-submodules https://github.com/digital-apple/DerivedAttributeFramework
cd DerivedAttributeFramework
```

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Build Output (Optional)
If you want to redirect the build output, set one of or both of the following environment variables:

- Path to a Skyrim install folder: `XSE_TES5_GAME_PATH`

- Path to a Mod Manager mods folder: `XSE_TES5_MODS_PATH`

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```

## Documentation
[Usage](https://github.com/digital-apple/DerivedAttributeFramework/wiki/Usage)

[Translation](https://github.com/digital-apple/DerivedAttributeFramework/wiki/Translation)

## Special Credits

[Qudix](https://github.com/Qudix) for [CommonLibSSE-NG Template Plugin](https://github.com/qudix/commonlibsse-ng-template).

[alandtse](https://github.com/alandtse) and all the people who contributed to [CommonLibVR/NG](https://github.com/alandtse/CommonLibVR/tree/ng).

[powerofthree](https://github.com/powerof3), [Noah](https://github.com/NoahBoddie) and Orrie for general support.

## License
[GPL-3.0](LICENSE)
