//functions.h
#pragma once
#include "header.h"

void toGrayScale(std::ifstream &input_file, Header &header, string output_path);
void magicNumberVerification(Header& header);
void zoom(std::ifstream& input_file, Header& header, string output_path, float zoom_factor);
