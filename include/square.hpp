#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include "constants.hpp"

using namespace std;

class Square
{
private:
    /** @brief coordinate about x axis */
    int pos_x;

    /** @brief coordinate about y axis */
    int pos_y;

public:
    /** @brief creates a Square from is name 
     * (example : a1 -> pos_x = 0 pos_y = 0) */
    Square(string name);

    /** @brief creates a Square by giving directly the coordinates */
    Square(int pos_x, int pos_y);

    /** @brief return pos_x attribute */
    int get_x() const;

    /** @brief return pos_y attribute */
    int get_y() const;

    /** @brief flip square coordinate about the x axis */
    void flip_y_axis();

    /** @brief return the associated name of the square (0,0) becomes "a8" */
    string to_string() const;
};

/** @brief return true if [x,y] is in the chessboard (0 <= x,y <= 7) */
bool is_in_chessboard(int x, int y);

#endif
