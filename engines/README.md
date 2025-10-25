# EDGE-2/engines

Modelling engine (and potentially other engines in future)

## Prequisites

Ensure you have the prerequisites as described in the README in the root folder of the project.

Then install magnum globally, used by the C++ desktop application (Note: this will soon be discarded).
```
% brew install mosra/magnum/magnum
```

## Build and test the engines

```
% cd engines
% mkdir build && cd build
% cmake ..
% make
% ctest
```

## Run the modelling service interactively

```
% ../scripts/e2_modellingService.zsh
```

