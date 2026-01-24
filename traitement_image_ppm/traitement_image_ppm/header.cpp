//header.cpp
#include "header.h"

void Header::showHeader()
{
	std::cout << "magic_number: " << magic_number_m << "\n";
	std::cout << "length: " << length_m << "\n";
	std::cout << "height: " << height_m << "\n";
	std::cout << "max_val: " << max_val_m << "\n";
	std::cout << "skip_m: " << skip_m << "\n";
}

//Function that extracts the header of a PPM image file and returns a Header struct containing those informations.
Header::Header(std::ifstream &input_file)
{
    string line;
    skip_m = 0;

    for (int i = 0; i < 3; ++i) {
        //We check if the file is not empty (if we can't get the line, there's a failure)
        if (!std::getline(input_file, line)) {
            std::cerr << "\033[0;31mError : empty file.\n\033[0m";
            exit(EXIT_FAILURE);
        }

        skip_m++;

        //If a line starts by #, we ignore it.
        if (line[0] == '#') { 
            i--;
            continue;
        }

        //If a line is empty, we ignore it.
        if (line.length() == 0) {
            i--;
            continue;
        }

        //MAGIC NUMBER
        if (i == 0) {
            magic_number_m = line;
        }

        //LENGTH AND HEIGHT (it's in the same line, separated by " ")
        if (i == 1) {
            string length, height;
            std::stringstream sstr(line);
            //automatically "cuts" the stream and puts it in the attributed variables.
            sstr >> length >> height;

            //we convert it to integers.
            length_m = std::stoi(length); 
            height_m = std::stoi(height);
        }

        //MAX VALUE
        if (i == 2) {
            max_val_m = std::stoi(line);
        }
    }
}