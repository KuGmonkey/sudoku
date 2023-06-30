#include"shudu.h"
#include <random>
#include<chrono>
#include<fstream>
#include <sstream>
#include<iostream>
bool Sudoku::isValid(const vector<vector<int>>& puzzle, int row, int col, int num) {
	//check row
	for (int c = 0; c < 9; c++)
	{
		if (puzzle[row][c] == num)
			return false;
	}

	//check col
	for (int r = 0; r < 9; r++) {
		if (puzzle[r][col] == num)
			return false;
	}

	//check subgrid
	int subgridStartRow = row / 3 * 3;
	int subgridStartCol = col / 3 * 3;
	for (int r = subgridStartRow; r < subgridStartRow + 3; r++)
	{
		for (int c = subgridStartCol; c < subgridStartCol + 3; c++)
		{
			if (puzzle[r][c] == num)
				return false;
		}
	}
	return true;
}

bool Sudoku::FindUnassignedPos(const vector<vector<int>>& puzzle, int& row, int& col) {
	for (row = 0; row < GRID_SIZE; row++) {
		for (col = 0; col < GRID_SIZE; col++) {
			if (puzzle[row][col] == 0) {
				// 找到未填充的位置
				return true;
			}
		}
	}
	// 所有位置均已填充
	return false;
}
void Sudoku::solveSudoku(vector<vector<int>>& puzzle) {
	int row, col;
	if (!FindUnassignedPos(puzzle, row, col)) {
		// 如果没有未填充的位置，数独已解决
		// 将当前解加入解集
		solutions.push_back(puzzle);
		return;
	}
	//获取可能的数字
	vector<int> possibleVals = PossibleValAtPos(puzzle, row, col);

	// 创建随机数引擎
	std::random_device rd;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());

	// 打乱vector顺序
	std::shuffle(possibleVals.begin(), possibleVals.end(), gen);

	for (int num : possibleVals) {
		// 可以放置数字
		puzzle[row][col] = num;

		// 递归调用solveSudoku()，尝试填充下一个位置
		solveSudoku(puzzle);
		if (solutions.size() > 0)
			return;
		// 如果要找到所有解，可以注释掉下面这行代码
		puzzle[row][col] = 0;
	}
}

bool Sudoku::unique()
{
	solutions.clear();
	vector<vector<int>> backup(grid);
	solveSudoku(backup);
	return solutions.size() == 1;
}

vector<int> Sudoku::PossibleValAtPos(const vector<vector<int>>& puzzle, int row, int col) {
	//检查给定位置是否已经填充
	if (grid[row][col] != 0) {
		//位置已经填充，返回空向量
		return vector<int>();
	}
	vector<int> possibleVals;

	//检查数字1到9是否可以放置在给定位置
	for (int num = 1; num <= 9; num++) {
		if (isValid(puzzle, row, col, num)) {
			possibleVals.push_back(num);
		}
	}
	return possibleVals;
}

void Sudoku::solve() {
	solutions.clear();
	solveSudoku(grid);
}
//生成n个数独终局并存储在sudoku.txt
void Sudoku::generateEndings(int count) {

	vector<vector<vector<int>>> endings;
	while (count--) {
		vector<vector<int>> puzzle(GRID_SIZE, vector<int>(GRID_SIZE, 0));
		solveSudoku(puzzle);
		endings.insert(endings.end(), solutions.begin(), solutions.end());
		solutions.clear();
	}
	writeSolutionsToFile(endings);
	solutions.clear();
}
void Sudoku::generate() {
	// 生成完整的数独终局
	solve();

	// 挖空指定数量的格子
	switch (difficulty) {
	case 1:
		range = { 20,30 };
		break;
	case 2:
		range = { 31,40 };
		break;
	case 3:
		range = { 41,55 };
		break;
	default:
		range = { 55,60 };
		break;
	}

	// 随机选取挖空的数目
	int holes = rand() % (range.second - range.first + 1) + range.first;

	while (holes > 0) {
		// 选择行和列
		int row = rand() % 9;
		int col = rand() % 9;

		if (grid[row][col] != 0) {
			// 暂存当前数字
			int temp = grid[row][col];
			grid[row][col] = 0;


			// 检查数独终局是否仍然具有唯一解
			bool hasUnique = unique();
			if (!uniqueSolution)
				hasUnique = true;
			if (uniqueSolution && !hasUnique) {
				// 如果要求唯一解且当前数独终局无唯一解，则恢复格子的原始数字
				grid[row][col] = temp;
			}
			else {
				// 数独终局仍然具有唯一解或不要求唯一解，继续挖空
				holes--;
			}
		}
	}
}
void Sudoku::writeGameToFile(const std::string& filename) {
	std::ofstream file(filename, std::ios::app);
	if (!file) {
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}


	for (const auto& row : grid) {
		for (int num : row) {
			if (num == 0) {
				file << "$ ";
			}
			else {
				file << num << " ";
			}
		}
		file << std::endl;
	}
	file << std::endl;
	file.close();
}
void Sudoku::writeSolutionsToFile(vector<vector<vector<int>>>solutions, const std::string& filename) {
	std::ofstream file(filename, std::ios::app);
	if (!file) {
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}

	for (const auto& solution : solutions) {
		for (const auto& row : solution) {
			for (int num : row) {
				file << num << " ";
			}
			file << std::endl;
		}
		file << std::endl;
	}

	file.close();
}
void Sudoku::writeSolutionsToFile(const std::string& filename)
{
	writeSolutionsToFile(solutions, filename);
}
// 辅助函数：读取并求解数独游戏文件
void Sudoku::readGamesFromFile(vector<vector<vector<int>>>& ans, const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	std::string line;
	std::vector<std::vector<int>> game;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;  // 跳过空行
		}

		std::vector<int> gameRow;
		std::istringstream iss(line);
		std::string token;
		while (iss >> token) {
			if (token == "$") {
				gameRow.push_back(0);  // 待填写的数字用0表示
			}
			else {
				try {
					int num = std::stoi(token);
					if (num >= 1 && num <= 9) {
						gameRow.push_back(num);  // 将数字添加到游戏行中
					}
					else {
						std::cerr << "Invalid number in game: " << num << std::endl;
						return;
					}
				}
				catch (const std::exception& e) {
					std::cerr << "Invalid token in game: " << token << std::endl;
					return;
				}
			}
		}

		if (gameRow.size() == GRID_SIZE) {
			game.push_back(gameRow);
		}
		else {
			std::cerr << "Invalid game size: " << gameRow.size() << std::endl;
			return;
		}

		if (game.size() == GRID_SIZE) {
			solveSudoku(game);  // 求解当前数独游戏
			ans.push_back(solutions[0]);
			this->solutions.clear();
			game.clear();  // 清空当前数独游戏
		}
	}
	file.close();
}