#include <iostream>
#include"shudu.h"
#include <string>
#include<fstream>
#include<sstream>
#include"getopt.h"

// 定义标志位
bool flagC = false;
bool flagS = false;
bool flagN = false;
bool flagM = false;
bool flagR = false;
bool flagU = false;

bool isNumeric(const std::string& str) {
    return !str.empty() && str.find_first_not_of("0123456789") == std::string::npos;
}

bool isValidCount(int count) {
    return count >= 1 && count <= 1000000;
}

bool isValidFilePath(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

bool isValidDifficulty(int difficulty) {
    return difficulty >= 1 && difficulty <= 3;
}

bool isValidRange(const std::pair<int, int>& range) {
    return range.first >= 20 && range.first <= 55 &&
        range.second >= 20 && range.second <= 55 &&
        range.first <= range.second;
}

void printUsage() {
    std::cout << "Usage: sudoku -c <count> -s <file> -n <count> -m <difficulty> -r <range> -u" << std::endl;
}
int main(int argc, char* argv[]) {
    srand(time(0));
    if (argc < 2) {
        std::cout << "Usage: sudoku.exe <options>" << std::endl;
        std::cout << "-c <count>        Generate <count> unique Sudoku grids" << std::endl;
        std::cout << "-s <file>         Solve Sudoku grids from <file> and output the solutions" << std::endl;
        std::cout << "-n <count>        Generate <count> Sudoku games" << std::endl;
        std::cout << "-m <difficulty>   Set the difficulty level for generated games (1-3) [only when '-n' is used]" << std::endl;
        std::cout << "-r <range>        Set the range of empty cells in generated games (e.g., 20-55) [only when '-n' is used]" << std::endl;
        std::cout << "-u                Generate games with unique solutions [only when '-n' is used]" << std::endl;
        return 1;
    }
    int option;
    int final_count = 0;
    int game_count = 0;
    std::string filename;
    int difficulty = 0;
    std::pair<int, int> range;
    bool uniqueSolution = false;
    char args[] = "c:s:n:m:r:u";
    while ((option = getopt(argc, argv, args)) != -1) {
        switch (option) {
        case 'c':
            if (final_count != 0 || !isNumeric(optarg)) {
                std::cerr << "Invalid command line arguments" << std::endl;
                printUsage();
                return 1;
            }
            final_count = std::stoi(optarg);
            if (!isValidCount(final_count)) {
                std::cerr << "Invalid count value. Must be between 1 and 1000000." << std::endl;
                return 1;
            }
            flagC = true;
            break;
        case 's':
            if (!filename.empty()) {
                std::cerr << "Invalid command line arguments" << std::endl;
                printUsage();
                return 1;
            }
            filename = optarg;
            if (!isValidFilePath(filename)) {
                std::cerr << "Invalid file path: " << filename << std::endl;
                return 1;
            }
            flagS = true;
            break;
        case 'n':
            if (game_count != 0 || !isNumeric(optarg)) {
                std::cerr << "Invalid command line arguments" << std::endl;
                printUsage();
                return 1;
            }
            game_count = std::stoi(optarg);
            if (game_count < 1 || game_count>10000) {
                std::cerr << "Invalid count value. Must be between 1 and 10000." << std::endl;
                return 1;
            }
            flagN = true;
            break;
        case 'm':
            if (difficulty != 0 || !isNumeric(optarg)) {
                std::cerr << "Invalid command line arguments" << std::endl;
                printUsage();
                return 1;
            }
            difficulty = std::stoi(optarg);
            if (!isValidDifficulty(difficulty)) {
                std::cerr << "Invalid difficulty value. Must be between 1 and 3." << std::endl;
                return 1;
            }
            flagM = true;
            break;
        case 'r':
        {
            std::istringstream rangeStream(optarg);
            char delimiter;
            rangeStream >> range.first >> delimiter >> range.second;
            if (!rangeStream || !isValidRange(range)) {
                std::cerr << "Invalid command line arguments" << std::endl;
                printUsage();
                return 1;
            }
        }
        flagR = true;
        break;
        case 'u':
            uniqueSolution = true;
            flagU = true;
            break;
        default:
            std::cerr << "Invalid command line arguments" << std::endl;
            printUsage();
            return 1;
        }
    }

    if ((flagM || flagR || flagU) && !flagN)
    {
        std::cerr << "-m,-r,-u must follows -n" << std::endl;
        return 1;
    }

    Sudoku sudoku;
    //开始执行操作
    if (flagC)
    {
        sudoku.generateEndings(final_count);
    }
    if (flagS)
    {
        vector<vector<vector<int>>> ans;
        sudoku.readGamesFromFile(ans, filename);
        sudoku.writeSolutionsToFile(ans, "sudoku.txt");
    }
    if (flagN)
    {
        if (flagM)
            sudoku.setLevel(difficulty);
        if (flagR)
            sudoku.setRange(range);
        if (flagU)
            sudoku.setUnique(uniqueSolution);
        while (game_count--)
        {
            sudoku.generate();

            sudoku.writeGameToFile();

            sudoku.clearGrid();
        }
    }
    return 0;
}