/**
 * @file life_game.cpp
 *
 * @description
 * This program simulates the Conway's Game of Life.
 * It reads a initial game configuration from a input file and
 * keeps updating it, if the user so whishes.
 *
 * @author	Selan R dos Santos, <selan.rds@gmail.com>
 * @date	2015, updated 2015-04-03, updated 2024-05-31
 *
 * @remark On 2015-09-19 changed the way stability is verified.
 * In the previous version we just looked back one configuration and compare it
 * with the current configuration.
 * Now, the stability may span more than one configuration. Because of that we
 * need to keep track of all configurations and check whether the current
 * configuration has already been generated.
 * If this is the case, the game has reached stability and must stop.
 */

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream>
#include <string.h>
#include "life.h"
#include <cctype>


//Struct that contains the options provided by the user. 
struct RunningOpt {
    unsigned int generations;   //!<Max number of generations.
    std :: string cell_color;   //!<Color to show the cell.
    std :: string back_color;   //!<Background color.
    size_t pixel_size;    //!<Pixel size of a square cell.
    unsigned int fps;           //!<# of generations presented p/ second.
    std :: string file_name;    //!<Name of the file that contains the beginning of the game. 
    std :: string image_dir;    //!<Name of the file that the png`s will be saved.
};

/*!
 * Print the help message.
 * @return void
 */
void help_message(){
    std :: cout << "Usage: glife [options] input_cfg_file" << std :: endl;
    std :: cout << "Running options:" << std :: endl;
    std :: cout << "    --help Print this help text." << std :: endl;
    std :: cout << "    --maxgen <num> Maximum number of generations to simulate. No default." << std :: endl;
    std :: cout << "    --fps <num> # of generations presented p/ second. Default = 2 fps." << std :: endl;
    std :: cout << "    --imgdir <path> Images output directory." << std :: endl;
    std :: cout << "    --blocksize <num> Pixel size of a square cell. Default = 5." << std :: endl;
    std :: cout << "    --bkgcolor <color> Color name for the background. Default = GREEN." << std :: endl;
    std :: cout << "    --alivecolor <color> Color name for the alive cells. Default = RED." << std :: endl;
    std :: cout << std :: endl;
    std :: cout << "Available colors are:" << std :: endl;
    std :: cout << "BLACK BLUE CRIMSON DARK_GREEN DEEP_SKY_BLUE DODGER_BLUE GREEN LIGHT_BLUE" << std :: endl;
    std :: cout << "LIGHT_GREY LIGHT_YELLOW RED STEEL_BLUE WHITE YELLOW" << std :: endl;
};

RunningOpt validate_input(int argc, char* argv[]){
    RunningOpt input;
    input.pixel_size = 5;
    input.cell_color = "red";
    input.back_color = "green";
    input.fps = 0;
    input.image_dir = "";
    input.file_name = "";
    input.generations = 50;
    if (argc == 1){
        help_message();
        exit(1);
    } 
    else if (argc == 2){
        std :: string arg;
        arg = argv[1];
        if (arg == "-h" || arg == "--help"){
            help_message();
            exit(1);
        }
        else{input.file_name = argv[1];}
    }
    for (int i = 1; i < argc; i++){
        std :: string arg = argv[i];
        if (arg == "-h" || arg == "--help"){
            help_message();
            exit(1);
        }
        else if(arg == "--maxgen"){
            if (i + 1 < argc){input.generations = std :: stoi(argv[i + 1]);}
            else {
                std :: cout << "max of generations was not provided!" << std :: endl;
                help_message();
                exit(1);
            }
        }
        else if(arg == "--fps"){
            if (i + 1 < argc){input.fps = std :: stoi(argv[i + 1]);}
            else {
                std :: cout << "Fps was not provided!" << std :: endl;
                help_message();
                exit(1);
            }
        }
        else if(arg == "--imgdir"){
            if (i + 1 < argc){input.image_dir = argv[i + 1];}
            else {
                std :: cout << "Path to image directory was not provided!" << std :: endl;
                help_message();
                exit(1);
            }
        }
        else if(arg == "--blocksize"){
            if (i + 1 < argc){input.pixel_size = std :: stoi(argv[i + 1]);}
            else {
                std :: cout << "Block size was not provided!" << std :: endl;
                help_message();
                exit(1);
            }
        }
        else if(arg == "--alivecolor"){
            if (i + 1 < argc){
                std :: string temp = argv[i + 1];
                for (char &c : temp){
                    c = tolower(c);
                }
                input.cell_color = temp;
                }
            else {
                std :: cout << "Alive cel color was not provided!" << std :: endl;
                help_message();
                exit(1);
            }
        }
        else if (arg == "--bkgcolor"){
            if (i + 1 < argc){
                std :: string temp = argv[i + 1];
                for (char &c : temp){
                    c = tolower(c);
                }
                input.back_color = temp;
            }
            else {
                std :: cout << "Background color was not provided!" << std :: endl;
                help_message();
                exit(1);
            }
        }
        else if (arg.size() > 4 && arg.substr(arg.size() - 4) == ".txt"){
            input.file_name = arg;
        }
    }
    if (input.file_name == ""){
        std :: cout << "txt file was not provided!" << std :: endl;
        help_message();
        exit(1);
    }
    return input;
};

int main(int argc, char* argv[]) { 
    life :: LifeCfg cw;
    RunningOpt input = validate_input(argc, argv);
    cw.start(input.generations, input.file_name, input.image_dir, input.cell_color, input.back_color, input.pixel_size, input.fps);
    while(not cw.exit_conway()){
        cw.update();
    }
    return EXIT_SUCCESS; 
}
