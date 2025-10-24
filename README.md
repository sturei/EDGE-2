# EDGE-2

An experimental shape modeller

## Download and inspect the project

1. open Github Desktop and log in to github
2. File > Clone Repository. Select EDGE-2 and the path to your Github projects.
3. In VSCode and select EDGE-2 or EDGE-2/engines

## Ensure prequisites are present. The following instructions are for MacOS.

First, install Homebrew for installing the prereqs. The line below was copied from https://brew.sh/ in Oct 2025.

```
% /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install CMake for building the C++ code, Github Desktop for interacting with Github, VSCode for developing the code

```
% brew install cmake
% brew install --cask github
% brew install --cask visual-studio-code
```

## Build and test the project

1. Build and test the engines. See the README in that directory

## In case you want a clean start before rebuilding everything

```
% git clean -dxf
```

## License

MIT