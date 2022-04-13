#include <iostream>
#include <exception>
#include "Server.hpp"
#include "ThreadPool.hpp"

#define NUMBER_OF_PORTS 65535
#define HELP "-help"

bool checkCommandLineParameters(int, char*[]);
void printInstructions();

int main(int argc, char* argv[]){
    if(!checkCommandLineParameters(argc, argv)){
        return -1;
    }

    Server* server = nullptr; // init the server object.

    try{
        server = new Server(atoi(argv[1]));
    }
    catch (const std::exception& e){
        std::cout << "Cant run the server..." << std::endl;
        std::cout << e.what() << std::endl;
        return -1;
    }

    server->run();
    std::string userInput = ""; //saves the user input (exit when "quit" is entered)
    while (true)
    {
        getline(std::cin, userInput);
        if(userInput == "quit"){
            server->shutdown();
            break;
        }
    }

    return 0;
}

/*
Function that check if the paramteres from the command line are valid.
Input: argc - the number of parameters, and argv - the command line parameters in char* array.
Output: if the command line parameters are valid.
*/
bool checkCommandLineParameters(int argc, char* argv[]){
    if(argc == 2){
        // getting the flag from the command line (user input)
        std::string flag = std::string(argv[1]);
        if(flag == HELP){
            printInstructions();
            return false;
        }
        else{
            // getting the server port number and check if the port is valid
            int port = atoi(argv[1]);
            // check if the port is valid
            if(port > 0 && port <= NUMBER_OF_PORTS){
                return true;
            }
            else{
                std::cout << "Please enter port value between 1 to " << NUMBER_OF_PORTS << std::endl;
                return false;
            }
        }
    }
    else{
        std::cout << "Error with command lines parameters." << std::endl << "Try to run the server with " << HELP << " parameter to get more info." << std::endl;
        return false;
    }
}


/*
* this functions prints welcome message and instructions for user
* input: none
* output: none
*/
void printInstructions(){
    std::cout << "Welcome to the relay server of the onion routing project" << std::endl;
    std::cout << "To run the relay server, please pass the variables that needed." << std::endl;
    std::cout << "Please pass the variables in the next format: " << std::endl;
    std::cout << "{executable file name} {port}" << std::endl;
}