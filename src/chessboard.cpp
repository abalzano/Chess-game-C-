#include "chessboard.hpp"

Chessboard::Chessboard()
{
    for (int i = 0; i < NO_COLUMNS; i++)
        for (int j = 0; j < NO_COLUMNS; j++) {
            chessboard[i][j] = NULL;
            for (int k = 0; k < 6; k++)
                dead_pieces[k][i*NO_COLUMNS + j] = NULL;
        }
    (chessboard[0][0]) = new Rook  (Black, "\u2656");
    (chessboard[1][0]) = new Knight(Black, "\u2658");
    (chessboard[2][0]) = new Bishop(Black, "\u2657");
    (chessboard[3][0]) = new Queen (Black, "\u2655");
    (chessboard[4][0]) = new King  (Black, "\u2654");
    (chessboard[5][0]) = new Bishop(Black, "\u2657");
    (chessboard[6][0]) = new Knight(Black, "\u2658");
    (chessboard[7][0]) = new Rook  (Black, "\u2656");

    (chessboard[0][7]) = new Rook  (White, "\u265C");
    (chessboard[1][7]) = new Knight(White, "\u265E");
    (chessboard[2][7]) = new Bishop(White, "\u265D");
    (chessboard[3][7]) = new Queen (White, "\u265B");
    (chessboard[4][7]) = new King  (White, "\u265A");
    (chessboard[5][7]) = new Bishop(White, "\u265D");
    (chessboard[6][7]) = new Knight(White, "\u265E");
    (chessboard[7][7]) = new Rook  (White, "\u265C");

    for (int i = 0; i < NO_COLUMNS; i++) {
        (chessboard[i][1]) = new Pawn(Black, "\u2659");
        (chessboard[i][6]) = new Pawn(White, "\u265F");
    }

    black_king_pos = new int [2];
    black_king_pos[0] = 4;
    black_king_pos[1] = 0;
    white_king_pos = new int [2];
    white_king_pos[0] = 4;
    white_king_pos[1] = 7;
}

Chessboard::~Chessboard()
{
    delete[] black_king_pos;
    delete[] white_king_pos;
    for (size_t i = 0; i < NO_COLUMNS; i++)
        for (size_t j = 0; j < NO_COLUMNS; j++) {
            if (chessboard[i][j])
                delete chessboard[i][j];
            for (int k = 0; k < 6; k++)
                if (dead_pieces[k][i*NO_COLUMNS + j])
                    delete dead_pieces[k][i*NO_COLUMNS + j];
        }
}

void
Chessboard::display() const
{
    display_dead_pieces();
    string space = "\u2009\u2009\u2009\u2009";
    cout << "\u2009\u2009 " << "a   b   c   d   e   f   g   h" << endl;
    bool tile_color = false; // for alternating tile colors
    for (int i = 0; i < NO_COLUMNS; i++) {
        cout << NO_COLUMNS - i << " "; // number of lines
        for (int j = 0; j < NO_COLUMNS; j++) {
            if (tile_color)
                cout<<"\033[1;47m\033[1;30m";
            if (chessboard[j][i]) {
                cout << "\u2009"; //U+0020 : utf-8 space
                if (tile_color)
                    cout << chessboard[j][i]->swap_color();
                else
                    chessboard[j][i]->display();
                cout  << "\u2009\u2009";
            }
            else
                cout << space;
            if (tile_color)
                cout<<"\033[1;m";
            tile_color = !tile_color;
        }
        cout << endl;
        tile_color = !tile_color;
    }
}

void
Chessboard::display_dead_pieces() const
{
    for (size_t i = 0; i < 6; i++) {
        int j = 0;
        while (dead_pieces[i][j] != NULL) {
            if (dead_pieces[i][j]->get_color() == White) {
                dead_pieces[i][j]->display();
                cout << " ";
            }
            j++;
        }
    }
    for (size_t i = 0; i < 6; i++) {
        int j = 0;
        while (dead_pieces[i][j] != NULL) {
            if (dead_pieces[i][j]->get_color() == Black) {
                dead_pieces[i][j]->display();
                cout << " ";
            }
            j++;
        }
    }
    cout << endl;
}

Color
Chessboard::get_color(int x, int y) const
{
    return chessboard[x][y] ? chessboard[x][y]->get_color() : Empty;
}

