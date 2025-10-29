# EDGE-2

An experimental shape modeller

## Download and inspect the project

If using Github Desktop:

1. Log in to github
2. File > Clone Repository. Select EDGE-2 and the path to your Github projects.
3. In VSCode and select EDGE-2 or EDGE-2/engines

Otherwise, clone the repo however you would normally do it.

## Ensure prequisites are present. 

The following instructions are for zshell on MacOS.

Homebrew is required to install and manage some of the prereqs.
If not installed already, install Homebrew globally by entering the following at a command prompt. The line below was copied from https://brew.sh/ in Oct 2025.

```
% /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Note that installing Homebrew will install the Command Line Tools as a side-effect. The Command Line Tools are required.

CMake is required for building the C++ code. 
GitHub destop is recommended for interacting with Github.
VSCode is recommended for developing with C++, typescript and other languages.
npm and node are required for building the typescript code; nvm is recommended for installing and managing them as below. Note that nvm should NOT be installed via Homebrew. Use the instructions below.

Install them globally as follows:

```
% brew install cmake
% brew install --cask github
% brew install --cask visual-studio-code
% touch ~/.zshrc ~/.bash_profile
% curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash
% nvm install node
```
Note that nvm has a quirk that .zshrc and .bash_profile need to exist before running the curl command to install it. 
If you get a warning about command completion on starting a new shell window, disable it in ~/.zshrc. I don't know why nvm puts that in!

See https://stackoverflow.com/questions/16904658/node-version-manager-install-nvm-command-not-found 

## Build and test the project

For each of the subfolders (engines and web at time of writing)
```
% cd <folder>
% <follow the instructions in the README that you will find there>
% code            # opens Visual Studio Code
```

### Tips and troubleshooting

In Visual Studio Code, it is recommended that you install the following extensions (from the extension button in the sidebar of VSCode)

- C/C++
- CMake Tools
- Github Copilot
- Github Copilot Chat
- Markdown All in One
- Tailwind CSS Intellisense

In Visual Studio Code, if the red squiggles show too many false positives, switch to the old tag parser:

```
Go to Code, Settings, Settings, Workspace, Extensions, C/C++, Intellisense
Locate "C_Cpp: Intelli Sense Engine"
From the dropdown, select Tag Parser
Restart VSCode when asked to do so
```

In Visual Studio Code, to debug C++:

```
From the left toolbar, set the Launch Target and the Debug Target to the target you want to debug
Configure launch.json for the debugger to point to the executable and the cwd in build
```

Note that VSCode's Integrated Terminal is not supported in the mac default llvm debugger.

In Visual Studio Code, if you get a warning about unrecognised plugin for tailwind, add it to the file.associations (you can do it from Settings)
```
"files.associations": {
  "*.css": "tailwindcss"
}
```


## In case you want a clean start before rebuilding everything

```
% git clean -dxf
```

## License

MIT