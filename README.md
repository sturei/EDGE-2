# EDGE-2

An experimental shape modeller

## Prequisites (instructions for MacOS)

First, install Homebrew for installing the prereqs. For reference, the line below was copied from https://brew.sh/ in Oct 2025.

```
% /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then install CMake for building the code, Github Desktop for interacting with Github, VSCode for development the code and Magnum for OpenGL Graphics.

```
% brew install cmake
% brew install --cask github
% brew install --cask visual-studio-code
% brew install mosra/magnum/magnum
```

## Download and inspect the project

1. open Github Desktop and log in to github
2. File > Clone Repository. Select EDGE-2 and the path to your Github projects.
3. Open Folder in VSCode and select EDGE-2
4. Switch Intellisense to use the tag parser. The new parser doesn't work very well with namespaces

```
Go to Code, Settings, Settings, Workspace, Extensions, C/C++, Intellisense
Locate "C_Cpp: Intelli Sense Engine"
From the dropdown, select Tag Parser
Restart VSCode when asked to do so
```

## Build and test the project

```
% cd <path_to_your_github_projects>/EDGE-2
% mkdir build && cd build
% cmake ..
% make
% ctest
```

## Run the modelling service interactively

```
% ../scripts/e2_modellingService.zsh
```

## In case you want a clean start before rebuilding everything

```
% git clean -dxf
```

## License

MIT