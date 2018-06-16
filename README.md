# GameBoy-Advance-3D-voxel-space-demo
A custom 3D voxel space engine tech demo for the GameBoy Advance written in C compilable with [DevKitARM](https://devkitpro.org/)
![emulator screenshot](https://1.bp.blogspot.com/-SwvNjzXt0g8/WwbdIHc4OnI/AAAAAAAAAm0/NQX5AFOm5zYo5uImaia6Xj_6eIBsNr9wwCLcBGAs/s1600/v2.png)

Full C sourcecode for devkitARM, math table generators, and binary(ROM) is included.

**Controls:**

* **D-Pad:** move
* **A:** Increase altitude
* **B:** Decrease altitude
* **RT:** Turn right
* **LT:** Turn left

## How does it work?
The [voxel space](https://en.wikipedia.org/wiki/Voxel_Space) engine is a 2.5D technology that uses a color map and a depth map to generate a 3D looking environment.

![animation](https://github.com/s-macke/VoxelSpace/raw/master/images/fronttoback.gif)

The environment can be rendered just plotting some vertical lines with a color obtained from the color map, and the initial y-coordinate calculated from the depth map.

However the simple voxel space engine calculations are excesive for the GameBoy Advance ARM CPU, at least if we want a good frame rate. To make it move at a decent frame rate I made the following optimizations:

* All the ARM engine code is copied to the RAM. That's because RAM data access is much faster than running the code directly from the cartridge.
* Horizontal plane render frequency decreases with the distance (see animation).
* A big amount of the engine repeated calculations were stored in hardcoded math tables. This limits the number of camera heights and vertical angles to the stored ones, but multiplies the speed exaggeratedly.
* Using GBA background mode5 decreases the resolution and allows to switch between 2 frame buffers, increasing the speed.
* Using the GBA background scaling and scroll featured, we made some "intra-frames" before the final frame is rendered (that takes a couple of frames) making it more fluid.
* Rotation calculations are fast aproximations of the correct ones, trying to use the **<<** and **>>** shift operations when possible..
* Some other code optimizations (like using **<<** and **>>** instead of multiplications) to make it a bit more faster.

## License

The Voxel Space technology might be still [patented](https://patents.justia.com/assignee/novalogic-inc) in some countries. The color and height maps used for this demo are reverse engineered from the game Comanche.

## Links

Full blog article:
<http://tragicomedy-hellin.blogspot.com/2018/04/motor-3d-para-gameboy-advance.html>

GBADEV.ORG link: <http://gbadev.org/demos.php?showinfo=1481>
