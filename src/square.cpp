#include <iostream>
#include <unistd.h>
#include <term.h>
#include <string>
#include "square.hpp"

Square::Square(string name)
{
    pos_x = (int)(name[0] - 'a');
    pos_y = (int)(name[1] - '0' - 1);
}

Square::Square(int pos_x, int pos_y): pos_x(pos_x), pos_y(pos_y) {}

int
Square::get_x() const
{
    return pos_x;
}

int
Square::get_y() const
{
    return pos_y;
}

void
Square::flip_y_axis()
{
    pos_y = NO_COLUMNS - 1 - pos_y;
}

string
Square::to_string() const
{
    string name("a1");
    name[0] = (char)(pos_x) + 'a';
    name[1] = (char)(pos_y) + '0' + 1;
    return name;
}

bool is_in_chessboard(int x, int y)
{
    return x >= 0 && x < NO_COLUMNS && y >= 0 && y < NO_COLUMNS;
}