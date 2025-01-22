# SandboxSim

This is a simple **Falling Sand Simulator** built using **C++** and **OpenGL**. The simulation allows particles to interact dynamically, creating visually engaging patterns and behaviors.

## Features

- Real-time sand simulation.
- Interactive controls for spawning and manipulating sand particles.
- Visualized using OpenGL for fast rendering.

## Technologies Used

- **C++** for the core logic and application structure.
- **OpenGL** for rendering the simulation.
- **GLFW** for window and input management.
- **GLAD** as the OpenGL function loader.

## Prerequisites

- **CMake** 3.10 or higher ([Download here](https://cmake.org/download/))
- **OpenGL** 3.3
- A **C++ compiler** (Only tested with Visual Studio)

## How It Works

The simulator is built around a **grid-based system**, where each cell represents a particle of sand or an empty space. Here's a brief overview:

- On each frame, the grid updates by checking and updating the state of every cell based on predefined rules.
- The rendering system visualizes the grid in real time, with each cell corresponding to a pixel or a small block on the screen.

This approach ensures efficiency while allowing dynamic and realistic sand behavior.

## Build Instructions (Windows)

1. Clone the repository:

   ```sh
   git clone https://github.com/MntlGrlla/SandboxSim.git
   ```

2. Update the submodule:

   ```sh
   git submodule update --init --recursive
   ```

   This step ensures that the GLFW library, which is included as a submodule, is correctly initialized.

3. Create a `build` directory:

   ```sh
   mkdir build
   ```

4. Navigate into the `build` directory:

   ```sh
   cd build
   ```

5. Generate the build files using CMake:

   ```sh
   cmake ..
   ```

6. Build the project:

   ```sh
   cmake --build .
   ```

7. Locate the executable:

   - If using the Visual Studio compiler, the executable will likely be in the `Debug` folder inside the `build` directory.
   - Otherwise, it should be directly in the `build` folder.

8. Run the executable:

   ```sh
   ./SandboxSim
   SandboxSim
   ```

   Use `./SandboxSim` for a bash environment and `SandboxSim` for the Windows command line.

## Future Features

- Support for multiple particle types (e.g., water, walls, fire) with unique behaviors and interactions.
- A level editor for designing custom setups and scenarios.
- Enhanced visualization and performance improvements.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to improve the simulator.

---

**Enjoy experimenting with the dynamics of the Falling Sand Simulator!**

