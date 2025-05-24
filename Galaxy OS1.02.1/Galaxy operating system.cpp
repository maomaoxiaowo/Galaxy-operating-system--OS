#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

void printHelpInfo() {
    std::cout << "��������:\n";
    std::cout << "  echo [��Ϣ] - ��ʾ��Ϣ\n";
    std::cout << "  help        - ��ʾ�˰�����Ϣ\n";
    std::cout << "  exit        - �˳� shell\n";
    std::cout << "  ls          - �г���ǰĿ¼�µ��ļ�����Ŀ¼\n";
    std::cout << "  cd [·��]   - ���ĵ�ǰ����Ŀ¼\n";
    std::cout << "  cat [�ļ�]  - ��ʾ�ļ�����\n";
    std::cout << "  touch [�ļ�]- �������ļ�\n";
    std::cout << "  rm [�ļ�]   - ɾ���ļ�\n";
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
            perror("�޷���Ŀ¼");
        }
    } else if (command == "cd") {
        if (parameters.empty()) {
            std::cerr << "����: ȱ��·������\n";
        } else {
            if (chdir(parameters[0].c_str()) != 0) {
                perror("�޷�����Ŀ¼");
            }
        }
    } else if (command == "cat") {
        if (parameters.empty()) {
            std::cerr << "����: ȱ���ļ�����\n";
        } else {
            std::ifstream file(parameters[0].c_str());
            if (!file.is_open()) {
                std::cerr << "�޷����ļ�: " << parameters[0] << "\n";
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
            std::cerr << "����: ȱ���ļ�����\n";
        } else {
            std::ofstream file(parameters[0].c_str());
            if (!file.is_open()) {
                std::cerr << "�޷������ļ�: " << parameters[0] << "\n";
            } else {
                file.close();
                std::cout << "�ļ��Ѵ���: " << parameters[0] << "\n";
            }
        }
    } else if (command == "rm") {
        if (parameters.empty()) {
            std::cerr << "����: ȱ���ļ�����\n";
        } else {
            if (remove(parameters[0].c_str()) != 0) {
                std::cerr << "�޷�ɾ���ļ�: " << parameters[0] << "\n";
            } else {
                std::cout << "�ļ���ɾ��: " << parameters[0] << "\n";
            }
        }
    } else {
        std::cout << "δ֪����: " << command << "\n";
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
        perror("�޷���ȡ��ǰĿ¼");
        return "";
    }
}

int main() {
    std::string userInput;
    std::cout << "  ��ӭʹ��Galaxy operating system\n";

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



