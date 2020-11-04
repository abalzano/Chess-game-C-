#include <iostream>
#include <vector>
#include <string>
#include "constants.hpp"
#include "square.hpp"
#include "piece.hpp"
#include "chessboard.hpp"
#include "game.hpp"
#include <unistd.h>
using namespace std;

int main()
{
    Game myGame;
    Game_state stop = running;
    do {
        vector<string> legal_moves = myGame.get_all_legal_moves();
        myGame.set_check_state();
        if (legal_moves.size() == 0) {
            if (myGame.get_check()) {
                if (myGame.get_player() == White)
                    stop = black_win;
                else
                    stop = white_win;
                myGame.display("CHECKMATE");
            }
            else { // if not in check but no moves are possible then stalemate
                stop = egality;
                myGame.display("STALEMATE");
            }
            break;
        }
        myGame.turn(legal_moves);
        if (myGame.get_game_state() != running) {
            stop = myGame.get_game_state();
            break;
        }
        myGame.next_turn();
        myGame.switch_player();
    } while (stop == running);
    
    switch (stop) {
    case white_win:
        cout << "white win" << endl;
        break;
    case black_win:
        cout << "Black win" << endl;
        break;
    case egality:
        cout << "Egality" << endl;
        break;
    default:
        break;
    }
    cout << endl;
    return 0;
}