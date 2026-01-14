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

// Turns the AIS payload into "chunks" using the read_payload_chunk function.
string chunk_payload(string &payload) {
    string chunked_payload;

    //for each characters in payload, we read it as 6 bit chunks and add it to chunked_payload.
    for (int i = 0; i < payload.length(); i++) {
        chunked_payload += read_payload_chunk(payload, i);
    }

    return chunked_payload;
}

// Returns the message_id as an unsigned int by extacting it from chunked_payload.
unsigned int get_message_id(string &chunked_payload) {
    //it just corresponds to the first 6 bits.
    return chunked_payload[0];
}

// Returns the user_id as an unsigned int by extacting it from chunked_payload.
unsigned int get_user_id(string &chunked_payload) {

    //on chunked_payload[1], we apply a mask that keep the lats 4 bits and
    //we shift this result by 26 bits on the left (to add them correctly)
    unsigned int user_id = (chunked_payload[1] & 0b001111) << 26;
    //same, we add to user_id chunked_payload[2] shifted by 20 for the next additions.
    user_id += (chunked_payload[2] << 20);
    user_id += (chunked_payload[3] << 14);
    user_id += (chunked_payload[4] << 8);
    user_id += (chunked_payload[5] << 2);
    //we apply a mask on chunked_payload[6] to keep the 2 first bits, then we shift it by
    //4 to get the correct value, and we add it to user_id.
    user_id += (chunked_payload[6] & 0b110000) >> 4;
    
    return user_id;
}

// Returns the name as a string by extacting it from chunked_payload.
string get_name(string &chunked_payload) {
    string name;
    //for the chunked_payload chunks from 18 to 19,
    for (int i = 0; i < 20; i++) {
        //since the 6 bits characters are shifted in our payload, we need to combine 2 chunked_payload chunks.
        //A 6 bit character corresponds to the 2 last bits of our first chunk moved 4 on the left.
        //Then, we add the second part of the 6 bit character in the new chunk : the 4 first bits, moved by 2 on the right.
        char c_6_bits = ((chunked_payload[18 + i] & 0b000011) << 4) + ((chunked_payload[19 + i] & 0b111100) >> 2); 
        //Then, we turn that into 8 bit characters that we understand,
        char c_8_bits = from_6bits_char(c_6_bits);
        //and we add this character to the name string.
        name += c_8_bits;
    }

    return name;
}

// Returns the type as an int by extacting it from chunked_payload.
unsigned short get_type(string &chunked_payload) {

    //to get the type we have to take the 2 last bits of chunked_payload[38] with a mask, shifted by 6 on the lefte.
    unsigned short type = (chunked_payload[38] & 0b000011) << 6;
    //and we add the full chunked_payload[39]
    type += (chunked_payload[39]);

    return type;
}

// Returns the dimensions as an int by extacting it from chunked_payload.
// They can be "understood" as distances with get_distances().
int get_dimensions(string& chunked_payload) {
    int dimensions = 0;

    //for the chunked_payload chunks from 40 to 44,
    for (int i = 0; i < 5; i++) {
        //we shift the bits of dimension by 6 on the left. (ex: XXXXXX --> XXXXXX000000)
        dimensions = (dimensions << 6);
        //and we add the bits of the actual chunk. (ex: XXXXXX000000 + YYYYYY --> XXXXXXYYYYYY)
        dimensions += chunked_payload[40 + i];
    }

    return dimensions;
}

// Returns a vector of unsigned shorts by extacting it from chunked_payload and using get_dimensions().
// structure of the returned vector : {dist_a, dist_b, dist_c, dist_d}
std::vector<unsigned short> get_distances(string& chunked_payload) {
    //we get the dimensions
    int dimensions = get_dimensions(chunked_payload);

    std::vector<unsigned short> distances;

    //dist_a and dist_b are on 9 bits, dist_c and dist_d are on 6 bits.
    //To extract the correct number of bits we use masks.
    //The unprecised bits in the beginning of our marks are 0 by default.
    //For each distances, we shift bits to make them aligned with their mask and save them.
    short dist_a = (dimensions >> 21) & 0b111111111; //9 bits mask
    short dist_b = (dimensions >> 12) & 0b111111111;
    short dist_c = (dimensions >> 6) & 0b111111; //6 bits mask
    short dist_d = dimensions & 0b111111;

    //we then put them in our vector.
    distances.push_back(dist_a);
    distances.push_back(dist_b);
    distances.push_back(dist_c);
    distances.push_back(dist_d);

    return distances;
}

// Returns the total measurements as an int from two distances (unsigned shorts).
int get_measurement(unsigned short& dist_1, unsigned short& dist_2) {
    return dist_1 + dist_2;
}

