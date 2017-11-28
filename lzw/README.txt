(c) Matt Garmon - 2017

Algorithms Project 2: Implementation of LZW compression with and without variable code length

==Building==

Using the included CMakeLists.txt file, run the command "cmake .." without the parenthesis in 
the build directory. This will generate the make file to build using make commands.

==Executables==

lzw435 & lzw435M

==Usage==

Compression:	./lzw435 c filename		--generates filename.lzw
		./lzw435M c filename		--generates filename.lzw2


Expansion:	./lzw435 e filename.lzw 	--generates filename2
		./lzw435M e filename.lzw2	--generates filename2M

==Credit==

This program utilizes code structure from RosettaCode (http://rosettacode.org/wiki/LZW_compression#C.2B.2B)


