#include "piece.hpp"
#include "constants.hpp"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

Piece::Piece(Color color, string name, Piece_id id):
    color(color), name(name), id(id) {};

Color
Piece::get_color() const
{
    return color;
}

Color
Piece::get_ennemy_color() const
{
    if (get_color() == White)
        return Black;
    else if (get_color() == Black)
        return White;
    else
        return Empty;
}

Piece_id
Piece::get_id() const
{
    return id;
}

bool
Piece::get_is_first_move() const
{
    return is_first_move;
}

void 
Piece::set_first_move_false()
{
    is_first_move = false;
}

void
Piece::display() const
{
    cout << name;
}

string
Piece::swap_color() const
{
    if (name == "\u2656")//tour noire
        return "\u265C";
    else if (name == "\u265C")//tour blanche
        return "\u2656";
    else if (name == "\u2658")//cavalier noir
        return "\u265E";
    else if (name == "\u265E")//cavalier blanc
        return "\u2658";
    else if (name == "\u2657")//fou noir
        return "\u265D";
    else if (name == "\u265D")//fou blanc
        return "\u2657";
    else if (name == "\u2655")//reine noire
        return "\u265B";
    else if (name == "\u265B")//reine blanche
        return "\u2655";
    else if (name == "\u2654")//roi noir
        return "\u265A";
    else if (name == "\u265A")//roi blanc
        return "\u2654";
    else if (name == "\u2659")//poin noir
        return "\u265f";
    else if (name == "\u265F")//poin blanc
        return "\u2659";
    else
        return "X";
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

Rook::Rook(Color color, string name): Piece(color, name, Rook_id) {};

Knight::Knight(Color color, string name): Piece(color, name, Knight_id) {};

Bishop::Bishop(Color color, string name): Piece(color, name, Bishop_id) {};

Queen::Queen(Color color, string name): Piece(color, name, Queen_id) {};

King::King(Color color, string name): Piece(color, name, King_id) {};

Pawn::Pawn(Color color, string name): Piece(color, name, Pawn_id) {};

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

bool **
Piece::get_accessible_squares_not_pawn(
    const Square& pos1, Color **color_array, int range, 
    int no_direction, int *direction_x, int * direction_y)
{
    int x, y;

    bool **legal_square = new bool *[NO_COLUMNS];
    for (int i = 0; i < NO_COLUMNS; i++) {
        legal_square[i] = new bool[NO_COLUMNS];
        for (int j = 0; j < NO_COLUMNS; j++)
            legal_square[i][j] = false;
    }

    Color player_color = get_color();
    Color ennemy_color = get_ennemy_color();

    // try to extend legal move zone for all allowed directions
    for (int i = 0; i < no_direction; i++) {
        x = direction_x[i];
        y = direction_y[i];
        //while piece in chessboard check if we can extend the accessible zone
        while (is_in_chessboard(pos1.get_x() + x, pos1.get_y() + y)) {
            if (color_array[pos1.get_x() + x][pos1.get_y() + y] == player_color)
                break;
            legal_square[pos1.get_x() + x][pos1.get_y() + y] = true;
            if (color_array[pos1.get_x() + x][pos1.get_y() + y] == ennemy_color)
                break;
            if (range == 1) //range = 1 for king and knight
                break;
            x += direction_x[i];
            y += direction_y[i];
        }
    }
    return legal_square;
}

bool **
Rook::get_accessible_squares(const Square& pos1, Color **color_array)
{
    int range = NO_COLUMNS;
    int no_direction = 4;
    int direction_x[4] = {-1, 1, 0, 0};
    int direction_y[4] = {0, 0, -1, 1};
    
    bool **legal_square = 
        get_accessible_squares_not_pawn(pos1, color_array, range, no_direction, 
            direction_x, direction_y);
    return legal_square;
}

bool **
Knight::get_accessible_squares(const Square& pos1, Color **color_array)
{
    int range = 1;
    int no_direction = 8;
    int direction_x[8] = {1, 1, -1, -1, 2, 2, -2, -2};
    int direction_y[8] = {2, -2, 2, -2, 1, -1, 1, -1};

    bool **legal_square = 
        get_accessible_squares_not_pawn(pos1, color_array, range, no_direction, 
            direction_x, direction_y);
    return legal_square;
}

bool **
Bishop::get_accessible_squares(const Square& pos1, Color **color_array)
{
    int range = NO_COLUMNS;
    int no_direction = 4;
    int direction_x[4] = {-1, 1, 1, -1};
    int direction_y[4] = {-1, -1, 1, 1};

    bool **legal_square = 
        get_accessible_squares_not_pawn(pos1, color_array, range, no_direction, 
            direction_x, direction_y);
    return legal_square;
}

bool **
Queen::get_accessible_squares(const Square& pos1, Color **color_array)
{
    int range = NO_COLUMNS;
    int no_direction = 8;
    int direction_x[8] = {-1, 1, 1, -1, -1, 1, 0, 0};
    int direction_y[8] = {-1, -1, 1, 1, 0, 0, -1, 1};

    bool **legal_square = 
        get_accessible_squares_not_pawn(pos1, color_array, range, no_direction, 
            direction_x, direction_y);
    return legal_square;
}

bool **
King::get_accessible_squares(const Square& pos1, Color **color_array)
{
    int range = 1;
    int no_direction = 8;
    int direction_x[8] = {1, 1, 1, 0, 0, -1, -1, -1};
    int direction_y[8] = {1, -1, 0, 1, -1, 1, -1, 0};

    bool **legal_square = 
        get_accessible_squares_not_pawn(pos1, color_array, range, no_direction, 
            direction_x, direction_y);
    return legal_square;
}

bool **
Pawn::get_accessible_squares(const Square& pos1, Color **color_array)
{
    bool **legal_square = new bool *[NO_COLUMNS];
    for (int i = 0; i < NO_COLUMNS; i++)
    {
        legal_square[i] = new bool[NO_COLUMNS];
        for (int j = 0; j < NO_COLUMNS; j++)
            legal_square[i][j] = false;
    }

    // move direction : White->-1, Black->+1
    int direction = get_movement_direction();
    int ennemy_color = get_ennemy_color();
    //no piece should be in front of the pawn in order to advance
    if (is_in_chessboard(pos1.get_x(), pos1.get_y() + direction))
        if (color_array[pos1.get_x()][pos1.get_y()+direction] == Empty)
        {
            legal_square[pos1.get_x()][pos1.get_y()+direction] = true;
            //if it's the first move then pawn can advance of 2 tiles
            if (get_is_first_move())
                if (is_in_chessboard(pos1.get_x(), pos1.get_y()+2*direction))
                    legal_square[pos1.get_x()][pos1.get_y()+2*direction] = true;
        }
    
    //check if pawn can kill an ennemy
    if (is_in_chessboard(pos1.get_x()+1, pos1.get_y()+direction))
        if (color_array[pos1.get_x()+1][pos1.get_y()+direction] == ennemy_color)
            legal_square[pos1.get_x()+1][pos1.get_y()+direction] = true;
    
    if (is_in_chessboard(pos1.get_x()-1, pos1.get_y()+direction))
        if (color_array[pos1.get_x()-1][pos1.get_y()+direction] == ennemy_color)
            legal_square[pos1.get_x()-1][pos1.get_y()+direction] = true;
    
    //check en passant
    if (get_en_passant_x() != -1)
        legal_square[get_en_passant_x()][get_en_passant_y()] = true;
    
    return legal_square;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

bool **
Piece::get_defended_squares_not_pawn(
    const Square& pos1, Color **color_array, bool ***player_defense, int range, 
    int no_direction, int *direction_x, int * direction_y)
{
    int x, y;
    Color ennemy_color = get_ennemy_color();

    for (int i = 0; i < no_direction; i++) {
        x = direction_x[i];
        y = direction_y[i];
        while (is_in_chessboard(pos1.get_x() + x, pos1.get_y() + y)) {
            (*player_defense)[pos1.get_x() + x][pos1.get_y() + y] = true;
            if (color_array[pos1.get_x() + x][pos1.get_y() + y] == ennemy_color)
                break;
            if (range == 1)
                break;
            x += direction_x[i];
            y += direction_y[i];
        }
    }
    return *player_defense;
}

bool **
Rook::get_defended_squares(const Square& pos1, Color **color_array,
    bool ***player_defense)
{
    int range = NO_COLUMNS;
    int no_direction = 4;
    int direction_x[4] = {-1, 1, 0, 0};
    int direction_y[4] = {0, 0, -1, 1};
    
    return get_defended_squares_not_pawn(pos1, color_array, player_defense, 
        range, no_direction, direction_x, direction_y);
}

bool **
Knight::get_defended_squares(const Square& pos1, Color **color_array,
    bool ***player_defense)
{
    int range = NO_COLUMNS;
    int no_direction = 4;
    int direction_x[8] = {1, 1, -1, -1, 2, 2, -2, -2};
    int direction_y[8] = {2, -2, 2, -2, 1, -1, 1, -1};

    return get_defended_squares_not_pawn(pos1, color_array, player_defense, 
        range, no_direction, direction_x, direction_y);
}

bool **
Bishop::get_defended_squares(const Square& pos1, Color **color_array,
    bool ***player_defense)
{
    int range = NO_COLUMNS;
    int no_direction = 4;
    int direction_x[4] = {-1, 1, 1, -1};
    int direction_y[4] = {-1, -1, 1, 1};

    return get_defended_squares_not_pawn(pos1, color_array, player_defense, 
        range, no_direction, direction_x, direction_y);
}

bool **
Queen::get_defended_squares(const Square& pos1, Color **color_array,
    bool ***player_defense)
{    
    int range = NO_COLUMNS;
    int no_direction = 8;
    int direction_x[8] = {-1, 1, 1, -1, -1, 1, 0, 0};
    int direction_y[8] = {-1, -1, 1, 1, 0, 0, -1, 1};

    return get_defended_squares_not_pawn(pos1, color_array, player_defense, 
        range, no_direction, direction_x, direction_y);
}

bool **
King::get_defended_squares(const Square& pos1, Color **color_array,
    bool ***player_defense)
{
    int range = 1;
    int no_direction = 8;
    int direction_x[8] = {1, 1, 1, 0, 0, -1, -1, -1};
    int direction_y[8] = {1, -1, 0, 1, -1, 1, -1, 0};

    return get_defended_squares_not_pawn(pos1, color_array, player_defense, 
        range, no_direction, direction_x, direction_y);
}

bool **
Pawn::get_defended_squares(const Square& pos1, Color **color_array,
    bool ***player_defense)
{
    (void)color_array;
    // move direction : White->-1, Black->+1
    int direction = get_movement_direction();
    // pawn is defending 2 tiles in diagonale
    if (is_in_chessboard(pos1.get_x()+1, pos1.get_y()+direction))
        (*player_defense)[pos1.get_x()+1][pos1.get_y()+direction] = true;
    if (is_in_chessboard(pos1.get_x()-1, pos1.get_y()+direction))
        (*player_defense)[pos1.get_x()-1][pos1.get_y()+direction] = true;
    return *player_defense;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void
Pawn::enable_en_passant(int x, int y)
{
    en_passant_x = x;
    en_passant_y = y;
}

void
Pawn::disable_en_passant()
{
    en_passant_x = -1;
    en_passant_y = -1;
}

int
Pawn::get_en_passant_x() const
{
    return en_passant_x;
}

int
Pawn::get_en_passant_y() const
{
    return en_passant_y;
}

int
Pawn::get_movement_direction() const
{
    return (get_color() == Black) ? 1 : -1;
}
