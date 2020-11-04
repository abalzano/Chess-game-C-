#include <regex>
#include <unistd.h>
#include <term.h>
#include "game.hpp"

void
ClearScreen (void)
{
    if (!cur_term) {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if (result <= 0)
            return;
    }
    putp(tigetstr("clear"));
}

bool
correct_input_castling_king_side(string const & command)
{
    regex movement_pattern("(O|o|0)-(O|o|0)");
    return regex_match(command,movement_pattern);
}

bool
correct_input_castling_queen_side(string const & command)
{
    regex movement_pattern("(O|o|0)-(O|o|0)-(O|o|0)");
    return regex_match(command,movement_pattern);
}

Game::Game()
{
    chessboard = new Chessboard;
}

Game::~Game()
{
    delete chessboard;
}

Color
Game::get_player() const
{
    return player;
}

Color
Game::get_ennemy_player() const
{
    return (player == Black) ? White : Black; 
}

int
Game::get_turn_count() const
{
    return turn_count;
}

Game_state 
Game::get_game_state() const
{
    return game_state;
}

bool **
Game::get_player_defense(Color player) const
{
    Color **color_array = chessboard->get_color_per_square();
    bool **player_defense = new bool *[NO_COLUMNS];
    for (int i = 0; i < NO_COLUMNS; i++) {
        player_defense[i] = new bool[NO_COLUMNS];
        for (int j = 0; j < NO_COLUMNS; j++)
            player_defense[i][j] = false;
    }
    for (int i = 0; i < NO_COLUMNS; i++) {
        for (int j = 0; j < NO_COLUMNS; j++) {
            if (color_array[i][j] == player) {
                Square pos1 (i, j);
                player_defense = chessboard->get_piece(i,j)
                    ->get_defended_squares (pos1 ,color_array, &player_defense);
            }
        }
    }
    for (size_t i = 0; i < NO_COLUMNS; i++)
        delete[] color_array[i];
    delete[] color_array;
    return player_defense;
}

bool 
Game::get_check() const
{
    return check;
}

bool
Game::is_defended_position(Color player, int x, int y) const
{
    bool **player_defense = get_player_defense(player);
    bool test_tile = player_defense[x][y];
    for (size_t i = 0; i < NO_COLUMNS; i++)
        delete[] player_defense[i];
    delete[] player_defense;
    return test_tile;
}

bool
Game::is_check() const
{
    int *king_pos;
    if (get_player() == Black)
        king_pos = chessboard->get_black_king_pos();
    else
        king_pos = chessboard->get_white_king_pos();
    
    return is_defended_position(get_ennemy_player(), 
        king_pos[0], king_pos[1]);
}

void
Game::display(string msg) const
{
    ClearScreen();
    cout << "Turn : " << get_turn_count() << endl;
    if (msg == "") {
        cout << ((get_player() == White) ? "White's turn" : "Black's turn");
        if (check)
            cout << "\033[1;31m CHECK\033[0m";
        cout << endl;
    }
    else
        cout << "\033[1;31m"<< msg <<"\033[0m" << endl;
    chessboard->display();
}

void
Game::update_en_passant(const Square& pos1, const Square& pos2)
{
    for (int i = 0; i < NO_COLUMNS; i++)
        for (int j = 0; j < NO_COLUMNS; j++)
            if (chessboard->get_id(i, j) == Pawn_id) {
                Piece *p = chessboard->get_piece(i, j);
                p->disable_en_passant();
            }
    
    Color ennemy = get_ennemy_player();
    if (chessboard->get_id(pos1.get_x(), pos1.get_y()) == Pawn_id &&
        ABS((pos2.get_y()-pos1.get_y())) == 2) {
        if (is_in_chessboard(pos2.get_x()+1, pos1.get_y())) {
            if (chessboard->get_id(pos2.get_x()+1, pos2.get_y()) == Pawn_id &&
                chessboard->get_color(pos2.get_x()+1, pos2.get_y()) == ennemy) {
                Piece *p1 = chessboard->get_piece(pos2.get_x()+1, pos2.get_y());
                p1->enable_en_passant(pos2.get_x(), pos2.get_y()
                    - SIGN((pos2.get_y()-pos1.get_y())));
            }
        }
        if (is_in_chessboard(pos2.get_x()-1, pos1.get_y())) {
            if (chessboard->get_id(pos2.get_x()-1, pos2.get_y() == Pawn_id) &&
                chessboard->get_color(pos2.get_x()-1, pos2.get_y()) == ennemy) {
                Piece *p2 = chessboard->get_piece(pos2.get_x()-1, pos2.get_y());
                p2->enable_en_passant(pos2.get_x(), pos2.get_y()
                    - SIGN((pos2.get_y()-pos1.get_y())));
            }
        }
    }
}

