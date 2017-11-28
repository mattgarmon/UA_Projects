#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

//utility absolute value function
int abs(int n){
    return (n < 0 ? -n : n);
}

//utility minimum function
int min(int n, int m){
    return (n < m ? n : m);
}

//represents a 2D grid of integers
//to be used for PGM pixel array representation and energy matrix
struct Grid{
    std::vector<std::vector<int>> items; //to hold integers

    //paramaterized constructor
    Grid(int x, int y){
        items.resize(x);
        for(int i = 0; i < items.size(); ++i)
            items[i].resize(y);
    }

    //to swap rows and columns of grid
    void transpose(){
        std::vector<std::vector<int>> temp(items[0].size());
        for(int i = 0; i < temp.size(); ++i)
            temp[i].resize(items.size());

        for(int i = 0; i < items[0].size(); ++i)
            for(int j = 0; j < items.size(); ++j)
                temp[i][j] = items[j][i];

        items = temp;
    }

    //removes the minimum energy column seam
    void remove_column_seam(){
        Grid e = energy_matrix(*this); //get energy matrix
        
        //remove minimum energy seam
        std::vector<int> minimums = e.min_column_seam();
        for(int i = 0; i < items.size(); ++i){
            items[i].erase(items[i].begin() + minimums[i]);
        }
    }

    //removes the minimum energy row seam
    void remove_row_seam(){
        //transpose grid, work with columns, transpose back
        transpose();
        remove_column_seam();
        transpose();
    }

    //adds a column seam at minimum energy column seam
    void insert_column_seam(){
        Grid e = energy_matrix(*this); //get energy matrix
        
        //insert average pixel value at minimum energy seam
        std::vector<int> minimums = e.min_column_seam();
        int average; //to hold new pixel value
        for(int i = 0; i < items.size(); ++i){
            //left side case
            if(minimums[i] == 0){
                average = items[i][0];
            }
            //right side case
            else if(minimums[i] == items[0].size()-1){
                average = items[i][items[0].size()-1];
            }
            //inside case
            else{
                average = (items[i][minimums[i]-1] + items[i][minimums[i]+1])/2;     
            }
            items[i].insert(items[i].begin() + minimums[i], average);
        }
    }

    //adds a row seam at minimum energy row seam
    void insert_row_seam(){
        //transpose grid, work with columns, transpose back
        transpose();
        insert_column_seam();
        transpose();  
    }

    //function to find a minimum energy column in a grid
    //intended to be used on energy matrix
    std::vector<int> min_column_seam(){
        int rows = items.size(), columns = items[0].size();

        Grid m(rows, columns); //make grid of same size
        m.items[rows-1] = items[rows-1]; //fill bottom row with energy
        
        //work upward calculating minimum energy path
        for(int i = rows-2; i >= 0; --i){
            //left corner
            m.items[i][0] = items[i][0] + min(m.items[i+1][0], m.items[i+1][1]);
            //right corner
            m.items[i][columns-1] = items[i][columns-1] 
                + min(m.items[i+1][columns-1], m.items[i+1][columns-2]);
            //inside
            for(int j = 1; j < columns-1; ++j){
                m.items[i][j] = items[i][j] 
                    + min(m.items[i+1][j-1], min(m.items[i+1][j], m.items[i+1][j+1]));
            }
        }
        
        std::vector<int> minimums(rows); //to hold indices of path
        int index, left, right, middle;
        
        //get initial index of minimum energy path
        std::vector<int>::iterator min;
        min = std::min_element(m.items[0].begin(), m.items[0].end());
        minimums[0] = std::distance(m.items[0].begin(), min);
        
        //work downward to get indices of minimum energy path
        for(int i = 1; i < rows; ++i){
            index = minimums[i-1]; //previous index
            middle = m.items[i][index];
            if(index == 0){ //left side case
                right = m.items[i][index+1];
                
                if(middle <= right) 
                    minimums[i] = 0;
                else minimums[i] = 1;
            }
            else if(index == columns-1){ //right side case
                left = m.items[i][index-1];
                if(left <= middle) 
                    minimums[i] = columns-2;
                else minimums[i] = columns-1;
            }
            else{ //inside case
                left = m.items[i][index-1];
                right = m.items[i][index+1];
                if(left <= middle && left <= right) 
                    minimums[i] = index-1; //left is smallest
                else if(middle <= left && middle <= right) 
                    minimums[i] = index; //middle is smallest
                else if(right <= left && right <= middle) 
                    minimums[i] = index+1; //right is smallest
            }
        }

        return minimums; //return the minimum energy path
    }

