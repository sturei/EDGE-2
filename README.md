# EDGE-2

An experimental shape modeller

## Prequisites (instructions for MacOS)

First, install Homebrew. For reference, the line below was copied from https://brew.sh/ in Oct 2025.

```
% /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then:

```
% brew install cmake
% brew install --cask github
% brew install --cask visual-studio-code
% brew install mosra/magnum/magnum
```

## Download and inspect the project

1. open Github Desktop and log in to github
2. File > Clone Repository. Select EDGE-2
3. Open in VSCode

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