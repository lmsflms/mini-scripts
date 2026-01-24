//header.h
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

using string = std::string;
using vector_s = std::vector<string>;

struct Header {
	string magic_number_m;
	int length_m = 0;
	int height_m = 0;
	int max_val_m = 0;
	int skip_m = 0;
	
	//METHODS
	void showHeader();
	Header(std::ifstream &input_file);
};