#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <cmath>
#include <cctype>
#include <queue> // Ajout de la bibliothèque de files d'attente


class Player
{
public:
    std::string name;
    std::string color;
    char starting_tile;
    int tile_exchange_coupons;
    std::set<std::pair<int, int>> territory; // Coordonnées des tuiles sur le plateau

    Player() : starting_tile(' '), tile_exchange_coupons(0)
    {
        // Autres initialisations si nécessaires
    }

    void place_starting_tile(std::vector<std::vector<char>>& board);
    void play_turn(std::vector<std::vector<char>>& board);
    void use_bonus_square(std::string bonus_type, std::vector<std::vector<char>>& board);
    void display_board(const std::vector<std::vector<char>>& board);
    bool is_valid_move(int row, int col, const std::vector<std::vector<char>>& board);
};

class Game
{
public:
    int num_players;
    std::vector<Player> players;
    int board_size;
    std::vector<std::vector<char>> board;
    std::unordered_map<std::string, double> bonus_squares;

    Game(int num_players);

    void setup_game();
    void play_game();
    void display_board();
    bool is_valid_move(int row, int col, const Player& player);
};

Game::Game(int num_players) : num_players(num_players){
    board_size = (num_players <= 4) ? 20 : 30;
    board = std::vector<std::vector<char>>(board_size, std::vector<char>(board_size, ' '));

    bonus_squares = { {"Tile Exchange", 1.5 * num_players},
                     {"Stone", 0.5 * num_players},
                     {"Robbery", 1.0 * num_players} };
}

void Player::place_starting_tile(std::vector<std::vector<char>>& board){
    int row, col;

    display_board(board);

    std::cout << name << ", c'est à vous de placer votre première tuile." << std::endl;

    do
    {
        std::cout << "Entrez la position pour placer votre tuile (par exemple, A;C) : ";
        char input[4];  // Permet de stocker "A;C" et le caractère nul '\0'
        std::cin >> input;

        // Extraire les valeurs de la chaîne
        col = std::toupper(input[0]) - 'A';
        row = std::toupper(input[2]) - 'A';

        if (row >= 0 && row < board.size() && col >= 0 && col < board[0].size() && board[row][col] == ' ')
        {
            // La case est valide et non occupée
            starting_tile = toupper(name[0]);
            board[row][col] = starting_tile;
            territory.insert({ row, col });

            // Afficher le tableau mis à jour
            display_board(board);

            break;
        }
        else
        {
            std::cout << "Placement invalide. Veuillez réessayer." << std::endl;
        }
    } while (true);
}

void Player::play_turn(std::vector<std::vector<char>>& board){
    int row, col;

    display_board(board);

    std::cout << "C'est au tour de " << name << " :" << std::endl;
    std::cout << "Entrez la ligne et la colonne pour placer votre tuile (par exemple, 2 3) : ";
    std::cin >> row >> col;

    if (is_valid_move(row, col, board))
    {
        territory.insert({ row, col });
        std::cout << "Entrez la tuile a placer (par exemple, 'G' pour herbe) : ";
        char tile;
        std::cin >> tile;
        board[row][col] = tile;

        // Afficher le tableau mis à jour
        display_board(board);
    }
    else
    {
        std::cout << "Mouvement invalide. Veuillez reessayer." << std::endl;
    }
}

void Player::use_bonus_square(std::string bonus_type, std::vector<std::vector<char>>& board){
    // Pour simplifier, augmentez simplement tile_exchange_coupons dans cet exemple
    if (bonus_type == "Tile Exchange")
    {
        tile_exchange_coupons++;
    }
    else
    {
        std::cout << "Type de bonus invalide." << std::endl;
    }
}

void Player::display_board(const std::vector<std::vector<char>>& board){
    for (const auto& row : board)
    {
        for (char tile : row)
        {
            std::cout << tile << ' ';
        }
        std::cout << std::endl;
    }
}

bool Player::is_valid_move(int row, int col, const std::vector<std::vector<char>>& board){
    // Vérifiez si le mouvement est à l'intérieur des limites du plateau et si la tuile est adjacente au territoire du joueur
    return (row >= 0 && row < board.size() && col >= 0 && col < board[0].size() &&
        (std::abs(row - territory.begin()->first) == 1 || std::abs(col - territory.begin()->second) == 1) &&
        territory.find({ row, col }) == territory.end());
}

void Game::setup_game(){
    for (int i = 0; i < num_players; ++i)
    {
        Player player;
        std::cout << "Entrez le nom du joueur " << i + 1 << " : ";
        std::cin >> player.name;
        std::cout << "Entrez la couleur du joueur " << i + 1 << " : ";
        std::cin >> player.color;

        players.push_back(player);

        // Nouvelle étape pour que chaque joueur place sa première tuile
        std::cout << "C'est a " << player.name << " de placer sa premiere tuile.\n";
        player.place_starting_tile(board);
    }
}

