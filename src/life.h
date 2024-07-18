//! This class implements a life board in the Conway's Game of Life.
/*!
 * @file life.h
 *
 * @details Class Life, to simulate the Conway's Game of Life.
 *
 * @author	Selan R dos Santos, <selan.rds@gmail.com>
 * @date	2015, updated 2015-04-03
 * @update 2019, April 2nd.
 * @version     v1.1
 */

#ifndef _LIFE_H_
#define _LIFE_H_

#include <cassert>
#include <cstring>  // std::memcpy().
#include <iostream>
#include <set>
#include <sstream>  // std::ostringstream
#include <stdexcept>
#include <string>
#include <vector>
#include "common.h"

using std::cerr;
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;

#include "canvas.h"

namespace life {

/// A life configuration.
class LifeCfg {
    private:
    enum class state_e : short{
        STARTING = 0,
        RUNNING,
        END,
        UNDEFINED,
    };

    enum class ending_e : short {
        UNDEFINED = 0,
        EXTINCTION,
        MAXGEN,
        STABILITY,
    };

    ending_e m_ending;                      //!< Cause of conway's end.
    state_e m_state;                        //!< Current state.
    bool m_stop;                            //!< Flag to check if the conway ends.
    bool m_extinct;                         //!< Flag to check if a cell will die.
    bool m_exit;                            //!< Flag to end the program.
    unsigned int m_n_gen = 1;               //!< # of current generation.
    unsigned int m_cols;                    //!< # of colums.
    unsigned int m_rows;                    //!< # of rows.
    unsigned int m_maxgen;                  //!< Max number of generations.
    unsigned int m_fps;                     //!< # of generations presented p/ second.
    unsigned int m_pixel;
    string m_back_color;                    //!< Dead cell color.
    string m_cell_color;                    //!< Alive cell color.
    string m_txt_file;                      //!< Txt file name.
    string m_image_dir;                     //!< Image directory name.
    string m_file_path;                     //!< Image file.
    vector<vector<unsigned int>> m_table;   //!< Conways table.
    vector<string> m_old_tables;            //!< Tables already made.
    Canvas m_canvas;                        //!< Canvas object


    public:
    LifeCfg();  // Lifecgf constructor.
    ~LifeCfg(){ /* empty */ };
    
    //!< Update the conway.
    void update(void);
    
    //!< Returns true if the cell will be extinguished.
    bool extinct(void) const; 
    
    //!< Returns the # of rows.
    unsigned int rows(void) const;
    
    //!< Returns the # of cols.
    unsigned int cols(void) const; 

    //!< Returns the max of generations.
    unsigned int max_gen(void) const;

    //!< Returns the fps.
    unsigned int fps(void) const;

    //!< Return the pixel_size.
    unsigned int pixel_size(void) const;

    //!< Returns the background color.
    string back_color(void) const;

    //!< Returns the cell color.
    string cell_color(void) const;
    
    //!< Returns the name file.
    string file_name(void) const;

    //!< Returns the directory name to storage de png.
    std :: string& image_dir(void);

    //!< Returns the table.
    vector<vector<unsigned int>> table(void) const;

    //!< Returns the tables already made.
    vector<string> old_tables(void) const;
    
    //!< Returns true if conway has already been registered.
    bool compare(vector<string> olds, string table);

    //!< Returns true if all cells are dead.
    bool all_dead(string table);

    //!< Return true if is the end of conway.
    bool exit_conway() const;
    
    //!< Reads the file with columns, rows, and starting cell locations.
    void read_file(void); 

    //!< Transforms a table into a string.
    string table_to_string(vector<vector<unsigned int>>&);

    //!< Starts the object with its members provided in the imput.
    void start(unsigned int generations, string file, string dir, string cell, string back, unsigned int pixel, unsigned int fps);

    //!< Update the table to the next gen.
    void update_gen(void);

    //!< Check if the cell will die, live or be born. 
    bool set_alive(const vector<vector<unsigned int>> table, unsigned int row, unsigned int col, unsigned int cell);

    //!< Make words.
    void make_words(string& filename);

    //!< Display welcome mensage.
    void display_welcome(void) const;

    //!< Display conway.
    void display_conway(void) const; 

    //!< Display the farewell.
    void display_end(void) const;

    //!< Paint the pixel.
    void paint_pixel(vector<vector<unsigned int>>& table, Canvas& canvas);

};

}  // namespace life

#endif
