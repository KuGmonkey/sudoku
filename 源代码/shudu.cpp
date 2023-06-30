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
				// �ҵ�δ����λ��
				return true;
			}
		}
	}
	// ����λ�þ������
	return false;
}
void Sudoku::solveSudoku(vector<vector<int>>& puzzle) {
	int row, col;
	if (!FindUnassignedPos(puzzle, row, col)) {
		// ���û��δ����λ�ã������ѽ��
		// ����ǰ�����⼯
		solutions.push_back(puzzle);
		return;
	}
	//��ȡ���ܵ�����
	vector<int> possibleVals = PossibleValAtPos(puzzle, row, col);

	// �������������
	std::random_device rd;
	std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());

	// ����vector˳��
	std::shuffle(possibleVals.begin(), possibleVals.end(), gen);

	for (int num : possibleVals) {
		// ���Է�������
		puzzle[row][col] = num;

		// �ݹ����solveSudoku()�����������һ��λ��
		solveSudoku(puzzle);
		if (solutions.size() > 0)
			return;
		// ���Ҫ�ҵ����н⣬����ע�͵��������д���
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
	//������λ���Ƿ��Ѿ����
	if (grid[row][col] != 0) {
		//λ���Ѿ���䣬���ؿ�����
		return vector<int>();
	}
	vector<int> possibleVals;

	//�������1��9�Ƿ���Է����ڸ���λ��
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
//����n�������վֲ��洢��sudoku.txt
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
	// ���������������վ�
	solve();

	// �ڿ�ָ�������ĸ���
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

	// ���ѡȡ�ڿյ���Ŀ
	int holes = rand() % (range.second - range.first + 1) + range.first;

	while (holes > 0) {
		// ѡ���к���
		int row = rand() % 9;
		int col = rand() % 9;

		if (grid[row][col] != 0) {
			// �ݴ浱ǰ����
			int temp = grid[row][col];
			grid[row][col] = 0;


			// ��������վ��Ƿ���Ȼ����Ψһ��
			bool hasUnique = unique();
			if (!uniqueSolution)
				hasUnique = true;
			if (uniqueSolution && !hasUnique) {
				// ���Ҫ��Ψһ���ҵ�ǰ�����վ���Ψһ�⣬��ָ����ӵ�ԭʼ����
				grid[row][col] = temp;
			}
			else {
				// �����վ���Ȼ����Ψһ���Ҫ��Ψһ�⣬�����ڿ�
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
// ������������ȡ�����������Ϸ�ļ�
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
			continue;  // ��������
		}

		std::vector<int> gameRow;
		std::istringstream iss(line);
		std::string token;
		while (iss >> token) {
			if (token == "$") {
				gameRow.push_back(0);  // ����д��������0��ʾ
			}
			else {
				try {
					int num = std::stoi(token);
					if (num >= 1 && num <= 9) {
						gameRow.push_back(num);  // ��������ӵ���Ϸ����
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
			solveSudoku(game);  // ��⵱ǰ������Ϸ
			ans.push_back(solutions[0]);
			this->solutions.clear();
			game.clear();  // ��յ�ǰ������Ϸ
		}
	}
	file.close();
}