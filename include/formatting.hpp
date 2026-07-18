#include <string>
#include <iostream>

#define RED "\33[31m"
#define GREEN "\33[32m"
#define YELLOW "\33[33m"
#define BLUE "\33[34m"
#define MAGENTA "\33[35m"
#define CYAN "\33[36m"
#define WHITE "\33[37m"

#define RESET "\33[0m"

void error_message(std::string msg)
{
    std::cout << RED << msg << RESET << "\n";
}

void warning_message(std::string msg)
{
    std::cout << YELLOW << msg << RESET << "\n";
}

void info_message(std::string msg)
{
    std::cout << WHITE << msg << RESET << "\n";
}