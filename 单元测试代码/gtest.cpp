#include "pch.h"

class SudokuTest : public ::testing::Test {
protected:
    Sudoku sudoku;

};

TEST_F(SudokuTest, SetLevel) {
    sudoku.setLevel(2);
    EXPECT_EQ(2, sudoku.getLevel());
}

TEST_F(SudokuTest, SetRange) {
    sudoku.setRange({ 5, 10 });
    EXPECT_EQ(5, sudoku.getRange().first);
    EXPECT_EQ(10, sudoku.getRange().second);
}

TEST_F(SudokuTest, SetUnique) {
    sudoku.setUnique(true);
    EXPECT_TRUE(sudoku.getUnique());
}

TEST_F(SudokuTest, Reset) {
    sudoku.setLevel(3);
    sudoku.setRange({ 15, 25 });
    sudoku.setUnique(true);

    sudoku.reset();

    EXPECT_EQ(1, sudoku.getLevel());
    EXPECT_EQ(10, sudoku.getRange().first);
    EXPECT_EQ(20, sudoku.getRange().second);
    EXPECT_FALSE(sudoku.getUnique());
}

TEST_F(SudokuTest, ClearGrid) {
    sudoku.clearGrid();

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            EXPECT_EQ(0, sudoku.getGrid()[i][j]);
        }
    }
}

TEST_F(SudokuTest, Solve) {
    sudoku.generate();
    sudoku.solve();

    // Ensure that the solved puzzle is valid
    vector<vector<int>> solvedGrid = sudoku.getGrid();
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            EXPECT_NE(0, solvedGrid[i][j]);
        }
    }
}

TEST_F(SudokuTest, GenerateEndings) {
    
    vector<vector<vector<int>>> endings = sudoku.generateEndings(5);
    EXPECT_EQ(5, endings.size());
}

TEST_F(SudokuTest, WriteGameToFile) {
    sudoku.generate();
    sudoku.writeGameToFile("test_game.txt");

    // Perform assertions to check if the file was written correctly
}

TEST_F(SudokuTest, ReadGamesFromFile) {
    vector<vector<vector<int>>> games;
    sudoku.readGamesFromFile(games, "test_games.txt");

    // Perform assertions to check if the games were read correctly
}

TEST_F(SudokuTest, WriteSolutionsToFile) {
    sudoku.generate();
    sudoku.solve();
    sudoku.writeSolutionsToFile("test_solutions.txt");

    // Perform assertions to check if the file was written correctly
}

TEST_F(SudokuTest, WriteSolutionsToFile_WithSolutions) {
    sudoku.generate();
    sudoku.solve();
    vector<vector<vector<int>>> solutions = sudoku.getSolutions();
    sudoku.writeSolutionsToFile(solutions, "test_solutions.txt");

    // Perform assertions to check if the file was written correctly
}

TEST_F(SudokuTest, PrintGrid) {
    sudoku.clearGrid();

    // Perform assertions to check if the grid is printed correctly
}