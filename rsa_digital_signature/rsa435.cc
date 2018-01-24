// (c) Matt Garmon - 2017
// rsa435.cc - Implementation of RSA using BigIntegerLibrary

//BigInteger library
#include "BigIntegerLibrary.hh"

//Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <utility>
#include <fstream>

//UTILITY: function to compute a really big odd integer
BigInteger random_big_odd() {
	BigInteger n = 1;
	for (int i = 0; i < 150; ++i) { //get random big integer
		n = n*10 + rand();
	}
	if (n%2 == 0) return n+1; //make odd
	else return n;
}

//UTILITY: function to compute (a^b)%c (recursive modular exponentiation)
BigInteger mod_exp(BigInteger a, BigInteger b, BigInteger c) {
	if (b == 0) return 1; //base case
	else if (b%2 == 0) {
		BigInteger z = mod_exp(a, b/2, c); //even case
		return((z*z)%c);
	}
	else return ((a%c)*mod_exp(a,b-1,c))%c; //odd case
}

//function to compute gcd and coefficients (Extended Euclidean)
BigInteger ex_euclid(BigInteger a, BigInteger b, BigInteger &x, BigInteger &y) {
    if (b == 0) { x = 1; y = 0; return a;} //base case
    BigInteger x_r, y_r, gcd = ex_euclid(b, a%b, x_r, y_r); //recursive euclid
    x = y_r;
    y = x_r - (a/b)*y_r;
    return gcd; //return gcd with modified x and y
}

//function to test primality using Fermat's Little Theorem
bool fermat_test(BigInteger p) {
    return (mod_exp(2, p-1, p) == 1) //test with 2, 3, and 5
    && (mod_exp(3, p-1, p) == 1)     //very good chance it's prime
    && (mod_exp(5, p-1, p) == 1);
}

//function to compute a really big prime
BigInteger big_prime() {
    BigInteger x = random_big_odd();
    while(!fermat_test(x)) x += 2; //use fermat test to get probable prime
    return x;
}

//function to make a public and private key pair (e and d respectively)
std::pair<BigInteger,BigInteger> make_keys(BigInteger p, BigInteger q){
    BigInteger phi = (p-1)*(q-1), x, y; //calculate phi and declare coefficients
    BigInteger e(65537); //standard RSA exponent
    while (ex_euclid(e, phi, x, y) != 1) e += 2; //make sure e is co-prime to phi
    if (x<0) x = phi + x; //modular arithmetic
    return std::make_pair(e,x); //return e(first) and d(second)
}

//Main function
//writes newly generated RSA keys to output files
int main() {
    srand(time(0)); //seed random number generator

    std::ofstream p_q, e_n, d_n; //open output files
    p_q.open ("p_q.txt");
    e_n.open ("e_n.txt");
    d_n.open ("d_n.txt");

    BigInteger p = big_prime(); //get random primes p and q
    BigInteger q = big_prime();

    BigInteger n = p*q; //compute n = p*q

    //get public and private keys, e and d
    std::pair<BigInteger,BigInteger> keys = make_keys(p, q);

    //write to output files
    if (p_q.is_open() && e_n.is_open() && d_n.is_open()) { 
        p_q << p << '\n';
        p_q << q << '\n';
        e_n << keys.first << '\n';
        e_n << n << '\n';
        d_n << keys.second << '\n';
        d_n << n << '\n';
        p_q.close();
        e_n.close();
        d_n.close();
        std::cout << "RSA keys generated.\n";
        std::cout << "p_q.txt e_n.txt d_n.txt have been written.\n";
    }
    else std::cout << "Error opening output files!\n";

    return 0;
}