    //function to generate the energy matrix of a grid of values
    static Grid energy_matrix(const Grid &g){
        int value, rows = g.items.size(), columns = g.items[0].size();
        Grid e(rows, columns); //make grid of equal size
        
        //visit all items and calculate energy
        for(int i = 0; i < rows; ++i){
            for(int j = 0; j < columns; ++j){
                value = g.items[i][j];
                if(i == 0){ //top row
                    if(j == 0){ //top left corner
                        e.items[i][j] = abs(value - g.items[1][0])
                                      + abs(value - g.items[0][1]);
                    }
                    else if(j == columns-1){ //top right corner
                        e.items[i][j] = abs(value - g.items[1][columns-1])
                                      + abs(value - g.items[0][columns-2]);
                    }
                    else{ //inner top row
                        e.items[i][j] = abs(value - g.items[1][j])
                                      + abs(value - g.items[0][j-1])
                                      + abs(value - g.items[0][j+1]);
                    }
                }
                else if(j == 0){ //leftmost column
                    if(i == rows-1){ //bottom left corner
                        e.items[i][j] = abs(value - g.items[rows-2][0])
                                      + abs(value - g.items[rows-1][1]);
                    }
                    else{ //inner leftmost side
                        e.items[i][j] = abs(value - g.items[i][1])
                                      + abs(value - g.items[i-1][0])
                                      + abs(value - g.items[i+1][0]);
                    }
                }
                else if(i == rows-1){ //bottom row
                    if(j == columns-1){ //bottom right corner
                        e.items[i][j] = abs(value - g.items[rows-1][columns-2])
                                      + abs(value - g.items[rows-2][columns-1]);
                    }
                    else{ //inner bottom row
                        e.items[i][j] = abs(value - g.items[rows-2][j])
                                      + abs(value - g.items[rows-1][j-1])
                                      + abs(value - g.items[rows-1][j+1]);
                    }
                }
                else if(j == columns-1){ //inner rightmost column
                    e.items[i][j] = abs(value - g.items[i][columns-2])
                                  + abs(value - g.items[i-1][columns-1])
                                  + abs(value - g.items[i+1][columns-1]);
                }
                else{ //inside
                    e.items[i][j] = abs(value - g.items[i-1][j])
                                  + abs(value - g.items[i+1][j])
                                  + abs(value - g.items[i][j-1])
                                  + abs(value - g.items[i][j+1]);

                }
            }
        }

        return e; //return energy matrix
    }

    //function to pretty print the grid of integers
    void print() const{
        std::cout << '\n';
        for(int i = 0; i < items.size(); ++i){
            for(int j = 0; j < items[i].size(); ++j)
                std::cout << std::setw(4) << items[i][j];
            std::cout << '\n';
        }
    }
};

//function to parse a PGM file to a grid of integers
Grid parse_pgm(std::string filename){
	std::string line;
    int columns, rows;
    std::ifstream image(filename.c_str()); //open pgm image
    getline(image, line); //read version and discard
    getline(image, line); //read comment and discard
    image >> columns; //read columns
    image >> rows; //read rows
    getline(image, line); //read max value and discard
    
    Grid g(rows, columns); //fill grid with grayscale values
	for(int i = 0; i < rows; ++i)
        for(int j = 0; j < columns; ++j)
            image >> g.items[i][j];
    return g;
}

//function to generate a PGM file from a grid of integers
void generate_pgm(const Grid &g, std::string filename){
    std::size_t i = filename.find_last_of("."); 
    std::string name = filename.substr(0, i);
    std::ofstream image(name + "_processed.pgm");
    image << "P2\n";
    image << "# Seam carved by Matt\n";
    image << g.items[0].size() << ' ' << g.items.size() << '\n';
    image << "255\n";
    for(int i = 0; i < g.items.size(); ++i){
        for(int j = 0; j < g.items[i].size(); ++j){
            image << g.items[i][j] << ' ';
        }
        image << '\n';
    }
}

//testing suite
int main(int argc, char* argv[]){
	std::string file(argv[1]);

    //vertical and horizontal seams
    //negative values will expand image
    int v = stoi(std::string(argv[2]));
    int h = stoi(std::string(argv[3]));

    Grid pixels = parse_pgm(file); //parse pgm fill
    for(int i = 0; i < abs(v); ++i){ //columns
        if(v < 0) pixels.insert_column_seam();
        else pixels.remove_column_seam();
    }
    for(int j = 0; j < abs(h); ++j){ //rows
        if(h < 0) pixels.insert_row_seam();
        else pixels.remove_row_seam();
    }
    generate_pgm(pixels, file); //write new pgm

    //display new filename
    std::size_t i = file.find_last_of("."); 
    std::string name = file.substr(0, i);
    std::cout << "New file created: " 
              << name << "_processed.pgm\n";

	return 0;
}