(c) Matt Garmon - 2017

Algorithms Project 1: Implementation of RSA and digital signatures

(This program utilizes BigIntegerLibrary. For more information see "BigIntegerLibrary_README".)
(This program also utilizes an implementation of sha256. For more information see "sha256_README".)

Executables: rsa435 & sign

(Program is intended to be built using included Makefile. C++11 or above required.)


rsa435       Generates public and private keyset in e_n.txt and d_n.txt
		     Run in terminal with the following command: ./rsa435

sign         Generates signed version of input file
		     Run in terminal with the following command: ./sign s filename
		     Example: ./sign s monkey.jpg

		     Verify a previously signed file
		     Run in terminal with the following command: ./sign v filename.signed
		     Example: ./sign v monkey.jpg.signed