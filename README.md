# TritiumEngine
Graphics rendering engine built with C++ and OpenGL. I intend to eventually turn this into a physics simulation library.

# Screenshots
![ParticlesBox](https://github.com/PranavBahuguna/TritiumEngine/assets/10980962/ab87ecbe-d669-440a-b81c-ee5c8be66a20)
![Cube](https://github.com/PranavBahuguna/TritiumEngine/assets/10980962/c85f2bf0-2532-4959-b1cc-d766915637fe)

# Cloning the repo
There are several third-party submodules that are included in this project. That said, the CMakeLists.txt file will automatically
download and build all submodules anyway. It is thus only necessary to perform the following command to clone the repo:

```
git clone https://github.com/PranavBahuguna/TritiumEngine
```

You can then use CMake with Visual Studio to generate and build the project files, as well as the RenderingBenchmark app.

# RenderingBenchmark

This is an example application that makes use of TritiumEngine. It registers the performance of drawing many particles bouncing
inside a box, and provides controls for changing particle count and rendering method. It also incorporates use of
[EnTT](https://github.com/skypjack/entt) ECS framework for handling game entites, components and systems.

There are two scenes currently available in the app. The first shows a collection of particles arranged in a ball bouncing in a
box. It provides controls for changing the number of particles shown and the rendering method. The second scene draws 2.5
million particles in a 3D cube and colours them with a gradient dependent on their distanced from the center. It also provides
camera controls for navigating around the space. You can switch between scenes withthe **Enter** key, reload with the **R** key
and exit the app by closing the window or using the **Esc** key.

Future plans for this application will likely focus on implementing physics systems and eventually building versions running on
compute shaders for larger simulations.
