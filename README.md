# ImGUI + SFML wrapper project
This is a wrapper project combining SFML and ImGUIs docking branch.

The linking between both SFML and ImGUI was written by Elias Daler on github (https://github.com/eliasdaler/imgui-sfml)
I simply switched out some ImGUI files to work with a different version of ImGUI.

# What does the wrapper do?
This project provides a basic setup for a dockable project window. By passing std::functions to the class you will be able to set headers and the body.
The goal of this wrapper is to allow for a simple and fast creation of dockable projects similar to Visual Studio, Maya or AutoCAD (and more) using ImGUIs and this wrappers code alone.
Docking has been set up and should already work right off the bat. If there are problems with it please let me know and I will try to fix it as soon as possible.

# How to start
1. Download or clone the repository
2. Place all files in GUIProgram, imgui and imgui-sfml into your project folder (or copy & paste the folders)
3. Add SFML to your project if you haven't already done that
4. (Optional, if you copied the complete folders) Add additional include directories for GUIProgram, imgui and imgui-sfml (ex. `$(ProjectDir)GUIProgram`)
5. You are done and can start programming

# Important Note
I do not take credit for any ImGUI or SFML code. I also do not take credit for the code used to link the two libraries together. This was done by Elias Daler!