void
Game::turn(vector<string> legal_moves)
{
    string movement;
    display();
    cout << "move (example : " << legal_moves[0] << ") ? ";
    cin >> movement;
    if (movement == "/quit") {
        game_state = quit;
        return;
    }
    while ( !(legal_moves.end() != 
            find(legal_moves.begin(), legal_moves.end(), movement)) &&
            !(correct_input_castling_queen_side(movement) && 
            is_legal_castling_queen_side()) &&
            !(correct_input_castling_king_side(movement) &&
            is_legal_castling_king_side())) {
        display();
        cout << "Invalid move" << endl;
        cout << "move (example : " << legal_moves[0] << ") ? ";
        cin >> movement;
        if (movement == "/quit") {
            game_state = quit;
            return;
        }
    }
    if (correct_input_castling_king_side(movement)) {
        chessboard->move_castling_king_side(get_player());
        //if player is doing a castling then he can't do it later
        (get_player() == White) ? 
            white_castling = false : black_castling = false;
    }
    else if (correct_input_castling_queen_side(movement)) {
        chessboard->move_castling_queen_side(get_player());
        //if player is doing a castling then he can't do it later
        (get_player() == White) ? 
            white_castling = false : black_castling = false;
    }
    else { // classic input
        Square pos1 (movement.substr(0,2));
        Square pos2 (movement.substr(2,5));
        pos1.flip_y_axis();
        pos2.flip_y_axis();
        Piece *piece_to_move = chessboard->get_piece(pos1.get_x(),pos1.get_y());
        // if piece is moving then it's not its first turn anymore
        piece_to_move->set_first_move_false();
        // check if it's a "en passant"
        if (chessboard->get_id(pos1.get_x(), pos1.get_y()) == Pawn_id &&
            pos2.get_x() == piece_to_move->get_en_passant_x() &&
            pos2.get_y() == piece_to_move->get_en_passant_y()) {
            move_en_passant(pos1, pos2);
            update_en_passant(pos1, pos2);
        }
        else {
            update_en_passant(pos1, pos2);
            move(pos1, pos2);
        }
    }
    return;    
}

bool
Game::is_legal_castling_king_side() const
{
    int y = ((get_player() == White) ? NO_COLUMNS - 1 : 0);
    //castling is availaible
    if (!is_castling_available())
        return false;
    //pieces on tiles (4,y) ans (7,y) are a king and a rook
    if (!(chessboard->get_id(4, y) == King_id &&
        chessboard->get_id(NO_COLUMNS - 1, y) == Rook_id))
        return false;
    //king and rook never moved
    if (!(chessboard->get_is_first_move(4, y) == true &&
        chessboard->get_is_first_move(NO_COLUMNS - 1, y) == true))
        return false;
    //there is no piece between king and rook
    if (!(chessboard->get_id(5, y) == Empty_id &&
        chessboard->get_id(6, y) == Empty_id))
        return false;
    //king is not in check after the move
    if (is_defended_position(get_ennemy_player(), 6, y))
        return false;
    //king is not passing on a defended tile by the ennemy
    if (is_defended_position(get_ennemy_player(), 5, y))
        return false;
    return true;
}

bool
Game::is_legal_castling_queen_side() const
{
    int y = ((get_player() == White) ? NO_COLUMNS - 1 : 0);
    //castling is availaible
    if (!is_castling_available())
        return false;
    //pieces on tiles (4,y) ans (0,y) are a king and a rook
    if (!(chessboard->get_id(4, y) == King_id &&
        chessboard->get_id(0, y) == Rook_id))
        return false;
    //king and rook never moved
    if (!(chessboard->get_is_first_move(4, y) == true &&
        chessboard->get_is_first_move(0, y) == true))
        return false;
    //there is no piece between king and rook
    if (!(chessboard->get_id(1, y) == Empty_id &&
        chessboard->get_id(2, y) == Empty_id &&
        chessboard->get_id(3, y) == Empty_id))
        return false;
    //king is not in check after the move
    if (is_defended_position(get_ennemy_player(), 2, y))
        return false;
    //king is not passing on a defended tile by the ennemy
    if (is_defended_position(get_ennemy_player(), 3, y))
        return false;
    return true;
}

