#include<iostream>
#include<fstream>
using namespace std;

int main(int argc, char* argv[]){
    //Read files
    ifstream Infile;
    Infile.open(argv[1], fstream::in);
    if(!Infile.is_open()){
        cout<<"Error:Failed to open input file."<<endl;
        return 1;
    }

    //parameters initialize
    int row, col, battery;
    Infile>>row>>col>>battery;
    char** map = new char*[row];
    for(int i = 0; i < row; i++){
        map[i] = new char[col];
    }
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            Infile>>map[i][j];
        }
    }
    //Test:print the map
    /*for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            cout<<map[i][j]<<" ";
        }
        cout<<endl;
    }*/
    
}