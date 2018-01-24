(c) Matt Garmon - 2017

Algorithms Project 3: Implementation of seam carving on PGM (Portable Gray Map) files.

Note: Seam expansion is also implemented. This is essentially the same as seam carving,
except when a minimum energy seam is to be removed, a seam is inserted with average
values of the pixels around it.


==Building==

Using the included CMakeLists.txt file, run the command "cmake .." without the parenthesis in 
the build directory. This will generate the makefile to build using make commands.


==Executable==

seam


==Usage==

Seam carving:	./seam filename v h
Seam expanding:	./seam filename -v -h

filename = name of PGM file (including .pgm extension)
v = vertical seams to be modified
h = horizontal seams to be modified

Note: A combination of carving and expansion can also be performed. Positive v or h
corresponds to carving, while negative v or h corresponds to expanding.