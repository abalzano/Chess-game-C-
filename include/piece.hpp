#ifndef PIECE_H
#define PIECE_H

#include "square.hpp"

#include <iostream>
using namespace std;

class Piece
{
private:
    /** @brief Piece color (bLack or White) */
    Color color;

    /** @brief Piece name (utf-8 caracter) */
    string name;

    /** @brief Piece id (see constants.hpp for all id possible) */
    Piece_id id;

    /** @brief set to true, turn to false when the piece moves for 
     * the first time */
    bool is_first_move = true;

public:
    /** @brief constructor */
    Piece(Color color, string name, Piece_id id);

    /** @brief destructor */
    virtual ~Piece() {};

    /** @brief return the color of the piece */
    Color get_color() const;

    /** @brief return the ennemy color of the piece */
    Color get_ennemy_color() const;

    /** @brief return the value of the attribute is_first_move */
    bool get_is_first_move() const;

    /** @brief set the attribute is_first_move to false */
    void set_first_move_false();

    /** @brief return the coordinate of the piece */
    int *get_pos() const;

    /** @brief return the attribute id of the piece */
    Piece_id get_id() const;

    /** @brief print the piece name */
    void display() const;

    /** @brief return all accessible tiles of the piece */
    virtual bool ** get_accessible_squares (const Square& pos1,
        Color **color_array) = 0;

    /** @brief return all defended tiles of the piece 
     * (a tile can be defended but not accesible) */
    virtual bool ** get_defended_squares (const Square& pos1,
        Color **color_array, bool ***player_defense) = 0;

    /** @brief return the name of the piece of the opposite color */
    string swap_color() const;

    /** @brief set en_passant_x and en_passant_y attribute of pawn to x and y 
     * only implemanted in Pawn class */
    virtual void enable_en_passant(int x, int y) = 0;

    /** @brief set en_passant_x and en_passant_y attribute of pawn to -1 and -1 
     * only implemanted in Pawn class */
    virtual void disable_en_passant() = 0;

    /** @brief return the value of the attribute en_passant_x, -1 in
     * not pawn class or if en passant in disable for this pawn */
    virtual int get_en_passant_x() const = 0;

    /** @brief return the value of the attribute en_passant_y, -1 in
     * not pawn class or if en passant in disable for this pawn */
    virtual int get_en_passant_y() const = 0;

    /** @brief return all accessible tiles of the piece 
     * used by all piece expect pawn */
    bool ** get_accessible_squares_not_pawn(const Square& pos1, 
        Color **color_array, int range, int no_direction, int *direction_x,
        int * direction_y);

    /** @brief return all defended tiles of the piece 
     * used by all piece expect pawn */
    bool ** get_defended_squares_not_pawn(
        const Square& pos1, Color **color_array, bool ***player_defense, 
        int range, int no_direction, int *direction_x, int * direction_y);
};

class Rook : public Piece
{
public:
    Rook(Color color, string name);
    ~Rook() {};
    bool ** get_accessible_squares (const Square& pos1, Color **color_array);
    bool ** get_defended_squares (const Square& pos1, Color **color_array,
        bool ***player_defense);
    void enable_en_passant(int x, int y) {(void)x; (void)y;};
    void disable_en_passant() {};
    int get_en_passant_x() const {return -1;};
    int get_en_passant_y() const {return -1;};
};

class Knight : public Piece
{
public:
    Knight(Color color, string name);
    ~Knight() {};
    bool ** get_accessible_squares (const Square& pos1, Color **color_array);
    bool ** get_defended_squares (const Square& pos1, Color **color_array,
        bool ***player_defense);
    void enable_en_passant(int x, int y) {(void)x; (void)y;};
    void disable_en_passant() {};
    int get_en_passant_x() const {return -1;};
    int get_en_passant_y() const {return -1;};
};

class Bishop : public Piece
{
public:
    Bishop(Color color, string name);
    ~Bishop() {};
    bool ** get_accessible_squares (const Square& pos1, Color **color_array);
    bool ** get_defended_squares (const Square& pos1, Color **color_array,
        bool ***player_defense);
    void enable_en_passant(int x, int y) {(void)x; (void)y;};
    void disable_en_passant() {};
    int get_en_passant_x() const {return -1;};
    int get_en_passant_y() const {return -1;};
};

class Queen : public Piece
{
public:
    Queen(Color color, string name);
    ~Queen() {};
    bool ** get_accessible_squares (const Square& pos1, Color **color_array);
    bool ** get_defended_squares (const Square& pos1, Color **color_array,
        bool ***player_defense);
    void enable_en_passant(int x, int y) {(void)x; (void)y;};
    void disable_en_passant() {};
    int get_en_passant_x() const {return -1;};
    int get_en_passant_y() const {return -1;};
};

class King : public Piece
{
public:
    King(Color color, string name);
    ~King() {};
    bool ** get_accessible_squares (const Square& pos1, Color **color_array);
    bool ** get_defended_squares (const Square& pos1, Color **color_array,
        bool ***player_defense);
    void enable_en_passant(int x, int y) {(void)x; (void)y;};
    void disable_en_passant() {};
    int get_en_passant_x() const {return -1;};
    int get_en_passant_y() const {return -1;};
};

class Pawn : public Piece
{
private:
    /** @brief set to -1 if en_passant is disabled and to the x coordinate of 
     * the pawn that can be kill with en_passant */
    int en_passant_x = -1;
    
    /** @brief set to -1 if en_passant is disabled and to the y coordinate of 
     * the pawn that can be kill with en_passant */
    int en_passant_y = -1;
public:
    Pawn(Color color, string name);
    ~Pawn() {};
    bool ** get_accessible_squares (const Square& pos1, Color **color_array);
    bool ** get_defended_squares (const Square& pos1, Color **color_array,
        bool ***player_defense);
    void enable_en_passant(int x, int y);
    void disable_en_passant();
    int get_en_passant_x() const;
    int get_en_passant_y() const;

    /** @brief return move direction : White->-1, Black->+1 */
    int get_movement_direction() const;
};

#endif
