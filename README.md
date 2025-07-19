# Sudoku Solver in C++

A command-line based Sudoku solver written in C++ that allows users to input a 9x9 puzzle and solves it using a backtracking algorithm. It also validates the puzzle before solving, times the execution, and saves both input and solution to text files.

## Features

- User-friendly terminal input
- Input validation (checks for correct formatting and puzzle integrity)
- Solves using classic backtracking algorithm
- Times how long it takes to solve the puzzle
- Saves both original puzzle and solution to a `.txt` file
- Neatly formatted grid output

## How It Works

1. **Input**: User is prompted to enter the puzzle row by row (use `0` for empty cells).
2. **Validation**: Checks whether the entered puzzle follows Sudoku rules.
3. **Solving**: If valid, the puzzle is solved using recursive backtracking.
4. **Output**:
   - Displays the solution in the console.
   - Asks for an output file name to save both puzzle and solution.
   - Saves the puzzle and its solution in a visually formatted way.

## Example Input Format

When prompted:
Row 1: 5 3 0 0 7 0 0 0 0
Row 2: 6 0 0 1 9 5 0 0 0
...
Row 9: 0 0 0 0 8 0 0 7 9

## Build & Run
### Requirements

- C++11 or higher
- g++ or any compatible compiler

### Compile

```bash
g++ -std=c++11 sudoku_solver.cpp -o sudoku_solver
```
Run
./sudoku_solver
Follow the on-screen instructions to input your puzzle and receive the solution.

Files Generated
sudoku_input.txt: The puzzle you entered
sudoku_output.txt (or your custom filename): The original puzzle and its solved state

Sample Output
Solved Sudoku:
+-------+-------+-------+
| 5 3 4 | 6 7 8 | 9 1 2 | 
| 6 7 2 | 1 9 5 | 3 4 8 | 
| 1 9 8 | 3 4 2 | 5 6 7 | 
...
+-------+-------+-------+
Solved in 0.00215 seconds.
Puzzle and solution saved to 'sudoku_output.txt'


## Screenshot
### Code:
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20111910.png?raw=true)
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20111959.png?raw=true)
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20112422.png?raw=true)
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20112438.png?raw=true)

