/**
 * LLSeq class implementation.
 *
 */

#include "life.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace life {
/**
 * @brief Constructor for LifeCfg class.
 */
LifeCfg::LifeCfg() :
    m_ending(ending_e::UNDEFINED),
    m_state(state_e::STARTING),
    m_stop(false),
    m_extinct(false),
    m_exit(false),
    m_n_gen(1),
    m_cols(0),
    m_rows(0),
    m_maxgen(10),
    m_fps(2),
    m_back_color("green"),
    m_cell_color("red"),
    m_txt_file(""),
    m_image_dir(""),
    m_file_path(""),
    m_table(),
    m_old_tables(),
    m_canvas(0, 0, 5)
    {}

// TODO

/*!
 * @brief Updates the state of the simulation based on the current state (`m_state`).
 * 
 * The update function performs different actions based on the current state of the simulation:
 * - If `m_state` is `STARTING`, it reads the configuration file, initializes the canvas (`m_canvas`),
 *   displays a welcome message, and transitions to the `RUNNING` state.
 * - If `m_state` is `RUNNING`, it checks if the simulation should stop (`m_stop`). If so, it transitions
 *   to the `END` state; otherwise, it displays the current state of the simulation (Conway's Game of Life),
 *   optionally prints the simulation to an image file, updates the generation (`m_n_gen`), and checks
 *   for conditions such as stability, maximum generations, or extinction to potentially stop the simulation.
 * - If `m_state` is `END`, it displays an end message and sets `m_exit` to `true`.
 * 
 * @note This function is designed to be called iteratively to advance the simulation state.
 */
void LifeCfg :: update(void){
    switch(m_state){
        case state_e :: STARTING:
            read_file();
            m_canvas.start_canva(static_cast<short>(m_pixel), static_cast<size_t>(m_cols), static_cast<size_t>(m_rows));
            display_welcome();
            m_state = state_e :: RUNNING;
            break;
        case state_e :: RUNNING:
            if (m_stop == true){
                m_state = state_e :: END;
                break;
            }
            display_conway();
            if (m_image_dir != ""){
                size_t height = m_canvas.height();
                size_t width = m_canvas.width();
                unsigned block_size = m_pixel;
                Canvas canvas(width, height, block_size);
                paint_pixel(m_table, canvas);
                make_words(m_image_dir);
                string ppm = m_file_path + ".ppm";
                string png = m_file_path + ".png";
                canvas.encode_png(png, canvas.pixels(), width, height);
                canvas.save(canvas.pixels(), width, height, 4, ppm);
                
            }
            update_gen();
            break;
        case state_e :: END:
            display_end();
            m_exit = true;
            break;
        case state_e :: UNDEFINED:
            break;
    }
};

/**
 * @brief Returns the number of rows in the simulation grid.
 * @return Number of rows.
 */
unsigned int LifeCfg :: rows(void) const{return m_rows;};

/**
 * @brief Returns the number of columns in the simulation grid.
 * @return Number of columns.
 */
unsigned int LifeCfg :: cols(void) const{return m_cols;};

/**
 * @brief Returns the maximum number of generations for the simulation.
 * @return Maximum number of generations.
 */
unsigned int LifeCfg :: max_gen(void) const{return m_maxgen;};

/**
 * @brief Returns the frames per second (fps) setting for the simulation.
 * @return Frames per second.
 */
unsigned int LifeCfg :: fps(void) const{return m_fps;};

/**
 * @brief Returns the background color setting for the simulation.
 * @return Background color.
 */
std :: string LifeCfg :: back_color(void) const{return m_back_color;};

/**
 * @brief Returns the cell color setting for the simulation.
 * @return Cell color.
 */
std :: string LifeCfg :: cell_color(void) const{return m_cell_color;};

/**
 * @brief Returns the filename used for initializing the simulation.
 * @return Filename.
 */
std :: string LifeCfg :: file_name(void) const{return m_txt_file;};

/**
 * @brief Returns the directory path where generated images are stored.
 * @return Image directory path.
 */
std :: string& LifeCfg :: image_dir(void){return m_image_dir;};

/**
 * @brief Returns the current state of the simulation grid.
 * @return Current simulation grid.
 */
std :: vector<std :: vector<unsigned int>> LifeCfg :: table(void) const{return m_table;};

/**
 * @brief Returns a list of all previous simulation states.
 * @return List of previous simulation states.
 */
std :: vector<std :: string> LifeCfg :: old_tables(void) const{return m_old_tables;}; 

/**
 * @brief Compares the current simulation state with previously saved states.
 * @param olds List of previous simulation states.
 * @param table Current simulation state as a string.
 * @return True if the current state matches any previous state, false otherwise.
 */
