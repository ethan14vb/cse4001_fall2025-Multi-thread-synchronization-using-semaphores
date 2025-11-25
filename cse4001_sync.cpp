#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <vector>

int main(int argc, char* argv[]) 
{
    int choice = std::atoi(argv[1]);

    const char* program = nullptr;

    switch (choice) 
    {
        case 1:
            program = "./reader_writer_no_starve";
            break;
        case 2:
            program = "./writer_priority";
            break;
        case 3:
            program = "./dining_philosophers1";
            break;
        case 4:
            program = "./dining_philosophers2";
            break;
        default:
            return 1;
    }

    char* args[] = { const_cast<char*>(program), nullptr };

    execvp(program, args);

    // If exec fails
    perror("execvp failed");
    return 1;
}
