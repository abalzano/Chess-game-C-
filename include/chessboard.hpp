#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "piece.hpp"
#include "square.hpp"
#include "constants.hpp"

#include <iostream>
using namespace std;

class Chessboard {
private:
    /** @brief array of 2 int representing the coordinates of the white king */
    int *white_king_pos;

    /** @brief array of 2 int representing the coordinates of the black king */
    int *black_king_pos;
    
    /** @brief matrix of pointers to objets of type pieces*/
    Piece *chessboard[NO_COLUMNS][NO_COLUMNS];

    /** @brief array represented dead pieces in the game init at NULL*/
    Piece *dead_pieces[6][NO_COLUMNS*NO_COLUMNS];

public:
    /** @brief constructor : create an objet of type chessboard
     * only one per game is created
     * dynamic allocation of 64 elements in chessboard and positions of kings */
    Chessboard();

    /** @brief destructor
     * free all memory allocated by the constructor */
    ~Chessboard();

    /** @brief return color of the piece in the tile
     * return Empty if tile is empty
     * warning : parameters x and y must be in the chessboard */
    Color get_color(int x, int y) const;

    /** @brief return a matrix reprensenting the color of each tiles */
    Color **get_color_per_square() const;

    /** @brief return a pointer to the piece at the position (x,y) 
     * return NULL if tile is empty
     * warning : parameters x and y must be in the chessboard */
    Piece *get_piece(int x, int y) const;

    /** @brief return the attribute black_king_pos */
    int *get_black_king_pos() const;

    /** @brief return the attribute white_king_pos */
    int *get_white_king_pos() const;

    /** @brief return true if it's the first move of the piece on the tile
     * if (x,y) is out of the chessboard or empty then retur false */
    bool get_is_first_move(int x, int y) const;

    /** @brief return the id of the piece on tile (x,y)
     * return Empty_id if tile is empty
     * warning : parameters x and y must be in the chessboard */
    Piece_id get_id(int x, int y) const;

    /** @brief display the chessboard */
    void display() const;

    /** @brief display dead pices */
    void display_dead_pieces() const;

    /** @brief move a piece from pos1 to pos2
     * if a piece is taken then it is add to the dead pieces
     * return the state of the game because king can be killed in this function
     * warning : this function is not testing if the movement is legal */
    Game_state move(const Square& pos1, const Square& pos2);    
    
    /** @brief move a piece from pos1 to pos2 and kill the associated pawn 
     * warning : this function is not testing if the movement is legal
     */
    Game_state move_en_passant(const Square& pos1, const Square& pos2);

    /** @brief move the king and the tower following the casting rule
     * along the king side
     * always return running as game state
     * warning : this function is not testing if the movement is legal */
    Game_state move_castling_king_side(Color player);

    /** @brief move the king and the tower following the casting rule
     * along the queen side
     * always return running as game state
     * warning : this function is not testing if the movement is legal */
    Game_state move_castling_queen_side(Color player);
    
    /** @brief update the player's king position if pos1 to pos2 is a move 
     * of the current player king */
    void update_king_position(const Square& pos1, const Square& pos2);
};

#endif 