#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
using namespace std;

class SudokuSolver {
private:
    int grid[9][9];
    int original_grid[9][9];

public:
    void inputFromUser() {
        std::cout << "Enter Sudoku Puzzle row by row:\n[use 0 for empty cells and space between each value]\n";
        for (int i = 0; i < 9; ++i) {
            std::vector<int> row;
            while (true) {
                std::cout << "Row " << i + 1 << ": ";
                std::string line;
                std::getline(std::cin, line);
                std::istringstream iss(line);
                int num;
                row.clear();
                while (iss >> num) {
                    if (num < 0 || num > 9) {
                        row.clear();
                        break;
                    }
                    row.push_back(num);
                }
                if (row.size() == 9) break;
                std::cout << "Invalid input. Enter exactly 9 numbers between 0 to 9.\n";
            }
            for (int j = 0; j < 9; ++j) {
                grid[i][j] = row[j];
                original_grid[i][j] = row[j];
            }
        }
    }

    void writeGrid(std::ostream& out, int grid[9][9], const std::string& title) {
        if (!title.empty())
            out << title << "\n";
        out << "+-------+-------+-------+\n";
        for (int i = 0; i < 9; ++i) {
            out << "| ";
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

    void saveToFile(const std::string& filename, const std::string& heading, bool includeOriginal = false) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error writing to file: " << filename << "\n";
            return;
        }
        if (includeOriginal) {
            writeGrid(file, original_grid, "PUZZLE");
            file << "\n";
        }
        writeGrid(file, grid, heading);
        file.close();
    }

    void printGrid() {
        std::cout << "\nSolved Sudoku:\n";
        writeGrid(std::cout, grid, "");
    }

    bool isValid(int row, int col, int num) {
        for (int i = 0; i < 9; ++i) {
            if (grid[row][i] == num || grid[i][col] == num)
                return false;
        }
        int startRow = 3 * (row / 3), startCol = 3 * (col / 3);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (grid[startRow + i][startCol + j] == num)
                    return false;
        return true;
    }

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

    bool isAlreadySolved() {
    if (!isValidPuzzle())
        return false;

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (grid[i][j] == 0)
                return false;

    return true;
}

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
};

int main() {
    SudokuSolver solver;
    std::cout << "*** SUDOKU SOLVER ***\n";

    solver.inputFromUser();
    solver.saveToFile("sudoku_input.txt", "PUZZLE");

    std::cout << "\nValidating Puzzle\n";
    if (!solver.isValidPuzzle()) {
        std::cout << "The input puzzle is invalid. Please correct it.\n";
        return 1;
    }

    std::cout << "\nSolving the puzzle...\n";
    auto start = std::chrono::high_resolution_clock::now();
    bool solved = solver.solve();
    auto end = std::chrono::high_resolution_clock::now();
    double timeTaken = std::chrono::duration<double>(end - start).count();

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