Color **
Chessboard::get_color_per_square() const
{
    Color **color_array = new Color *[NO_COLUMNS];
    for (int i = 0; i < NO_COLUMNS; i++) {
        color_array[i] = new Color[NO_COLUMNS];
        for (int j = 0; j < NO_COLUMNS; j++)
            color_array[i][j] = 
                chessboard[i][j] ? chessboard[i][j]->get_color() : Empty;
    }
    return color_array;
}

Piece *
Chessboard::get_piece(int x, int y) const
{
    return chessboard[x][y] ? chessboard[x][y] : NULL;
}

int *
Chessboard::get_black_king_pos() const
{
    return black_king_pos;
}

int *
Chessboard::get_white_king_pos() const
{
    return white_king_pos;
}


bool 
Chessboard::get_is_first_move(int x, int y) const
{
    return (is_in_chessboard(x, y) && chessboard[x][y]) ?
        chessboard[x][y]->get_is_first_move() : false;
}

Game_state
Chessboard::move(const Square& pos1, const Square& pos2)
{
    if (chessboard[pos2.get_x()][pos2.get_y()]) {        
        int id = chessboard[pos2.get_x()][pos2.get_y()]->get_id();
        int j = 0;
        while (dead_pieces[id][j] != NULL)
            j++;
        dead_pieces[id][j] = chessboard[pos2.get_x()][pos2.get_y()];
        chessboard[pos2.get_x()][pos2.get_y()] = NULL;
    }
    chessboard[pos2.get_x()][pos2.get_y()] = 
        chessboard[pos1.get_x()][pos1.get_y()];
    chessboard[pos1.get_x()][pos1.get_y()] = NULL;

    if (pos2.get_x() == black_king_pos[0] && pos2.get_y() == black_king_pos[1])
        return white_win;
    if (pos2.get_x() == white_king_pos[0] && pos2.get_y() == white_king_pos[1])
        return black_win;
    
    update_king_position(pos1, pos2);
    return running;
}

void
Chessboard::update_king_position(const Square& pos1, const Square& pos2)
{
    if (pos1.get_x() == black_king_pos[0] && pos1.get_y() == black_king_pos[1]){
        black_king_pos[0] = pos2.get_x();
        black_king_pos[1] = pos2.get_y();
    }
    if (pos1.get_x() == white_king_pos[0] && pos1.get_y() == white_king_pos[1]){
        white_king_pos[0] = pos2.get_x();
        white_king_pos[1] = pos2.get_y();
    }
}

Game_state
Chessboard::move_en_passant(const Square& pos1, const Square& pos2)
{
    if (chessboard[pos2.get_x()][pos1.get_y()]) {
        int id = chessboard[pos2.get_x()][pos1.get_y()]->get_id();
        int j = 0;
        while (dead_pieces[id][j] != NULL)
            j++;
        dead_pieces[id][j] = chessboard[pos2.get_x()][pos1.get_y()];
        chessboard[pos2.get_x()][pos1.get_y()] = NULL;
    }
    chessboard[pos2.get_x()][pos2.get_y()] = 
        chessboard[pos1.get_x()][pos1.get_y()];
    chessboard[pos1.get_x()][pos1.get_y()] = NULL;
    return running;
}

Piece_id
Chessboard::get_id(int x, int y) const
{
    return (is_in_chessboard(x, y) && chessboard[x][y]) ?
        chessboard[x][y]->get_id() : Empty_id;    
}

Game_state
Chessboard::move_castling_queen_side(Color player)
{
    int y = ((player == White) ? NO_COLUMNS - 1 : 0);
    chessboard[2][y] = chessboard[4][y];
    chessboard[3][y] = chessboard[0][y];
    chessboard[4][y] = NULL;
    chessboard[0][y] = NULL;
    Square pos1(4, y);
    Square pos2(2, y);
    update_king_position(pos1, pos2);
    return running;
}

Game_state 
Chessboard::move_castling_king_side(Color player)
{
    int y = ((player == White) ? NO_COLUMNS - 1 : 0);
    chessboard[6][y] = chessboard[4][y];
    chessboard[5][y] = chessboard[7][y];
    chessboard[4][y] = NULL;
    chessboard[7][y] = NULL;
    Square pos1(4, y);
    Square pos2(6, y);
    update_king_position(pos1, pos2);
    return running;
}