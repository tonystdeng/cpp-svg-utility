#include "view.hpp"

int main(int argc, char* argv[]){
    if (!(argc==2)){
        std::cout<<"something is wrong with your input, just enter the file name bro its not that hard\n";
        return 1;
    }
    Game game(argv[1]);

    while (game.window->isOpen()){
        game.main();
    }
    return 0;
}