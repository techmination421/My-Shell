#include <iostream>
#include <string>
#include <vector>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  while (true) {
    std::cout << "$ ";

    std::string input;
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
      if(input.substr(5) == "echo"){
        std::cout << "echo is a shell builtin" << std::endl;
      }
      else if(input.substr(5) == "exit"){
        std::cout << "exit is a shell builtin" << std::endl;
      }
      else if(input.substr(5) == "type"){
        std::cout << "type is a shell builtin" << std::endl;
      }
      else{
        std::cout << input.substr(5) << ": command not found" << std::endl;
      }
    }
    else {
    std::cout << input<< ": command not found" << std::endl;
    }
  }
}
