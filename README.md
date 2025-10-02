# Flight Route Optimization Tool

## Overview

The Flight Route Optimization Tool is a C++ application designed to find the most efficient routes between airports using Dijkstra's algorithm. This tool allows users to minimize travel time, cost, or both, and handle layovers effectively.

## Features

- **Graph Representation**: Uses an adjacency list to represent airports and routes.
- **Dijkstra's Algorithm**: Computes the shortest path based on time, cost, or stops.
- **Dynamic Data Loading**: Loads airport and route data from CSV files.
- **GUI Integration**: Designed for future integration with a graphical user interface (GUI).

## Installation

### Prerequisites

- **C++ Compiler**: Ensure you have a C++ compiler installed (e.g., GCC, Clang).
- **CMake**: Required for building the project.

### Steps

1. **Clone the Repository**

   ```sh
   git clone https://github.com/Satyam-k-jha/flight-Route-Optimization.git
   cd flight-Route-Optimization
   ```

2. **Prepare the Data Files**

   Place your `airports.dat` and `routes.dat` files in the `data/` directory. Ensure that the data files follow the correct format.

3. **Build the Project**

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the Application**

   ```sh
   ./flightRouteOptimization
   ```

## Usage

- **Loading Data**: The application reads `airports.dat` and `routes.dat` from the `data/` directory.
- **Finding Routes**: Test different routes by modifying the source code or future GUI components.

## Project Structure

- `src/` - Contains the source code files.
- `data/` - Data files used for the application (e.g., `airports.dat`, `routes.dat`).
- `CMakeLists.txt` - CMake build configuration file.
- `README.md` - This file.

## Contributing

If you would like to contribute to this project, please follow these steps:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push the branch (`git push origin feature-branch`).
5. Create a new Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For any inquiries or feedback, please contact [Satyam-k-jha](mailto:your-email@example.com).
