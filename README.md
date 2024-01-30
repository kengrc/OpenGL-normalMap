# Setup Guide

Clone the repository and follow these steps:

* Go to your Project Properties and add your "include" folder inside the "Libraries" folder to the Include directories
* Do the same with the "Library Directories" and add your "lib" folder  
![image](https://github.com/kengrc/OpenGL-normalMap/assets/101011406/acad61d1-89fc-4c88-b4be-608f9605ca24)
* now go to your Linker settings and add two additional dependecies to your input: add "glfw3.lib" and "opengl32.lib"

  
![image](https://github.com/kengrc/OpenGL-normalMap/assets/101011406/3969ee07-f94e-4d1d-8d65-2ba1035573cb)

You should be good to go. If you're encountering any problems, please refer to https://learnopengl.com/