bool LifeCfg :: compare(vector<string> olds, string table){
    for (long unsigned int i = 0; i < olds.size(); i++){
        if (olds[i] == table){return true;}
    }
    return false; 
};

/**
 * @brief Checks if all cells in the simulation grid are dead.
 * @param table Current simulation state as a string.
 * @return True if all cells are dead, false otherwise.
 */
bool LifeCfg :: all_dead(std :: string table){
    for (char c : table){
        if (c == '1'){return false;}
    }
    return true;
};

/**
 * @brief Checks if the simulation has reached its end condition.
 * @return True if the simulation has reached its end, false otherwise.
 */
bool LifeCfg :: exit_conway() const{
    if (m_exit){return true;}
    else{return false;}
};

/**
 * @brief Reads the initial configuration from a file.
 */
void LifeCfg :: read_file(void){
    std :: ifstream file(m_txt_file);
    if (!file.is_open()){
        std :: cerr << "Unable to open the file!" << std :: endl;
        std :: exit(EXIT_FAILURE);
    }
    std :: string line;
    std :: getline(file, line);
    std::istringstream iss(line);
    std::string token;
    if (!(iss >> token)) {
        std::cerr << "Error reading dimensions from file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    m_rows = std::stoi(token);
    if (!(iss >> token)) {
        std::cerr << "Error reading dimensions from file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    m_cols = std::stoi(token);   
    if (m_cols < 3 || m_rows < 3) {
        std::cerr << "The dimensions stated are insufficient." << std::endl;
        std::exit(EXIT_FAILURE);
    }   
    m_table.resize(m_rows, std::vector<unsigned int>(m_cols, 0));
    std :: getline(file, line);
    char alive = line[0];
    unsigned int i = 0;
    while(std :: getline(file, line)){
        for (unsigned int j = 0; j < m_cols; j++){
            if (line[j] == alive){
                m_table[i][j] = 1;
            }
            else {m_table[i][j] = 0;}
        }
        i++;
        if (i >= m_rows){break;}
    }
    file.close();
};

/**
 * @brief Converts the simulation grid to a string representation.
 * @param table Simulation grid to convert.
 * @return String representation of the simulation grid.
 */
std :: string LifeCfg :: table_to_string(std :: vector<std :: vector<unsigned int>>& table){
    std :: stringstream ss;
    for (const auto& row : table) {
        for (const auto& cell : row){
            ss << cell;
        }
        ss << '\n'; 
    }
    return ss.str();   
};

/**
 * @brief Initializes the simulation with provided parameters.
 * @param gen Maximum number of generations.
 * @param file Filename for initial configuration.
 * @param dir Directory for storing generated images.
 * @param cell Cell color.
 * @param back Background color.
 * @param pixel Pixel size for images.
 * @param fps Frames per second.
 */
void LifeCfg :: start(unsigned int gen, std :: string file, std :: string dir, std :: string cell, std :: string back, 
unsigned int pixel, unsigned int fps){
    m_fps = fps;
    m_image_dir = dir;
    m_maxgen = gen;
    m_txt_file = file;
    m_cell_color = cell;
    m_back_color = back;
    m_pixel = pixel;
};

/**
 * @brief Displays a welcome message at the start of the simulation.
 */
void LifeCfg :: display_welcome(void) const{
    std :: cout << ">>> Trying to open input file [" << m_txt_file << "]... done!" << std :: endl;
    std :: cout << ">>> Running simulation up to" << m_maxgen << "generations, or until extinction/stability is reached, whichever comes first." << std :: endl;
    std :: cout << ">>> Processing data, please wait..." << std :: endl;
    std :: cout << ">>> Grid size read from input file: " << m_rows << " rows by "<< m_cols <<" cols." << std :: endl;
    std :: cout << ">>> Character that represents a living cell read from input file: '*' " << std :: endl;
    std :: cout << ">>> Finished reading input data file." << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "********************************************************************" << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "    Welcome to Conway's game of Life." << std :: endl;
    std :: cout << "    Running a simulation on a grid of size " << m_rows << " by " << m_cols << " in which" << std :: endl;
    std :: cout << "    each cell can either be occupied by an organism or not." << std :: endl;
    std :: cout << "    The occupied cells change from generation to generation" << std :: endl;   
    std :: cout << "    according to the number of neighboring cells which are alive." << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "********************************************************************" << std :: endl;
};

/**
 * @brief Updates the simulation grid to the next generation.
 */
void LifeCfg :: update_gen(void){
    m_n_gen++;
    std :: string old_table = table_to_string(m_table);
    if (compare(m_old_tables, old_table)){
        m_stop = true;
        m_ending = ending_e :: STABILITY;
    }
    if (m_n_gen == m_maxgen){
        m_stop = true;
        m_ending = ending_e :: MAXGEN;
    }
    if (all_dead(old_table)){
        m_stop = true;
        m_ending = ending_e :: EXTINCTION;
    }
    m_old_tables.push_back(old_table);
    std :: vector<vector<unsigned int>> temp_table = m_table;
    for (unsigned int i = 0; i < m_rows; i++){
        for (unsigned int j = 0; j < m_cols; j++){
            if (set_alive(temp_table, i, j, temp_table[i][j])){m_table[i][j] = 1;}
            else {m_table[i][j] = 0;}
        }
    }
};

/**
 * @brief Determines if a cell will live, die, or be born based on game rules.
 * @param table Current simulation grid.
 * @param row Row index of the cell.
 * @param col Column index of the cell.
 * @param cell Current state of the cell (alive or dead).
 * @return True if the cell will be alive in the next generation, false otherwise.
 */ 
bool LifeCfg :: set_alive(const std :: vector<std :: vector<unsigned int>> table, unsigned int row, unsigned int col, unsigned int cell){
    int n_alives = 0;
    bool alive = false;

    // Obtém o número de linhas e colunas da matriz
    size_t rows = table.size();
    size_t cols = table[0].size();

    // Verifica células vizinhas de acordo com a posição (row, col)
    for (int cell_row = -1; cell_row <= 1; ++cell_row) {
        for (int cell_col = -1; cell_col <= 1; ++cell_col) {
            if (cell_row == 0 && cell_col == 0) continue; // Ignora a célula atual

            int neighbor_row = row + cell_row;
            int neighbor_col = col + cell_col;

            if (neighbor_row < 0) neighbor_row = rows - 1;
            if (static_cast<size_t>(neighbor_row) >= rows) neighbor_row = 0;
            if (neighbor_col < 0) neighbor_col = cols - 1;
            if (static_cast<size_t>(neighbor_col) >= cols) neighbor_col = 0;

            if (table[neighbor_row][neighbor_col] == 1) {
                ++n_alives;
            }
        }
    }

    if (cell == 1) { 
        if (n_alives == 2 || n_alives == 3) {alive = true;} 
        else {alive = false;}
    } 
    else { 
        if (n_alives == 3) {alive = true;} 
        else {alive = false;}
    }

    return alive;
};

/**
 * @brief Displays the current state of the simulation grid.
 */
void LifeCfg :: display_conway(void) const{
    std :: cout << "Generation " << m_n_gen << ":" << std :: endl;
    for (unsigned int i = 0; i < m_rows; i++){
        std :: cout << "[";
        for (unsigned int j = 0; j < m_cols; j++){
            if (m_table[i][j] == 1){
                std :: cout << "*";
            }
            else {std :: cout << " ";}
        } 
        std :: cout << "]" << std :: endl;
    }
};

/**
 * @brief Generates a random file name.
 * @param filename Directory path where the file will be stored.
 */
void LifeCfg :: make_words(std :: string& filename){
    static const char letras[] = "abcdefghijklmnopqrstuvwxyz"; 
    int tamLetras = sizeof(letras) - 1;
    static int counter = 0;
    std :: string word = "";
    for (int i = 0; i < 3; ++i) {
      word += letras[counter % tamLetras];
      counter++;
    }
    m_file_path = filename + "/" + word;
};

/**
 * @brief Displays a farewell message at the end of the simulation.
 */
void LifeCfg :: display_end(void) const{
    std :: cout << "********************************" << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "End of conway." << std :: endl;
    switch(m_ending){
        case ending_e :: EXTINCTION:
            std :: cout << "There is no alive cell, all of then are dead.";
            break;
        case ending_e :: STABILITY:
            std :: cout << "The alives cells find stability.";
            break;
        case ending_e :: MAXGEN:
            std :: cout << "The informed generation limit has been reached.";
            break;
        case ending_e :: UNDEFINED:
            break;        
    }
    std :: cout << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "********************************" << std :: endl;
};

/**
 * @brief Paints the simulation grid into a Canvas object.
 * @param table Current simulation grid.
 * @param canvas Canvas object where the grid will be painted.
 */
void LifeCfg :: paint_pixel(std :: vector<std :: vector<unsigned int>>& table, Canvas& canvas){
    size_t row = table.size();
    size_t col = table[0].size();
    for (size_t i = 0; i < row; i++){
        for (size_t j = 0; j < col; j++){
            if (table[i][j] == 1){canvas.pixel(j, i, color_pallet[m_cell_color]);}
            else if(table[i][j] == 0){canvas.pixel(j, i, color_pallet[m_back_color]);}
            
        }
    }  
};

}  // namespace life