bool
Game::test_move(Color **color_array,const Square& pos1,const Square& pos2) const
{
    Color player = get_player();
    Color ennemy_player = get_ennemy_player();
    bool **player_defense = new bool *[NO_COLUMNS];
    for (int i = 0; i < NO_COLUMNS; i++) {
        player_defense[i] = new bool[NO_COLUMNS];
        for (int j = 0; j < NO_COLUMNS; j++)
            player_defense[i][j] = false;
    }
    Color mem_color_pos1 = color_array[pos1.get_x()][pos1.get_y()];
    Color mem_color_pos2 = color_array[pos2.get_x()][pos2.get_y()];
    color_array[pos2.get_x()][pos2.get_y()] = player;
    color_array[pos1.get_x()][pos1.get_y()] = Empty;
    for (int i = 0; i < NO_COLUMNS; i++) {
        for (int j = 0; j < NO_COLUMNS; j++) {
            if (color_array[i][j] == ennemy_player) {
                Square pos3 (i, j);
                player_defense = chessboard->get_piece(i,j)
                    ->get_defended_squares (pos3 ,color_array, &player_defense);
            }
        }
    }
    bool test = false;
    if (chessboard->get_id(pos1.get_x(), pos1.get_y()) == King_id)
        test = !player_defense[pos2.get_x()][pos2.get_y()];
    else {
        int *king_pos;
        if (get_player() == Black)
            king_pos = chessboard->get_black_king_pos();
        else
            king_pos = chessboard->get_white_king_pos();
        test = !player_defense[king_pos[0]][king_pos[1]];
    }
    for (size_t i = 0; i < NO_COLUMNS; i++)
        delete[] player_defense[i];
    delete[] player_defense;
    color_array[pos1.get_x()][pos1.get_y()] = mem_color_pos1;
    color_array[pos2.get_x()][pos2.get_y()] = mem_color_pos2;
    return test;
}

vector <string>
Game::get_all_legal_moves() const
{
    vector <string> legal_moves;
    Color **color_array = chessboard->get_color_per_square();
    // first double loop to search all pieces of the player
    for (int i = 0; i < NO_COLUMNS; i++)
        for (int j = 0; j < NO_COLUMNS; j++)
            if (get_player() == chessboard->get_color(i, j)) {
                Piece *piece_temp = chessboard->get_piece(i, j);
                Square pos1 (i, j);
                //if it 's a player piece then if all the availaible moves of
                //this piece are not ended on a check
                bool **legal_square = 
                    piece_temp->get_accessible_squares(pos1, color_array);
                for (int i2 = 0; i2 < NO_COLUMNS; i2++)
                    for (int j2 = 0; j2 < NO_COLUMNS; j2++)
                        if (legal_square[i2][j2]) {
                            Square pos2 (i2, j2);
                            // if the move is legal and it's not leading to 
                            //a check of the current player then we add the
                            // move to the vector of all legal moves
                            if (test_move(color_array, pos1, pos2) == true) {
                                pos1.flip_y_axis();
                                pos2.flip_y_axis();
                                string s = pos1.to_string() + pos2.to_string();
                                legal_moves.push_back(s);
                                pos1.flip_y_axis();
                            }
                        }
                for (size_t i = 0; i < NO_COLUMNS; i++)
                    delete[] legal_square[i];
                delete[] legal_square;
            }
    for (size_t i = 0; i < NO_COLUMNS; i++)
        delete[] color_array[i];
    delete[] color_array;
    if (is_legal_castling_queen_side())
        legal_moves.push_back("o-o-o");
    if (is_legal_castling_king_side())
        legal_moves.push_back("o-o");
    return legal_moves;
}

bool 
Game::is_castling_available() const
{
    return (get_player() == White) ? 
        !check && white_castling : !check && black_castling; 
}

void
Game::set_check_state()
{
    check = is_check();
}

void
Game::move(const Square& pos1, const Square& pos2)
{
    game_state = chessboard->move(pos1, pos2);
}

void
Game::move_en_passant(const Square& pos1, const Square& pos2)
{
    game_state = chessboard->move_en_passant(pos1, pos2);
}

void
Game::next_turn()
{
    turn_count = get_player() == Black ? turn_count + 1 : turn_count;
}

void
Game::switch_player() 
{
    (player == White) ? player = Black : player = White;
}