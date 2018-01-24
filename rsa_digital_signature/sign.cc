// (c) Matt Garmon - 2017
// sign.cc - Implementation of Digital Signatures
// (uses RSA keys generated from rsa435.cc in e_n.txt and d_n.txt)

//SHA256 Algorithm
#include "sha256.hh"

//BigInteger library
#include "BigIntegerLibrary.hh"

//Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

//UTILITY: function to compute (a^b)%c (recursive modular exponentiation)
//(also implemented in rsa435.cc)
BigInteger mod_exp(BigInteger a, BigInteger b, BigInteger c) {
	if (b == 0) return 1; //base case
	else if (b%2 == 0) {
		BigInteger z = mod_exp(a, b/2, c); //even case
		return((z*z)%c);
	}
	else return ((a%c)*mod_exp(a,b-1,c))%c; //odd case
}

//UTILITY: function for fast exponentiation (iterative repeated squaring)
BigInteger fast_exp(BigInteger base, BigInteger exp){
	BigInteger result = 1;
	while(exp > 0){ //square base until no exponent
		if(exp%2 == 0){exp = exp/2; base *= base;}
		else{--exp; result *= base; exp = exp/2; base*= base;}
	}
	return result;
}

//UTILITY: function to convert hex string to BigInteger
BigInteger hex_string_to_BigInteger(std::string hex_string){
	std::string bin_string = "";//first convert to binary string
	for(unsigned int i = 0; i < hex_string.size(); ++i){
		switch(hex_string[i]){
			case '0': bin_string.append("0000"); break;
			case '1': bin_string.append("0001"); break;
			case '2': bin_string.append("0010"); break;
			case '3': bin_string.append("0011"); break;
			case '4': bin_string.append("0100"); break;
			case '5': bin_string.append("0101"); break;
			case '6': bin_string.append("0110"); break;
			case '7': bin_string.append("0111"); break;
			case '8': bin_string.append("1000"); break;
			case '9': bin_string.append("1001"); break;
			case 'a': bin_string.append("1010"); break;
			case 'b': bin_string.append("1011"); break;
			case 'c': bin_string.append("1100"); break;
			case 'd': bin_string.append("1101"); break;
			case 'e': bin_string.append("1110"); break;
			case 'f': bin_string.append("1111"); break;
		}
	}
	BigInteger n = 0; //use binary string to compute decimal number
	for(unsigned int i = 0; i < bin_string.size(); ++i){
		if(bin_string[i] == '1') n += fast_exp(2,bin_string.size()-i-1);
	}
	return n;
}

//UTILITY: function to convert a decimal string to a BigInteger
BigInteger dec_string_to_BigInteger(std::string s) {
	BigInteger result; //use string to compute BigInteger
	for(unsigned int i = 0; i < s.size(); ++i){
		switch(s[i]){
			case '0': break;
			case '1': result += fast_exp(10, s.size()-i-1); break;
			case '2': result += BigInteger(2)*fast_exp(10, s.size()-i-1); break;
			case '3': result += BigInteger(3)*fast_exp(10, s.size()-i-1); break;
			case '4': result += BigInteger(4)*fast_exp(10, s.size()-i-1); break;
			case '5': result += BigInteger(5)*fast_exp(10, s.size()-i-1); break;
			case '6': result += BigInteger(6)*fast_exp(10, s.size()-i-1); break;
			case '7': result += BigInteger(7)*fast_exp(10, s.size()-i-1); break;
			case '8': result += BigInteger(8)*fast_exp(10, s.size()-i-1); break;
			case '9': result += BigInteger(9)*fast_exp(10, s.size()-i-1); break;
			case NULL: result /= fast_exp(10, s.size()-i); return result;
		}
	}
	return result;
}

//UTILITY: function to convert a BigInteger into a string
std::string BigInteger_to_dec_string(BigInteger n){
	std::stringstream s; //use stringsteam (can convert anything with << operator)
	s << n;
	return s.str();
}

//UTILITY: function to see if an input file exists
bool exists(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}


//function to read file in binary, returns content string and file size
std::pair<std::string, int> read_binary(std::string filename){
	std::ifstream input (filename, std::ios::binary|std::ios::ate); //open at end in binary
	std::string content; //to hold the content
	std::streampos size; //to hold the size of the content

	if (input.is_open()) { //read the file
		size = input.tellg();
		content.resize(size); 
		input.seekg (0, std::ios::beg);
		input.read(&content[0], content.size());
		input.close();
	}

	return std::make_pair(content, size); //return file contents and size
}

