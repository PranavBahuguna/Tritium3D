# TritiumEngine
Graphics rendering engine built with C++ and OpenGL. I intend to eventually turn this into a physics simulation library.

# Screenshot (05/03/2024)
![RenderingBenchmark](https://github.com/PranavBahuguna/TritiumEngine/assets/10980962/ab87ecbe-d669-440a-b81c-ee5c8be66a20)

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
[EnTT](https://github.com/skypjack/entt) ECS framework for handling game entites, components and systems. It provides the following
key controls:

- **D** - Default rendering mode
- **I** - Instanced rendering mode
- **G** - Instanced + geometry shader rendering mode
- **1** - Simulate 1 entity
- **2** - Simulate 10 entities
- **3** - Simulate 100 entities
- **4** - Simulate 1000 entities
- **5** - Simulate 10000 entities
- **6** - Simulate 100000 entities
- **7** - Simulate 1000000 entities

I have found instancing to provide a significant speedup over default rendering (draw call for each entity), while using a geometry
shader in conjunction with instancing provides another small boost in performance. I plan on adding additional scenes and
implementing more techniques to improve performance over time. 
