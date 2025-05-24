#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

void printHelpInfo() {
    std::cout << "可用命令:\n";
    std::cout << "  echo [消息] - 显示消息\n";
    std::cout << "  help        - 显示此帮助信息\n";
    std::cout << "  exit        - 退出 shell\n";
    std::cout << "  ls          - 列出当前目录下的文件和子目录\n";
    std::cout << "  cd [路径]   - 更改当前工作目录\n";
    std::cout << "  cat [文件]  - 显示文件内容\n";
    std::cout << "  touch [文件]- 创建空文件\n";
    std::cout << "  rm [文件]   - 删除文件\n";
}

void executeCommand(const std::string& command, const std::vector<std::string>& parameters) {
    if (command == "echo") {
        for (size_t i = 0; i < parameters.size(); ++i) {
            std::cout << parameters[i];
            if (i != parameters.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    } else if (command == "help") {
        printHelpInfo();
    } else if (command == "exit") {
        exit(0);
    } else if (command == "ls") {
        DIR* dir;
        struct dirent* ent;
        if ((dir = opendir(".")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                std::cout << ent->d_name << "\n";
            }
            closedir(dir);
        } else {
            perror("无法打开目录");
        }
    } else if (command == "cd") {
        if (parameters.empty()) {
            std::cerr << "错误: 缺少路径参数\n";
        } else {
            if (chdir(parameters[0].c_str()) != 0) {
                perror("无法更改目录");
            }
        }
    } else if (command == "cat") {
        if (parameters.empty()) {
            std::cerr << "错误: 缺少文件参数\n";
        } else {
            std::ifstream file(parameters[0].c_str());
            if (!file.is_open()) {
                std::cerr << "无法打开文件: " << parameters[0] << "\n";
            } else {
                std::string line;
                while (std::getline(file, line)) {
                    std::cout << line << "\n";
                }
                file.close();
            }
        }
    } else if (command == "touch") {
        if (parameters.empty()) {
            std::cerr << "错误: 缺少文件参数\n";
        } else {
            std::ofstream file(parameters[0].c_str());
            if (!file.is_open()) {
                std::cerr << "无法创建文件: " << parameters[0] << "\n";
            } else {
                file.close();
                std::cout << "文件已创建: " << parameters[0] << "\n";
            }
        }
    } else if (command == "rm") {
        if (parameters.empty()) {
            std::cerr << "错误: 缺少文件参数\n";
        } else {
            if (remove(parameters[0].c_str()) != 0) {
                std::cerr << "无法删除文件: " << parameters[0] << "\n";
            } else {
                std::cout << "文件已删除: " << parameters[0] << "\n";
            }
        }
    } else {
        std::cout << "未知命令: " << command << "\n";
    }
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream stream(input);
    std::string item;
    while (std::getline(stream, item, delimiter)) {
        result.push_back(item);
    }
    return result;
}

std::string getCurrentDirectory() {
    char buffer[PATH_MAX];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return std::string(buffer);
    } else {
        perror("无法获取当前目录");
        return "";
    }
}

int main() {
    std::string userInput;
    std::cout << "  欢迎使用Galaxy operating system\n";

    while (true) {
        std::string currentDir = getCurrentDirectory();
        std::cout << "[" << currentDir << "] Galaxy> ";
        std::getline(std::cin, userInput);

        std::vector<std::string> tokens = splitString(userInput, ' ');
        if (!tokens.empty()) {
            std::string command = tokens[0];
            std::vector<std::string> parameters(tokens.begin() + 1, tokens.end());
            executeCommand(command, parameters);
        }
    }

    return 0;
}



