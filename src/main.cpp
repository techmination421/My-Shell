#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include<filesystem>
#include<fcntl.h>
#include<numeric>

void repl(const std::string& input, std::vector<std::string>& parts);

std::vector<std::string> split(const std::string& str){
  std::vector<std::string> result;
  std::istringstream iss(str);
  std::string token;

  while (std::getline(iss, token, ' ')) {
    result.push_back(token);
  }

  return result;
}

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
  if (command == "echo"|| command == "exit" || command == "type" || command == "pwd" || command == "cd") {
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
void redirectOutput(std::vector<std::string>& parts, int fd_value = 1) {
  std::string filename = parts.back();// Get the last part of the command as the filename where the output will be redirected
  parts.pop_back(); // Remove the filename from the parts vector
  parts.pop_back(); // Remove the redirection operator from the parts vector

  int saved = dup(1); // Save the current stdout file descriptor
  int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  dup2(fd, fd_value); // Redirect stdout to the file descriptor
  close(fd);
  repl(std::accumulate(parts.begin(), parts.end(), std::string(),
    [](const std::string& a, const std::string& b) {
        return a + (a.length() > 0 ? " " : "") + b;
    }),parts);
  dup2(saved, 1); // Restore the original stdout file descriptor
  close(saved);
}

void repl(const std::string& input, std::vector<std::string>& parts) {
  if (input == "exit") {
    std::exit(0);
  }
  // std::vector<std::string> args;
  else if (input.substr(0, 5) == "echo ") {
    std::string message = input.substr(5);
    std::cout << message << std::endl;
  }
  
  else if(input.substr(0,5) == "type "){
    typeCommand(input.substr(5));
  }

  else if (input == "pwd"){
    std::cout<<std::filesystem::current_path().string()<<std::endl;
  }

  else if(parts[0] == "cd"){
    if(parts[1] == "~"){
      parts[1] = std::getenv("HOME");
    }
    if(chdir(parts[1].c_str()) != 0){
      std::cerr << "cd: " << parts[1] << ": No such file or directory" << std::endl;
    }
  }
  
  else {
    execute(input);
  }
}


int main() {
  // Flush after every std::cout / std:cerr
  // std::cout << std::unitbuf;
  // std::cerr << std::unitbuf;

  // TODO: Uncomment the code below to pass the first stage
  std::string input;
  while (true) {
    std::cout << "$ ";

    std::getline(std::cin, input);

    std::vector<std::string> parts = split(input);
    if(parts.size() >2 ){
      if(parts[parts.size() - 2] == ">" || parts[parts.size() - 2] == "1>") {  
        // bool write_into_file = false;
        redirectOutput(parts);
      }
      else if(parts[parts.size() - 2] == "2>") {
        redirectOutput(parts, 2);
      }
      else{
        repl(input, parts);
      }
    }
    


    else{
      repl(input, parts);
    }
    
  }
}