//function to read signed file in binary, returns content string and signature
std::pair<std::string, BigInteger> read_signed_binary(std::string filename, std::streampos sig_size){	
	std::ifstream input (filename, std::ios::binary|std::ios::ate); //open at end in binary
	std::string content; //to hold the content
	std::string signature;//to hold the signature
	std::streampos content_size; //to hold the size of the content

	if (input.is_open()) { //read the file
		content_size = input.tellg()-sig_size;
		content.resize(content_size);
		signature.resize(sig_size);
		input.seekg (0, std::ios::beg);
		input.read (&content[0], content_size); //get content
		input.read (&signature[0], sig_size);   //get signature
		input.close();
	}

	//convert signature string to BigInteger for verification
	BigInteger signature_int = dec_string_to_BigInteger(signature);

	return std::make_pair(content, signature_int); //return file contents and signature
}

//function to generate a BigInteger signature from the file
//uses private RSA key from d_n.txt
BigInteger make_signature (std::string hash) {
	BigInteger hash_int = hex_string_to_BigInteger(hash); //convert hash to BigInteger

	//get private key contents
	std::ifstream key;
	std::string d_string, n_string;
	key.open("d_n.txt");
	if(key.is_open()){
		std::getline(key,d_string);
		std::getline(key,n_string);
		key.close();
	}
	else{
		std::cout << "No RSA key files were found.\n";
		exit(0);
	}

	//compute signature
	BigInteger d = dec_string_to_BigInteger(d_string);
	BigInteger n = dec_string_to_BigInteger(n_string);
	BigInteger sig = mod_exp(hash_int, d, n);

	return sig; //return signature
}

//function to generate a signed file
//takes filename, signature, and signature length as parameters
void generate_signed_file(std::string filename, BigInteger sig, int sig_size){	
	std::string signed_filename = filename + ".signed"; //get signed filename string

	//create a signed file
	std::ofstream signed_file (signed_filename.c_str(), std::ios::binary);
	std::pair<std::string, int> file_and_size = read_binary(filename);
	signed_file.write(file_and_size.first.c_str(), file_and_size.second); //copy original content

	std::string signature = BigInteger_to_dec_string(sig);
	signature.resize(sig_size);
	signed_file.write(signature.c_str(), sig_size); //write signature to file
	signed_file.close();

	std::cout << "Signed file generated: " << signed_filename << '\n';
}


//function to verify the signed file
//takes the recieved file contents as hash and received signature
bool verify(BigInteger recieved_hash, BigInteger signature){
	//get public key contents
	std::ifstream key;
	std::string e_string, n_string;
	key.open("e_n.txt");
	if(key.is_open()){
		std::getline(key,e_string);
		std::getline(key,n_string);
		key.close();
	}
	else{
		std::cout << "No RSA key files were found.\n";
		exit(0);
	}

	//compute encrypted signature
	BigInteger e = dec_string_to_BigInteger(e_string);
	BigInteger n = dec_string_to_BigInteger(n_string);
	BigInteger encrypted_signature = mod_exp(signature, e, n);

	//verify that nothing in the file was modified
	if(recieved_hash == encrypted_signature) return true;
	else return false;
}


int main(int argc, char* argv[]) {
	std::string file(argv[2]); //declare filename
	if(!exists(file)) {std::cout << "Input file does not exist.\n"; return 0;}

	if(argc < 3) {std::cout << "Usage is: s/v filename/filename.signed\n"; return 0;}

	//sign a file
	else if(std::string(argv[1]) == "s"){ 
		std::string input = read_binary(file).first; //read file in bytes
		std::string input_hash = sha256(input); //generate hash from file
		BigInteger sig = make_signature(input_hash); //make signature from file using private key
		generate_signed_file(file, sig, 1024); //generate a signed version

		return 0;
	}

	//verify a file
	else if(std::string(argv[1]) == "v"){
		//read signed file, extract recieved file hash and signature seperately
		std::pair<std::string, BigInteger> file_and_sig; 
		file_and_sig = read_signed_binary(file, 1024);

		std::string received_hash_string = sha256(file_and_sig.first);
		BigInteger received_hash = hex_string_to_BigInteger(received_hash_string);

		//verify that the file was not modified using public key
		//check that file hash is equal to encrypted signature
		std::cout << (verify(received_hash, file_and_sig.second) ? "Verified." : "Modified.") 
				  << '\n';

		return 0;
	}
	else {std::cout << "Usage is: s/v filename\n"; return 0;}

	return 0;
}