// (c) Matt Garmon - 2017
// lzw.cpp - Implementation of LZW compression with 12 bit code length

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cassert>

//write a binary string to file 
//compacts 8 binary chars into single bytes
void write_binary_string_file(const std::string &bits, std::string filename){
	std::ofstream output(filename+".lzw", std::ios::binary);
	if(output){
		for(int i = 0; i < bits.size(); i += 8){
			char c = 0;
			for(int j = 0; j < 8; ++j){
				if(bits[i+j] == '1') c |= (1 << j);
			}
			output.put(c);
		}
		output.close();
	}
}

//read a binary string from a file
//expands bytes to 8 binary chars, returns string of chars
std::string read_binary_string_file(std::string filename){
	std::ifstream input(filename+".lzw", std::ios::binary|std::ios::ate);
	if(input){
		std::streampos size;
		std::string bits;
		size = input.tellg()*8;
		bits.resize(size);
		input.seekg(0, std::ios::beg);
		char c = 0;
		for(int i = 0; i < size; i+=8){
			input.get(c);
			for(int j = 0; j < 8; ++j){
				if(c & 1 << j) bits[i+j] = '1';
				else bits[i+j] = '0';
			}
		}
		input.close();
		return bits;
	}
}

//writes a (binary) string to a file
//add '2' to the filename
void string_to_new_file(const std::string &string, std::string filename){
	std::size_t lastindex = filename.find_last_of("."); 
	std::string name = filename.substr(0, lastindex); 
	std::string type = filename.substr(lastindex, filename.size()-name.size());

	std::ofstream file(name + "2" + type, std::ios::binary);
	if(file){
		file.write(string.c_str(), string.size());
		file.close();
	}
}

//read file in binary, returns content string
std::string read_binary(std::string filename){
	//open at end in binary
	std::ifstream input (filename, std::ios::binary|std::ios::ate); 
	std::string content; //to hold the content
	std::streampos size; //to hold the size of the content
	
	if (input) { //read the file
		size = input.tellg();
		content.resize(size); 
		input.seekg(0, std::ios::beg);
		input.read(&content[0], content.size());
		input.close();
	}

	return content; //return file contents and size
}

//computes base^exp
int exp(int base, int exp){
	int result = 1;
	while(exp > 0){ //square base until no exponent
		if(exp%2 == 0){exp /= 2; base *= base;}
		else{--exp; result *= base; exp /= 2; base *= base;}
	}
	return result;
}

//compute integer value from binary string
int binary_to_int(std::string &bits){
	int num = 0, num_bits = bits.size();
	for(int i = 0; i < num_bits; ++i){
		if(bits[i] == '1') num += exp(2, (num_bits-i-1));
	}
	return num;
}

//compute binary string of fixed length from integer
//asserts if binary string cannot fit in num_bits length
std::string int_to_binary(int num, int num_bits){
	std::string bits = "";
	while(num != 0){
		bits.insert(bits.begin(), (num%2 == 0 ? '0' : '1'));
		num /= 2;
	}
	int s = bits.size();
	assert(s <= num_bits);
	if(s < num_bits){
		for(int i = 0; i < (num_bits - s); ++i){
			bits.insert(bits.begin(), '0');
		}
	}
	return bits;
}

//compresses a content string into an LZW binary string
std::string compress(const std::string &uncompressed) {
	//build the dictionary
	int dictSize = 256;
	std::map<std::string,int> dictionary;
	for (int i = 0; i < 256; i++)
		dictionary[std::string(1, i)] = i;
	
	std::string bits; //to hold code bits
	std::string w; //to hold current string

	for (char c : uncompressed) {
		//look for longest string not in dictionary
		std::string wc = w + c;
		if (dictionary.count(wc)) w = wc; //string exists, get next char
		else {
			//add binary code of w to output
			bits += int_to_binary(dictionary[w], 12);

			//if more 12 bit codes exist,
			//add wc to the dictionary
			if(dictSize+1 < (1<<12)) dictionary[wc] = dictSize++;
			
			//set w to current char
			w = std::string(1, c);
		}
	}

	//add binary code of w to output
	if (!w.empty()) bits += int_to_binary(dictionary[w], 12);

	//make sure integer number of bytes, and return
	while(bits.size()%8 != 0) bits += "0"; 
	return bits;
}

//decompresses an LZW binary string into into a content string
std::string decompress(const std::string &compressed) {
	//build the dictionary
	int dictSize = 256;
	std::map<int,std::string> dictionary;
	for (int i = 0; i < 256; i++) 
		dictionary[i] = std::string(1, i);

	//get first code, and add it to output
	std::string bits = compressed.substr(0, 12);
	std::string w(1, binary_to_int(bits)); 
	std::string result = w;

	std::string entry; //to hold current output entry

	for (int i = 12; i < compressed.size(); i += 12) {
		bits = compressed.substr(i, 12); //get next code
		if (bits.size() < 12) break; //handle zero buffer
		int k = binary_to_int(bits);

		if (dictionary.count(k)) entry = dictionary[k]; //get string from code
		else if (k == dictSize) entry = w + w[0]; //special case
		else throw std::logic_error("invalid code"); //something went wrong

		result += entry; //add entry to output

		//if more 12 bit codes exist,
		//add w + first char of entry to dictionary
		if(dictSize+1 < (1<<12)) 
			dictionary[dictSize++] = w + entry[0];

		w = entry; //set w to entry
	}

	return result; //return output string
}

int main(int argc, char* argv[]){
	//get input file string
	std::string user_input(argv[2]);

	if(std::string(argv[1]) == "c"){ //compress
		//compress input file
		std::string input = read_binary(user_input);
		std::string bits = compress(input);
		write_binary_string_file(bits, user_input);
		
		//display compressed filename and file sizes
		std::cout << "Compressed file created: " << user_input << ".lzw\n";
		std::cout << input.size() << " to " << bits.size()/8 << " bytes\n";
	}
	else if(std::string(argv[1]) == "e"){ //decompress
		//decompress input file
		std::size_t lastindex = user_input.find_last_of("."); 
		std::string original_user_input = user_input.substr(0, lastindex); 
		std::string bits2 = read_binary_string_file(original_user_input);
		std::string content = decompress(bits2);
		string_to_new_file(content, original_user_input);

		//display decompressed filename
		std::size_t lastindex2 = original_user_input.find_last_of("."); 
		std::string name = original_user_input.substr(0, lastindex2); 
		std::string type = original_user_input.substr(lastindex2, user_input.size()-name.size());
		std::cout << "Decompressed file created: " << name << '2' << type <<"\n";
	}
	else std::cout << "Invalid input\n";

	return 0;
}