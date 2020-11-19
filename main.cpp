#include<iostream>
#include<fstream>
using namespace std;
char **map;
int o_row, o_cul;
int row, cul, battery;
int **path;
int path_i;
fstream Out;

bool end(void){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul ; j++){
            if(map[i][j] <= '0'){
                return false;
            }
        }
    }
    return true;
}

void visit(int c_row, int c_cul, int c_battery){
    if(end()){
        return;
    }
    path[path_i][0] = c_row;
    path[path_i][1] = c_cul;
    path_i ++;
    map[c_row][c_cul] += 1;
    Out<<c_row<<" "<<c_cul<<endl;
    cout<<c_row<<" "<<c_cul<<" "<<c_battery<<map[c_row][c_cul + 1]<<map[c_row - 1][c_cul]<<map[c_row][c_cul - 1]<<endl;
    //If run out of battery
    if(c_battery < battery / 2 + 1){
        for(int i = path_i - 2; i > 0; i--){
            Out<<path[i][0]<<" "<<path[i][1]<<endl;
        }
        path_i = 0;
        visit(o_row, o_cul, battery);
        return;
    }
    //Charge at the charge point
    if(c_row == o_row && c_cul == o_cul)
        c_battery = battery;
    //Next step direction
    char dir;
    char min = map[c_row][c_cul + 1];
    if(c_row + 1 < row && map[c_row + 1][c_cul] < min){
        dir = 'd';
        min = map[c_row + 1][c_cul];
    }
    else if(c_cul - 1 >= 0 && map[c_row][c_cul - 1] < min){
        dir = 'l';
        min = map[c_row][c_cul - 1];
    }
    else if(c_row - 1 >= 0 && map[c_row - 1][c_cul] < min){
        dir = 'u';
        min = map[c_row - 1][c_cul];
    }
    else{
        dir = 'r';
    }
    if(c_row == 98 && c_cul == 98){
        cout<<dir<<endl;
    }
    //Next step
    switch (dir)
    {
    case 'r':
        visit(c_row, c_cul + 1, c_battery - 1);
        break;
    case 'd':
        visit(c_row + 1, c_cul, c_battery - 1);
        break;
    case 'l':
        visit(c_row, c_cul - 1, c_battery - 1);
        break;
    case 'u':
        visit(c_row - 1, c_cul, c_battery - 1);
        break;
    default:
        break;
    }
}

int main(int argc, char* argv[]){
    //Read files
    ifstream Infile;
    Infile.open(argv[1], fstream::in);
    if(!Infile.is_open()){
        cout<<"Error:Failed to open input file."<<endl;
        return 1;
    }

    //parameters initialize
    
    Infile>>row>>cul>>battery;
    map = new char*[row];
    for(int i = 0; i < row; i++){
        map[i] = new char[cul];
    }
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            Infile>>map[i][j];
        }
    }
    path = new int *[battery / 2 + 2];
    for(int i = 0; i < battery / 2 + 2; i++)
        path[i] = new int [2];
    //Find the charge point
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            if(map[i][j] == 'R'){
                o_row = i;
                o_cul = j;
                map[i][j] = '0';
            }
        }
    }
    //Map modification
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            if(map[i][j] == '1')
                map[i][j] = '~';
        }
    }
    //Output
    Out.open("final.path", fstream::out);
    //Start
    path_i = 0;
    visit(o_row, o_cul, battery);
    //Test:print the map
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            cout<<map[i][j]<<" ";
        }
        cout<<endl;
    }
    Infile.close();
    Out.close();
}