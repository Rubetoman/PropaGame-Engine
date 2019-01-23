# PropaGame Engine
This is a C/C++ game engine created by Rubén Crispín for the "Universidad Politécnica de Cataluña" as a proyect for the Master Degree in Videogame Design & Development.

https://github.com/Rubetoman/PropaGame-Engine

![alt text](https://pbs.twimg.com/media/DxcWmwKX4AENdpp.jpg:large)

## Executing Instructions
-------------
Inside the folder Game there is a file called "PropaGame Engine.exe"; just execute it.

## Controls
-------------
### Editor Camera
a.	While Right clicking: “WASD” fps-like movement, "QE" translates verticaly and with mouse movement you can look around.
b.	Mouse wheel zooms in and out.
c.	Alt+Left click orbits the selected Game Object (if none is selected will orbit around (0,0,0) of the world.
d.	Pressing “F” the camera looks at the selected Game Object.
e.	Holding SHIFT increases movement and rotating speed.

### Game Object manipulation:
a. Game Objects with mesh component can be selected by clicking at them.
b. When a Game Object is selected a Guizmos will appear, it can be used to translate, rotate or scale the selected Game Object.
c. The buttons on the upper-left corner of the Editor Window can be used to change between Guizmos type (translation, rotation, scale).

![alt text](https://s2.gifyu.com/images/Guizmo.gif)

#### Hierarchy Window
On this window all the Game Objects are listed, by using the right click over the name of any Game Object they can be deleted, duplicated or moved up on the hierarchy.
Also by right click, new empty Game Objects or basic geometrical shapes can be created.


## Components
The engine works with component that are attached to Game Objects. This components parameters can be seen the inspector window for the selected GameObject.

### Transform
All Game Objects have this component by default. It can modify the position, rotation or size of the Game Object. On the inspector window it will show this parameters on local coordinates. The “Default” button will reset the object parameters to default (position = (0,0,0), rotation = (0,0,0), scale = (1,1,1).

### Mesh
This component will contain a mesh, that can be selected on the dropdown menu shown on the inspector window. On that dropdown menu only meshes located on “Library/Meshes” will appear.

### Material
This component can add used textures or colors for the mesh rendering, also some parameters can be modified. At the moment Blinn-Phong Shading is used by default with diffuse, specular, ambient and emissive variables. A dropdown to select the shader to be used is shown on the Inspector Window currently only Phong and Blinn are available.

![alt text](https://s2.gifyu.com/images/material.gif)

### Camera
The camera component includes a frustum that can be rendered to a texture and displayed on an editor window. By default, one Game Camera is loaded but more can be added. To open a window rendering the camera it can be selected on the Main Menu Bar.

-	Frustum culling is used by default on every camera, this option can be disabled on the inspector.

### Light
This component is still in development, by the moment only the default light loaded on the scene can be modified and has a effect to the scene.


## Files
-------------
The engine uses his own file format:
- FBX files and DDS textures can be drag & dropped from the corresponding assets folder or imported from the built-in folder explorer.
- Imported meshes and textures will be located on “Library/Meshes” and “Library/ Textures”.
- Own meshes files can be drag & dropped on the scene to load the directly or loaded by selecting them on the built-in explorer.

Paths to import files:
For FBX files (Assets/Models)
For DDS textures (Assets/Textures)

![alt text](https://s2.gifyu.com/images/Import.gif)

## Scene Loading
---------
Scenes can be saved and loaded by using the Main Menu Bar buttons for scenes.
-On saving it will ask the user for a name, if the scene is already saved you can use the “Save” button to save it with the same name. 
- On load a pop up will appear, there the name of the scene can be inserted manually or selected from the list.

![alt text](https://s2.gifyu.com/images/SceneLoad.gif)

## Play/Pause/Stop/Skip buttons
------
On the middle of the Main Menu bar this buttons can be seen, they are for future use. By clicking on Play the scene will be saved and loaded again on Stop, so any changes done while in play mode will be discarded.

![alt text](https://s2.gifyu.com/images/Play.gif)

## Quadtree
------
Although quadtree is implemented, it stills at development and is not used by default. It can be activated on the Configuration Window, also there is an option to draw it on the scene.

## License
-------------
MIT License

Copyright (c) 2018 Rubén Crispín De la Cruz
