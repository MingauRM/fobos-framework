# Fobos game framework

**A Game Framework highly inspired by `LÖVE2D`, `Roblox` and `Lune`. Using Luau as it's scripting language**

<!-- markdownlint-disable-next-line MD045 MD033 -->
<div style="text-align: center;"><img src="assets/images/fobos-logo.svg" width="256" height="256" /></div>

## What is Fobos

Fobos is a free, easy-to-use game framework, using Luau as it's scripting language.

## Usage

### 1. Create your project's folder

Just create a folder and name it however you want.

### 2. Setup Fobos

Firstly, install the `Luau Language Server` VSCode extension made by `JohnnyMorganz`. Then, download Fobos to your computer and run the following command:

```bash
fobos setup .
```

This will generate all Luau Definition and Documentation files needed for IntelliSense.

### 3. Create a `main.luau` file

Go to your folder and add a file, name it `main.luau`

```txt
project_folder
|
\_main.luau
```

### 4. Start scripting

Write the following structure:

```luau
-- Runs once on startup.
function fobos.startup()

end

-- Runs every frame, good to calculate physics, for example.
function fobos.tick(dt: number)

end

-- Runs every frame when you can draw things on the screen.
function fobos.render()

end
```
