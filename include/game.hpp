#ifndef GAME_H
#define GAME_H

#include "chessboard.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "constants.hpp"
#include <vector>
#include <iostream>
using namespace std;

class Game {
private:
    /** @brief indicate the game state 
     * (running, quit, egality, white_win, black_win) */
    Game_state game_state = running;

    /** @brief chessboard */
    Chessboard *chessboard;

    /** @brief current player color */
    Color player = White;

    /** @brief turn counter */
    int turn_count = 1;

    /** @brief indicate if the current player is in check */
    bool check = false;

    /** @brief indicate if white player can castle */
    bool white_castling = true;

    /** @brief indicate if black player can castle */
    bool black_castling = true;
public:
    /** @brief constructor */
    Game();

    /** @brief destructor */
    ~Game();

    /** @brief return the color of the current player (attribute) */
    Color get_player() const;
    
    /** @brief return the color of the ennemy player */
    Color get_ennemy_player() const;

    /** @brief return the turn number (attribute) */
    int get_turn_count() const;


    /** @brief return the game state (attribute)
     * (running, quit, egality, white_win, black_win) */
    Game_state get_game_state() const;

    /** @brief return a matrix (dynamically allocated) which represent if a tile
     * is defended by the player in argument */
    bool **get_player_defense(Color player) const;

    /** @brief return if the current player is in check (attribute) */
    bool get_check() const;
    
    /** @brief display the chessboard and all the infos about the current
     * state of the game : current player, number of the turn, is_in_check
     * + msg (argument) which "CHECKMAE" or "STALEMATE" */
    void display(string msg = "") const;

    /** @brief update the attribute en_passant_x and en_passant_y potential 
     * pawn nearby the arrival position of a pawn which advanced of 2 tiles 
     * en_passant_x and en_passant_y are the coordinates of the tile that the
     * pawn has crossed */
    void update_en_passant(const Square& pos1, const Square& pos2);

    /** @brief handle the whole turn */
    void turn(vector<string> legal_moves);

    /** @brief move the piece from pos1 to pos2 
     * warning : the move must be legal (no security test in this function) */
    void move(const Square& pos1, const Square& pos2);

    /** @brief move the piece from pos1 to pos2 and kill the pawn which is taken
     * by this en passant
     * warning : the move must be legal (no security test in this function) */
    void move_en_passant(const Square& pos1, const Square& pos2);

    /** @brief update check attribute for the current player */
    void set_check_state();

    /** @brief switch the current player (attribute) */
    void switch_player();

    /** @brief increase the turn counter attribute (full turn) */
    void next_turn();

    /** @brief return true if the tile (x,y) is defended by the player
     * (given in arguments) */
    bool is_defended_position(Color player, int x, int y) const;

    /** @brief return true is the current player is in chess */
    bool is_check() const;

    /** @brief return a bool indicating if castling along king side is legal */
    bool is_legal_castling_king_side() const;

    /** @brief return a bool indicating if castling along queen side is legal */
    bool is_legal_castling_queen_side() const;

    /** @brief return the value of the attribute white_castling or 
     * black_castling for the current player */
    bool is_castling_available() const;

    /** @brief return all legal moves of the current player */
    vector <string> get_all_legal_moves() const;

    
    /** @brief test if the move from pos1 to pos2 is not puting the player king
     in danger */
    bool test_move (Color **color_array, 
                    const Square& pos1,
                    const Square& pos2) const;
};

/** @brief clear screen so that chessboard is always at the top of the window */
void ClearScreen (void);

/** @brief test with regex if the input is a castling along queen side */
bool correct_input_castling_queen_side(string const & command);

/** @brief test with regex if the input is a castling along king side */
bool correct_input_castling_king_side(string const & command);

#endif 