void Game::play_game(){
    for (int round = 0; round < 9; ++round)
    {
        for (auto& player : players)
        {
            player.play_turn(board);
        }

        // Ajouter ici une logique pour les tuiles bonus si nécessaire
    }

    // Fin de la logique du jeu
}

void Game::display_board(){
    // Afficher l'état actuel du plateau
    for (const auto& row : board)
    {
        for (char tile : row)
        {
            std::cout << tile << ' ';
        }
        std::cout << std::endl;
    }
}

void display_cli_board(const std::vector<std::vector<char>>& board){
    // Affichez les coordonnées des colonnes
    std::cout << "    ";
    for (char c = 'A'; c < 'A' + board[0].size(); ++c)
    {
        std::cout << " " << c << " ";
    }
    std::cout << std::endl;

    // Ajoutez une ligne séparatrice
    std::cout << "   +";
    for (size_t i = 0; i < board[0].size(); ++i)
    {
        std::cout << "---";
    }
    std::cout << "+" << std::endl;

    char row_label = 'A';
    for (size_t i = 0; i < board.size(); ++i)
    {
        // Affichez les coordonnées de ligne
        std::cout << " " << row_label++ << " |";
        for (size_t j = 0; j < board[i].size(); ++j)
        {
            // Affichez la tuile du joueur, ou '.' s'il n'y en a pas
            std::cout << " " << (board[i][j] == ' ' ? '.' : board[i][j]) << " ";
        }
        std::cout << "|" << std::endl;
    }
    // Ajoutez une ligne séparatrice
    std::cout << "   +";
    for (size_t i = 0; i < board[0].size(); ++i)
    {
        std::cout << "---";
    }
    std::cout << "+" << std::endl;
}


struct Tile {
    std::string name;
    std::vector<std::vector<char>> shape;

    Tile(const std::string& n, const std::vector<std::vector<char>>& s) : name(n), shape(s) {}
};

// Définition des tuiles
Tile tile1("Tuile 1", { {'a'}, {'a', 'a', 'a'} });
Tile tile2("Tuile 2", { {' ', ' ', 'a'}, {' ', ' ', 'a'}, {'a', 'a', 'a'} });
Tile tile3("Tuile 3", { {' ', 'a', ' '}, {'a', 'a', 'a'}, {' ', 'a', ' '} });
Tile tile4("Tuile 4", { {' ', ' ', ' ', 'a'}, {' ', ' ', 'a', ' '}, {'a', 'a', 'a', 'a'} });
Tile tile5("Tuile 5", { {' ', 'a', ' '}, {'a', 'a', 'a'}, {' ', ' ', ' '} });
Tile tile6("Tuile 6", { {'a', 'a'}, {'a', 'a'} });
Tile tile7("Tuile 7", { {'a', ' ', ' ', 'a'}, {'a', 'a', 'a', 'a'} });
// ... Définissez les autres tuiles de la même manière

// Fonction pour afficher une tuile


void displayTile(const Tile& tile) {
    std::cout << tile.name << ":\n";
    for (const auto& row : tile.shape) {
        for (char c : row) {
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}








int main()
{
    int num_players;
    std::cout << "Entrez le nombre de joueurs (2-9) : ";
    std::cin >> num_players;

    if (num_players >= 2 && num_players <= 9)
    {
        Game game(num_players);
        game.setup_game();
        game.display_board();

        // Exemple d'affichage pour le deuxième joueur au deuxième tour
        Player& current_player = game.players[1]; // Supposons que le deuxième joueur joue au deuxième tour
        int current_round = 2;

        std::cout << "Joueur " << current_player.name << " (" << current_player.color << ") - Tour " << current_round << " :\n";
        display_cli_board(game.board);
        


        std::queue<Tile> tileQueue;

        // Ajoutez les tuiles à la file d'attente
        tileQueue.push(tile1);  // Ajoutez la Tuile 1
        tileQueue.push(tile2);
        tileQueue.push(tile3);
        tileQueue.push(tile4);
        tileQueue.push(tile5);
        tileQueue.push(tile6);
        tileQueue.push(tile7);
        // ... Ajoutez les autres tuiles à la file d'attente

        // Affichez la première tuile actuelle
        Tile currentTile = tileQueue.front();
        tileQueue.pop();
        std::cout << "Tuiles actuelles :\n";
        displayTile(currentTile);

        // Affichez les 5 prochaines tuiles
        std::cout << "Prochaines tuiles :\n";
        for (int i = 0; i < 5; ++i) {
            displayTile(tileQueue.front());
            tileQueue.pop();
        }


        return 0;
    }
}