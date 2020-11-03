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
}