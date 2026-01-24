//functions.cpp
#include "functions.h"

void magicNumberVerification(Header& header) {
    if (header.magic_number_m != "P3") {
        std::cerr << "\033[0;31mError : wrong file format. Only P3 PPMs allowed.\n\033[0m";
        exit(EXIT_FAILURE);
    }
}


void toGrayScale(std::ifstream& input_file, Header& header, string output_path) {

    magicNumberVerification(header);

    std::ofstream output_file(output_path);

    string skip;
    for (int i = 0; i < header.skip_m; i++) {
        std::getline(input_file, skip);
    }

    output_file << "P3\n" << header.length_m << " " << header.height_m << "\n" << header.max_val_m << "\n";

    //3 x 3
    string line;
    string r_str, g_str, b_str;
    while (std::getline(input_file, line)) {
        if (line[0] == '#') { //si une ligne commence par #, on l'ignore
            continue;
        }

        if (line.length() == 0) { //si une ligne est vide, on l'ignore
            continue;
        }

        std::stringstream ss(line);

        while (ss >> r_str >> g_str >> b_str) {
            int r = std::stoi(r_str);
            int g = std::stoi(g_str);
            int b = std::stoi(b_str);

            int gray = (r * 0.2126 + g * 0.7152 + b * 0.0722);

            output_file << gray << " " << gray << " " << gray << "\n";
        }
    }
    std::cout << "\033[0;36mGenerated the grayscaled image : " << output_path << "\n\033[0m";
}


void zoom(std::ifstream& input_file, Header& header, string output_path, float zoom_factor) {
    //We check if the file is "P3" format.
    magicNumberVerification(header);

    //We skip the header
    string skip;
    for (int i = 0; i < header.skip_m; i++) {
        std::getline(input_file, skip);
    }

    //We put the image into a data vector
    std::vector<int> data;
    int value;

    while (input_file >> value) {
        data.push_back(value);
    }

    //RATIO CALCULATION
    //if we want to reduce the image by 2 (zoom_factor = 0.5), the ratio becomes
    //1 / 0.5 = 2, so this means that we will work by blocks of 2*2px.
    int ratio = int(std::ceil(1.0 / zoom_factor));

    //We cap the ratio to 1 if above.
    if (ratio < 1) {
        ratio = 1;
    }

    //We calculate the new sizes
    int new_length = header.length_m / ratio;
    int new_height = header.height_m / ratio;

    //We set up the header
    std::ofstream output_file(output_path);
    output_file << "P3\n" << new_length << " " << new_height << "\n" << header.max_val_m << "\n";

    //CONSTRUCTING THE NEW IMAGE
    //here, we used the code of an old project and updated it for our needs.
    //We "jump" by units of ratio (if ratio = 2, we jump by 2 px) in the original image
    for (int i = 0; i < header.height_m; i += ratio)
    {
        for (int j = 0; j < header.length_m; j += ratio)
        {
            //sum_r, sum_g and sum_b will be used to storek the colored px inside a block
            int sum_r = 0;
            int sum_g = 0;
            int sum_b = 0;
            int n = 0;

            //We go INSIDE a block
            for (int y = 0; y < ratio; ++y)
            {
                for (int x = 0; x < ratio; ++x)
                {
                    int sum_iy = i + y; //line in the block
                    int sum_jx = j + x; //col in the block

                    if (sum_iy < header.height_m && sum_jx < header.length_m)
                    {
                        //We get the pixel value (we multiply by 3 to get the r g b channels,
                        //each takes 3 slots in data)
                        int index = (sum_iy * header.length_m + sum_jx) * 3;

                        sum_r += data[index]; 
                        sum_g += data[index + 1];
                        sum_b += data[index + 2];
                        n++;
                    }
                }
            }

            //Calculation of the block's average value if we found pixels
            if (n > 0)
            {
                int average_r = sum_r / n;
                int average_g = sum_g / n;
                int average_b = sum_b / n;

                //Writing inside the PPM file.
                output_file << average_r << " " << average_g << " " << average_b << " ";
            }
        }
        output_file << "\n";
    }
    std::cout << "\033[0;36mGenerated the zoomed image ("<< zoom_factor << ") : " << output_path << "\n\033[0m";
}