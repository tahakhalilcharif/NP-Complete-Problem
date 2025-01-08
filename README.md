# Project: Algorithm Implementations

This repository contains implementations of various deterministic and non-deterministic algorithms, as well as visualization tools for analyzing their results. Below is a breakdown of the project structure and how to use each component.

## Table of Contents
- [Overview](#overview)
- [Files](#files)
- [Setup](#setup)
- [Usage](#usage)
  - [Running Algorithms](#running-algorithms)
  - [Visualization](#visualization)
- [Requirements](#requirements)
- [Contributing](#contributing)
- [License](#license)

## Overview
This project implements the following algorithms:
- **Non-deterministic General Algorithm**: A generic framework for non-deterministic problem-solving.
- **Simulated Annealing**: A non-deterministic algorithm for optimization problems.
- **IDA***: A deterministic algorithm for graph search and problem-solving.
- **SAT Verification**: A verification algorithm for Boolean satisfiability problems.

Additionally, Python scripts are provided for visualizing the results of Simulated Annealing and IDA* algorithms.

## Files
| File                              | Description                                      |
|-----------------------------------|--------------------------------------------------|
| `algorithme_non_deterministe_general.c` | General non-deterministic algorithm implementation. |
| `algo_non_deterministe_Simulated_Annealing.c` | Simulated Annealing algorithm implementation.       |
| `ida_star.c`                      | IDA* (Iterative Deepening A*) algorithm implementation. |
| `sat.c`                           | SAT verification algorithm.                      |
| `generate_simulated_annealing.py` | Visualization of Simulated Annealing results.    |
| `ida_star.py`                     | Visualization of IDA* algorithm results.         |

## Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/your_username/algorithm-implementations.git
   cd algorithm-implementations
   ```

2. Install required dependencies for Python scripts:
   ```bash
   pip install -r requirements.txt
   ```

## Usage

### Running Algorithms

#### Non-deterministic General Algorithm
Compile and run:
```bash
gcc algorithme_non_deterministe_general.c -o algorithme_general
./algorithme_general
```

#### Simulated Annealing
Compile and run:
```bash
gcc algo_non_deterministe_Simulated_Annealing.c -o simulated_annealing
./simulated_annealing
```

#### IDA*
Compile and run:
```bash
gcc ida_star.c -o ida_star
./ida_star
```

#### SAT Verification
Compile and run:
```bash
gcc sat.c -o sat_verification
./sat_verification
```

### Visualization

#### Simulated Annealing Results
Run the Python visualization script:
```bash
python generate_simulated_annealing.py
```

#### IDA* Results
Run the Python visualization script:
```bash
python ida_star.py
```

## Requirements

- **C Compiler**: GCC or any C compiler.
- **Python**: Version 3.7 or later.
- Python Libraries (listed in `requirements.txt`):
  - `matplotlib`
  - `numpy`

## Contributing
Contributions are welcome! Please submit a pull request or create an issue to suggest improvements or report bugs.

## License
This project is licensed under the [MIT License](LICENSE).

