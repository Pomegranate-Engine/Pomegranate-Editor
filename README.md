# Pomegranate-Editor
A simple customizable 2D Game Engine with a heavy focus on organization, and visual pleasure.

![Full Scene.png](img%2FFull%20Scene.png)

# About

The pomegranate Editor is an extension of the base Pomegranate Engine.
It is a simple 2D game engine that uses a unique Entity-Component-System (ECS) architecture with more traditional Game object architecture influences. 
The engine is written in C++ and uses the SDL2 library for rendering and input handling. 
The engine is designed to be simple and easy to use, with a focus on flexibility and customization.
The engine comes with a Lua Wrapper so that systems/components can be written in lua.

![Searching By Tag.png](img%2FSearching%20By%20Tag.png)
The engine also comes with a custom file manager that uses a tag system to organize files.

![Hierarchy.png](img%2FHierarchy.png)
Pomegranate implements a unique Graph Hierarchy system that makes it much easier to view entity links. 
This is required because of the ECS architecture allowing for entities to be linked to multiple groups at once.
The engine is still in development and is not yet feature complete.

## Features

- [x] Entity-Component-System architecture
- [x] C++ and Lua systems/components (WIP)
- [x] Tilemap support (Reworking)
- [x] Physics support
- [x] Customizable rendering pipeline
- [x] Unique graph hierarchy
- [x] Unique file manager tag system
- [x] Incredibly lightweight and fast
- [x] (Hopefully) Easy to use and understand
- [x] Easy to extend and customize (WIP)

## TODO

- [ ] Finish Lua Wrapper (Switch to sol2)
- [ ] Add Teascript Wrapper
- [ ] Finish C++ integration
- [ ] Add editor tilemap support
- [ ] Add inspector custom types support
- [ ] Add shaders via Vulkan
- [ ] Bunch of other stuff
- [ ] MAJOR CLEANUP
- [ ] Add documentation
- [ ] Create unique icons for the editor
- [ ] Add more editor tools
- [ ] Add more physics support
- [ ] Add more rendering support
- [ ] Add more audio support
- [ ] Fix weird little annoyances
- [ ] Setup an addons system

## Possible Future Features

- [ ] Fully switch to Vulkan
- [ ] Add 3D support
- [ ] Add networking support
- [ ] Add VR support
- [ ] Add scripting support for other languages
- [ ] Create/finish PomPom scripting language

# Building

## Dependencies
[SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.2)

[SDL2_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.2)

[SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.22.0)

[SDL2_mixer](https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.8.0)

[LuaJIT](https://luajit.org/download.html)


## Building

Idk about the rest.
Figure it out. Good luck! 🥰

### Suggestions

Trying run ```cmake .```  and see what breaks.

# Contribution Guidelines

I need help.

## Bug Reports

If you find any issues or bugs, please check to see if they were already reported in the [issue tracker](https://github.com/Pomegranate-Engine/Pomegranate-Engine/issues). 
If not, please open a new issue with a clear description, steps to reproduce, and any relevant information.

## Feature Requests

If you have ideas for new features or improvements, create a new issue and clearly describe the improvement or feature, 
and if possible, provide examples or use cases.

## Pull Requests

We welcome and appreciate contributions! If you plan to submit a pull request:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes, ensuring the code follows our coding standards.
4. Write tests if applicable.
5. Ensure the documentation is updated.
6. Open a pull request, explaining the changes and providing context.

## Coding Standards

- Follow the established coding style and conventions. You can find the style guide below.
- Write clear and concise code with meaningful variable and function names.
- Document your code appropriately to help others understand your contributions.

## Style Guide

This is based on my personal preferences and is subject to change.

- Use snake_case for variable and function names.
- Use CamelCase for class names.
- Use ALL_CAPS for constants.
- Use spaces for indentation. (4 spaces)