### Output:
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20112718.png?raw=true)
#### input.txt
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20112724.png?raw=true)
#### output.txt
![image](https://github.com/ashishyadav-1510/CodeAlpha_Sudoku_Solver/blob/main/Screenshot/Screenshot%202025-07-19%20112736.png?raw=true)

## Video

[Video on YouTube](https://youtu.be/wtPGRVgTBvQ)

## Explaination:

Includes and Namespaces

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
using namespace std;
#include <iostream>: Enables console input/output.
#include <vector>: Allows use of the std::vector container.
#include <fstream>: For reading/writing files.
#include <sstream>: Allows parsing of strings using string streams.
#include <iomanip>: Supports formatted output (not used here).
#include <string>: Enables string manipulation.
#include <chrono>: Used for measuring time.
using namespace std;: Avoids writing std:: before standard functions.

Class Definition

class SudokuSolver {
private:
    int grid[9][9];
    int original_grid[9][9];
A class SudokuSolver encapsulates the functionality.
grid: the main working grid.
original_grid: stores the user's input for later output.

User Input

void inputFromUser() {
Begins function to collect puzzle input from the user.

    std::cout << "Enter Sudoku Puzzle row by row:\n[use 0 for empty cells and space between each value]\n";
Prompts the user for the input format.

    for (int i = 0; i < 9; ++i) {
        std::vector<int> row;
Iterate over 9 rows. row stores values temporarily.

        while (true) {
            std::cout << "Row " << i + 1 << ": ";
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int num;
            row.clear();
Continuously ask for a row until valid input is received.

            while (iss >> num) {
                if (num < 0 || num > 9) {
                    row.clear();
                    break;
                }
                row.push_back(num);
            }
Parses numbers and checks they're in the 0–9 range.

            if (row.size() == 9) break;
            std::cout << "Invalid input. Enter exactly 9 numbers between 0 to 9.\n";
        }
Validates that 9 entries were given.

        for (int j = 0; j < 9; ++j) {
            grid[i][j] = row[j];
            original_grid[i][j] = row[j];
        }
    }
}
Copies the valid input into both the main and original grids.
Grid Display Helper

void writeGrid(std::ostream& out, int grid[9][9], const std::string& title) {
Prints the grid to a file or console.

    if (!title.empty())
        out << title << "\n";
Prints a title like "PUZZLE" or "SOLUTION".

    out << "+-------+-------+-------+\n";
    for (int i = 0; i < 9; ++i) {
        out << "| ";
Draws the top border and starts each row.

        for (int j = 0; j < 9; ++j) {
            char ch = (grid[i][j] == 0) ? '.' : '0' + grid[i][j];
            out << ch << " ";
            if ((j + 1) % 3 == 0) out << "| ";
        }
        out << "\n";
        if ((i + 1) % 3 == 0)
            out << "+-------+-------+-------+\n";
    }
}
Prints rows with grid lines. Empty cells show as ..

Save to File

void saveToFile(const std::string& filename, const std::string& heading, bool includeOriginal = false) {
Writes the puzzle or solution to a text file.

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error writing to file: " << filename << "\n";
        return;
    }
Checks for file write errors.

    if (includeOriginal) {
        writeGrid(file, original_grid, "PUZZLE");
        file << "\n";
    }
    writeGrid(file, grid, heading);
    file.close();
}
Optionally includes original puzzle, then the solution.
Console Output

void printGrid() {
    std::cout << "\nSolved Sudoku:\n";
    writeGrid(std::cout, grid, "");
}
Displays the solved puzzle in the console.

Validation

bool isValid(int row, int col, int num) {
Checks if placing num at (row, col) is allowed.

    for (int i = 0; i < 9; ++i) {
        if (grid[row][i] == num || grid[i][col] == num)
            return false;
    }
Checks the row and column.

    int startRow = 3 * (row / 3), startCol = 3 * (col / 3);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (grid[startRow + i][startCol + j] == num)
                return false;
    return true;
}
Checks the 3×3 subgrid. Returns true if valid.

Check Puzzle Validity

bool isValidPuzzle() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int num = grid[row][col];
            if (num != 0) {
                grid[row][col] = 0;
                if (!isValid(row, col, num)) {
                    grid[row][col] = num;
                    return false;
                }
                grid[row][col] = num;
            }
        }
    }
    return true;
}
Temporarily clears each number and checks if placing it back is valid. Ensures no duplicate numbers already exist.

Check If Already Solved

bool isAlreadySolved() {
    if (!isValidPuzzle())
        return false;

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (grid[i][j] == 0)
                return false;

    return true;
}
Returns true if all cells are filled and the puzzle is valid.

Backtracking Solver
bool solve() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; ++num) {
                    if (isValid(row, col, num)) {
                        grid[row][col] = num;
                        if (solve())
                            return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
Recursive function that tries all numbers in empty cells using backtracking.

Main Function

int main() {
    SudokuSolver solver;
    std::cout << "*** SUDOKU SOLVER ***\n";
Starts program and creates solver object.

    solver.inputFromUser();
    solver.saveToFile("sudoku_input.txt", "PUZZLE");
Collects puzzle input and saves it.

    std::cout << "\nValidating Puzzle\n";
    if (!solver.isValidPuzzle()) {
        std::cout << "The input puzzle is invalid. Please correct it.\n";
        return 1;
    }
Validates puzzle before solving.

    std::cout << "\nSolving the puzzle...\n";
    auto start = std::chrono::high_resolution_clock::now();
    bool solved = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    double timeTaken = std::chrono::duration<double>(end - start).count();
Times how long solving takes.

    if (solved) {
        solver.printGrid();
        std::string filename;
        std::cout << "Enter output filename (with .txt extension): ";
        std::getline(std::cin, filename);

        if (filename.empty() || filename.find_first_not_of(" \t") == std::string::npos)
            filename = "sudoku_output.txt";
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt")
            filename += ".txt";

        solver.saveToFile(filename, "SOLUTION OF PUZZLE", true);
        std::cout << "\nSolved in " << timeTaken << " seconds.\n";
        std::cout << "Puzzle and solution saved to '" << filename << "'\n";
    } else {
        std::cout << "No solution exists for the given puzzle.\n";
    }

    return 0;
}
Handles output: prints and saves results, reports time taken, or tells the user if the puzzle has no solution.