#include <iostream>
#include <vector>
#include <string>
using string = std::string;

// Read Ais payload 6 bits chunck.
// Remaining 2 MSB bits are left to zero.
// Reference: https://gpsd.gitlab.io/gpsd/AIVDM.html# aivdmaivdo payload armoring
unsigned char read_payload_chunk(const std::string& payload, int position)
{
    char c = payload[position] - 48;
    if (c > 40)
        c = c - 8;
    return c;
}

// Convert an Ais 6 bits char into c char.
// Reference: https://gpsd.gitlab.io/gpsd/AIVDM.html# ais payload data types
unsigned char from_6bits_char(unsigned char c)
{
    if (c >= 0 and c <= 31)
        return c + 64;
    return c;
}

//Turns the AIS payload into "chunks" using the read_payload_chunk function.
string chunk_payload(string &payload) {
    string chunked_payload;

    for (int i = 0; i < payload.length(); i++) {
        chunked_payload += read_payload_chunk(payload, i);
    }

    return chunked_payload;
}

//message_id just corresponds to the first 6 bits.
unsigned int get_message_id(string &chunked_payload) {
    return chunked_payload[0];
}

unsigned int get_user_id(string &chunked_payload) {
    unsigned int user_id;

    user_id = (chunked_payload[1] & 0b001111) << 26;
    user_id += (chunked_payload[2] << 20);
    user_id += (chunked_payload[3] << 14);
    user_id += (chunked_payload[4] << 8);
    user_id += (chunked_payload[5] << 2);
    user_id += (chunked_payload[6] & 0b110000) >> 4;
    
    return user_id;
}

string get_name(string &chunked_payload) {
    string name;

    for (int i = 0; i < 20; i++) {
        char c_6_bits = ((chunked_payload[18 + i] & 0b10001) << 4) + ((chunked_payload[19 + i] & 0b111100) >> 2); //we keep the two last bits of this payload part
        char c_8_bits = from_6bits_char(c_6_bits);

        name += c_8_bits;
    }

    return name;
}

int get_type(string &chunked_payload) {
    int type;

    type = (chunked_payload[38] & 0b000011) << 6;
    type += (chunked_payload[39]);

    return type;
}

int get_dimensions(string& chunked_payload) {
    int dimensions = 0;

    for (int i = 0; i < 5; i++) {
        dimensions = (dimensions << 6);
        dimensions += chunked_payload[40 + i];
    }

    return dimensions;
}

std::vector<unsigned short> get_distances(int& dimensions) {
    std::vector<unsigned short> distances;

    short dist_a = (dimensions >> 21) & 0b111111111;
    short dist_b = (dimensions >> 12) & 0b111111111;
    short dist_c = (dimensions >> 6) & 0b111111;
    short dist_d = dimensions & 0b111111;

    distances.push_back(dist_a);
    distances.push_back(dist_b);
    distances.push_back(dist_c);
    distances.push_back(dist_d);

    return distances;
}

int get_measurement(unsigned short& dist_1, unsigned short& dist_2) {
    return dist_1 + dist_2;
}

//Gets the date as a vector with month, day, hour and minute.
std::vector<unsigned short> get_date(string &chunked_payload){
    int ETA = 0;

    ETA += chunked_payload[45] & 0b000011;

    for (int i = 0; i < 3; i++) {
        ETA = (ETA << 6);
        ETA += chunked_payload[46 + i];
    }

    std::vector<unsigned short> date;

    short month = (ETA >> 16) & 0b1111;
    short day = (ETA >> 11) & 0b11111;
    short hour = (ETA >> 6) & 0b11111;
    short minute = ETA & 0b111111;

    date.push_back(month);
    date.push_back(day);
    date.push_back(hour);
    date.push_back(minute);
    
    return date;
}

//Returns a string with the date under the format :
// [month]/[day] - [hour]:[minute]
string get_date_str(std::vector<unsigned short> &date) {
    string date_str = "";
    date_str += std::to_string(date[0]);
    date_str += "/" + std::to_string(date[1]);
    date_str += " - " + std::to_string(date[2]);
    date_str += ":" + std::to_string(date[3]);

    return date_str;
}

int main()
{
    string payload = "58wt8Ui`g??r21`7S=:22058<v05Htp000000015>8OA;0skeQ8823mDm3kP00000000000";
    string chunked_payload = chunk_payload(payload);

    //Initialisation
    unsigned int message_id;
    unsigned int user_id;
    string name = "";
    unsigned short type;
    unsigned short dist_a, dist_b, dist_c, dist_d;
    int dimensions = 0, lenght, width;

    //===================MESSAGE ID=====================
    message_id = get_message_id(chunked_payload);

    //====================USER ID======================
    user_id = get_user_id(chunked_payload);

    //=====================NAME======================
    name = get_name(chunked_payload);

    //======================TYPE======================
    type = get_type(chunked_payload);

    //====================DIMENSIONS=====================
    dimensions = get_dimensions(chunked_payload);
    std::vector<unsigned short> distances = get_distances(dimensions);

    dist_a = distances[0];
    dist_b = distances[1];
    dist_c = distances[2];
    dist_d = distances[3];

    lenght = get_measurement(dist_a, dist_b);
    width = get_measurement(dist_c, dist_d);

    //===========ESTIMATED TIME OF ARRIVAL (ETA)============
    std::vector<unsigned short> date = get_date(chunked_payload);
    string date_str = get_date_str(date);

    //TESTS
    std::cout << "message_id: " << message_id << "\n";
    std::cout << "user_id: " << user_id << "\n";
    std::cout << "name: " << name << "\n";
    std::cout << "type: " << type << "\n";
    std::cout << "dist_a: " << dist_a << "\n";
    std::cout << "dist_b: " << dist_b << "\n";
    std::cout << "dist_c: " << dist_c << "\n";
    std::cout << "dist_d: " << dist_d << "\n";
    std::cout << "lenght: " << lenght << "\n";
    std::cout << "width: " << width << "\n";
    std::cout << "date_str: " << date_str << "\n";
}