// Returns a vector of unsigned shorts by extacting it from chunked_payload representing the date.
// structure of the returned vector : {month, day, hour, minute}
std::vector<unsigned short> get_date(string &chunked_payload){
    //we get the estimated time of arrival
    int ETA = 0;

    //we only keep the last 2 bits of the first chunk we need (chunked_payload[45])
    //the first 4 bits of the mask could be removed since they are 0s,
    //but keeping them makes it easier to understand.
    ETA += chunked_payload[45] & 0b000011; 

    //for the chunked_payload chunks from 46 to 48
    for (int i = 0; i < 3; i++) {
        //we shift the bits of dimension by 6 on the left. (ex: XXXXXX --> XXXXXX000000)
        ETA = (ETA << 6);
        //and we add the bits of the actual chunk. (ex: XXXXXX000000 + YYYYYY --> XXXXXXYYYYYY)
        ETA += chunked_payload[46 + i];
    }

    std::vector<unsigned short> date;

    //For each date values, we shift bits to make them aligned with their mask and save them.
    short month = (ETA >> 16) & 0b1111; //4 bits mask
    short day = (ETA >> 11) & 0b11111; //5 bits mask
    short hour = (ETA >> 6) & 0b11111; //5 bits mask
    short minute = ETA & 0b111111; //6 bits mask

    date.push_back(month);
    date.push_back(day);
    date.push_back(hour);
    date.push_back(minute);
    
    return date;
}

// Converts an unsigned short into a string value completed by zeros if it is written in less than 1 character,
// and set as "--" if it's more than 2 characters.
// (ex: 1 -- > "01", 100 --> "--")
string convert_to_date_string(unsigned short& val_short) {
    string val_str = std::to_string(val_short);

    //if it's less or equal to 1 character
    if (val_str.length() <= 1) {
        val_str = "0" + val_str;
    }

    //if it's more than 2 characters
    else if (val_str.length() > 2) {
        val_str = "--";
    }
    return val_str;
}

// Returns a string from a date vector from get_date() and using convert_to_date_string()
// structure of the returned string : MMDDHHMM UTC
string get_date_str(std::vector<unsigned short> &date) {
    string date_str = "";

    //we concatenate our converted date values to our string
    date_str += convert_to_date_string(date[0]);
    date_str += convert_to_date_string(date[1]);
    date_str += convert_to_date_string(date[2]);
    date_str += convert_to_date_string(date[3]);
    date_str += " UTC";

    return date_str;
}

int main()
{
    //================IMPORT AND CONVERSION OF THE PAYLOAD==================

    string payload = "58wt8Ui`g??r21`7S=:22058<v05Htp000000015>8OA;0skeQ8823mDm3kP00000000000";

    //we transform our payload into 6 bits chunks
    string chunked_payload = chunk_payload(payload);

    //================INITIALISATION==================

    //initialisation of the variables we will use.
    unsigned int message_id;
    unsigned int user_id;
    string name = "";
    unsigned short type;
    unsigned short dist_a, dist_b, dist_c, dist_d;
    int dimensions = 0, lenght, width;

    //================GETTING THE VALUES==================
    //we set the values using the functions we made.

    //MESSAGE ID
    message_id = get_message_id(chunked_payload);

    //USER ID
    user_id = get_user_id(chunked_payload);

    //NAME
    name = get_name(chunked_payload);

    //TYPE
    type = get_type(chunked_payload);

    //DIMENSIONS
    
    //distances is a vector of unsigned shorts containing the 4 distances.
    std::vector<unsigned short> distances = get_distances(chunked_payload);

    //we put the corresponding distances into their variable.
    dist_a = distances[0];
    dist_b = distances[1];
    dist_c = distances[2];
    dist_d = distances[3];

    //we can also calculate the lenght and width by adding two distances.
    lenght = get_measurement(dist_a, dist_b);
    width = get_measurement(dist_c, dist_d);

    //ESTIMATED TIME OF ARRIVAL (ETA)

    //date is a vector of unsigned shorts containing the date values
    std::vector<unsigned short> date = get_date(chunked_payload);
    //we can turn this vector into a string that makes it better to read (MMDDHHMM UTC)
    string date_str = get_date_str(date);

    //================PRINTING THE RESULTS==================
    std::cout << "============================AIS============================" << "\n";
    std::cout << "Message ID: " << message_id << "\n";
    std::cout << "User ID: " << user_id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Type of ship: " << type << "\n";
    std::cout << "Distance A: " << dist_a << "\n";
    std::cout << "Distance B: " << dist_b << "\n";
    std::cout << "Distance C: " << dist_c << "\n";
    std::cout << "Distance D: " << dist_d << "\n";
    std::cout << "Lenght of the ship (m): " << lenght << "\n";
    std::cout << "Width of the ship (m): " << width << "\n";
    std::cout << "Estimated tiem of arrival (MMDDHHMM UTC): " << date_str << "\n";
    std::cout << "===========================================================" << "\n";
}