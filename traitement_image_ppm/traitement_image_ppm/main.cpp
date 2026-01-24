#ifdef _WIN32
#include <Windows.h>
#endif //_WIN32

#include "header.h"
#include "functions.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif //_WIN32

    //Recommended website to view PPM https://imagetostl.com/view-ppm-online

    bool is_menu = true;
    char user_choice;

    while (is_menu) {
        std::cout << "\033[0;93m================ PPM IMAGE EDITOR ================\n\n"
            << "- g : Transform PPM P3 image into grayscale\n"
            << "- z : Apply a zoom factor to PPM P3 image\n"
            << "- q : Quit the program\n\n"
            << "==================================================\n\n\033[0m"
            << ">>> ";

        std::cin >> user_choice;

        std::string filename, input_path, output_path;
        float zoom_factor;

        switch (user_choice) {
        case 'q':
            is_menu = false;
            break;

        case 'g': {
            std::cout << "\033[0;36mEnter filename (input/): \n\033[0m>>> ";
            std::cin >> filename;

            input_path = "input/" + filename;
            output_path = "output/grayscale_" + filename;

            std::ifstream input_file(input_path);
            if (!input_file.is_open()) {
                std::cerr << "\033[0;31mError: input file can't be opened.\n\033[0m";
                break;
            }

            Header img_header(input_file);

            input_file.clear();
            input_file.seekg(0);

            toGrayScale(input_file, img_header, output_path);
            std::cout << "\033[0;36mGrayscale image saved as " << output_path << "\n\033[0m";
            break;
        }

        case 'z': {
            std::cout << "\033[0;36mEnter filename (input/) \n\033[0m>>> ";
            std::cin >> filename;

            std::cout << "\033[0;36mZoom factor (> 0): \n\033[0m>>> ";
            std::cin >> zoom_factor;
            if (zoom_factor <= 0) {
                std::cerr << "\033[0;31mError: Zoom factor must be > 0.\n\033[0m";
                break;
            }

            input_path = "input/" + filename;
            output_path = "output/zoom_" + filename;

            std::ifstream input_file(input_path);
            if (!input_file.is_open()) {
                std::cerr << "\033[0;31mError: input file can't be opened.\n\033[0m";
                break;
            }

            Header img_header(input_file);

            input_file.clear();
            input_file.seekg(0);

            zoom(input_file, img_header, output_path, zoom_factor);
            std::cout << "\033[0;36mZoomed image saved as " << output_path << "\n\033[0m";
            break;
        }

        default:
            std::cerr << "\033[0;31mInvalid choice!\n\033[0m";
        }
        std::cout << "\n";
    }
}
