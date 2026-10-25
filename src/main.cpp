#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>

std::string getCommandPath(const std::string command) {
  std:: string pathvar = std::getenv("PATH"); 
  std:: istringstream pathStream(pathvar);
  std:: string pathSplit;

  while (std::getline(pathStream, pathSplit, ':')) { //this whole while loop is somthing that I need to understand better, it is splitting the path variable into different paths and checking if the command exists in any of those paths
    // Check if the command exists in this path
    std::string filePath = pathSplit + "/" + command;
    if (access(filePath.c_str(), X_OK) == 0) {
      return filePath; // Command found, return the path
    }
  }
  return ""; // Command not found, return an empty string
}

void typeCommand(const std::string& command) {
  if (command == "echo"|| command == "exit" || command == "type") {
    std::cout << command << " is a shell builtin" << std::endl;
  }
  
  else {
    if (getCommandPath(command) != "") {
      std::cout << command << " is " << getCommandPath(command) << std::endl;
    } else {
      std::cout << command << ": not found" << std::endl;
    }
  }
}


int execute(const std::string& input) {
  std::string command = input.substr(0, input.find(' ')); // Extract the command from the input


  // std::string pathvar = std::getenv("PATH");
  // std::istringstream pathStream(pathvar);
  // std::string pathSplit;

  // while (std::getline(pathStream, pathSplit, ':')) {
  //   // Check if the command exists in this path
  //   std::string filePath = pathSplit + "/" + command;
  //   if (access(filePath.c_str(), X_OK) == 0) {
  //     return; // Command found, exit the function
  //   }
  // }

  if(getCommandPath(command) != "") {
    return std::system(input.c_str());
  }
  else {
    std::cout << input << ": command not found" << std::endl;
    return 1;
  }

}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  std::string input;
  while (true) {
    std::cout << "$ ";

    std::getline(std::cin, input);

    if (input == "exit") {
      break;
    }
    // std::vector<std::string> args;
    else if (input.substr(0, 5) == "echo ") {
      std::string message = input.substr(5);
      std::cout << message << std::endl;
    }
    
    else if(input.substr(0,5) == "type "){
      typeCommand(input.substr(5));
    }
    
    else {
      execute(input);
    }
  }
}
