#pragma once
#ifndef __SUDOKU_H__
#define __SUDOKU_H__
#include<vector>
#include<string>
using std::pair;
using std::vector;
const int GRID_SIZE = 9;
const int SUBGRID_SIZE = 3;

class Sudoku {
private:
	vector<vector<int>> grid;
	vector<vector<vector<int>>> solutions;

	int difficulty = 1;
	pair<int, int> range = { 10, 20 };
	bool uniqueSolution = false;


	//check num at pos(row,int) valid or invalid
	bool isValid(const vector<vector<int>>& puzzle, int row, int col, int num);
	//
	bool FindUnassignedPos(const vector<vector<int>>& puzzle, int& row, int& col);
	//返回pos(row,col)的可能数字
	vector<int> PossibleValAtPos(const vector<vector<int>>& puzzle, int row, int col);
	void solveSudoku(vector<vector<int>>& puzzle);
	bool unique();
public:
	//构造函数
	Sudoku() :grid(GRID_SIZE, vector<int>(GRID_SIZE, 0)) {};

	void setLevel(int difficulty) { difficulty = difficulty; }
	void setRange(pair<int, int>range) { range = range; }
	void setUnique(bool unique) { uniqueSolution = unique; }
	void reset()
	{
		difficulty = 1;
		range = { 10, 20 };
		uniqueSolution = false;
		grid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
		solutions.clear();
	}
	void clearGrid() {
		grid = vector<vector<int>>(GRID_SIZE, vector<int>(GRID_SIZE, 0));
	};
	//solve
	void solve();
	void generateEndings(int num);
	void generate();
	void writeGameToFile(const std::string& filename = "game.txt");
	void readGamesFromFile(vector<vector<vector<int>>>& ans, const std::string& filename);

	void writeSolutionsToFile(const std::string& filename = "sudoku.txt");
	void writeSolutionsToFile(vector<vector<vector<int>>>solutions, const std::string& filename = "sudoku_ending.txt");
	void printGrid(vector<vector<int>>puzzle) {
		for (int i = 0; i < GRID_SIZE; i++)
		{
			for (int j = 0; j < GRID_SIZE; j++)
			{
				printf("%d ", puzzle[i][j]);
			}
			printf("\n");
		}
	}
};

#endif

