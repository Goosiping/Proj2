#include<iostream>
#include<fstream>
#include<queue>
#include<stack>
using namespace std;
char **map;
int o_row, o_cul;
int row, cul, battery;
int c_battery;
int **path;
fstream Out;
stack<int> path_row;
stack<int> path_cul;

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
    path = new int *[row];
    for(int i = 0; i < row; i++)
        path[i] = new int [cul];
    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            path[i][j] = -2;
        }
    }
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
            if(map[i][j] == '1'){
                map[i][j] = '~';
                path[i][j] = -1;
            }
        }
    }
    //BFS find the minimun path to charge point
    queue<int> bfs_row;
    queue<int> bfs_cul;
    bfs_row.push(o_row);
    bfs_cul.push(o_cul);
    path[o_row][o_cul] = 0;
    while(!bfs_row.empty()){
        int temp_row = bfs_row.front();
        int temp_cul = bfs_cul.front();
        bfs_row.pop();
        bfs_cul.pop();
        if(temp_cul + 1 < cul && path[temp_row][temp_cul + 1] == -2){//right
            bfs_row.push(temp_row);
            bfs_cul.push(temp_cul + 1);
            path[temp_row][temp_cul + 1] = path[temp_row][temp_cul] + 1;
        }
        if(temp_row + 1 < row && path[temp_row + 1][temp_cul] == -2){//down
            bfs_row.push(temp_row + 1);
            bfs_cul.push(temp_cul);
            path[temp_row + 1][temp_cul] = path[temp_row][temp_cul] + 1;
        }
        if(temp_cul - 1 >= 0 && path[temp_row][temp_cul - 1] == -2){//left
            bfs_row.push(temp_row);
            bfs_cul.push(temp_cul - 1);
            path[temp_row][temp_cul - 1] = path[temp_row][temp_cul] + 1;
        }
        if(temp_row - 1 >=0 && path[temp_row - 1][temp_cul] == -2){//up
            bfs_row.push(temp_row - 1);
            bfs_cul.push(temp_cul);
            path[temp_row - 1][temp_cul] = path[temp_row][temp_cul] + 1;
        }
    }
    //DFS traversal
    int temp_row = o_row;
    int temp_cul = o_cul;
    char  min;
    int min_row, min_cul;
    c_battery = battery;
    while(!end()){
        path_row.push(temp_row);
        path_cul.push(temp_cul);
        map[temp_row][temp_cul]++;
        //need to come back or not
        if(c_battery <= path[temp_row][temp_cul]){
            while(temp_row != o_row && temp_cul!= o_cul){
                if(temp_cul + 1 < cul && path[temp_row][temp_cul + 1] == path[temp_row][temp_cul] - 1){//right
                    path_row.push(temp_row);
                    path_cul.push(temp_cul + 1);
                    temp_cul = temp_cul + 1;
                }
                else if(temp_row + 1 < row && path[temp_row + 1][temp_cul] == path[temp_row][temp_cul] - 1){//down
                    path_row.push(temp_row + 1);
                    path_cul.push(temp_cul);
                    temp_row = temp_row + 1;
                }
                else if(temp_cul - 1 >= 0 && path[temp_row][temp_cul - 1] == path[temp_row][temp_cul] - 1){//left
                    path_row.push(temp_row);
                    path_cul.push(temp_cul - 1);
                    temp_cul = temp_cul - 1;
                }
                else if(temp_row - 1 >= 0 && path[temp_row - 1][temp_cul] == path[temp_row][temp_cul] - 1){//up
                    path_row.push(temp_row - 1);
                    path_cul.push(temp_cul);
                    temp_row = temp_row - 1;
                }
            }
            c_battery = battery;
            temp_row = o_row;
            temp_cul = o_cul;
        }
        //find the direction
        else{
            min = '~';
            if(temp_cul + 1 < cul && min > map[temp_row][temp_cul + 1]){//right
                min = map[temp_row][temp_cul + 1];
                min_row = temp_row;
                min_cul = temp_cul + 1;
            }
            if(temp_row + 1 < row && min > map[temp_row + 1][temp_cul]){//down
                min = map[temp_row + 1][temp_cul];
                min_row = temp_row + 1;
                min_cul = temp_cul;
            }
            if(temp_cul - 1 >= 0 && min > map[temp_row][temp_cul - 1]){//left
                min = map[temp_row][temp_cul - 1];
                min_row = temp_row;
                min_cul = temp_cul - 1;
            }
            if(temp_row - 1 >= 0 && min > map[temp_row - 1][temp_cul]){//up
                min = map[temp_row - 1][temp_cul];
                min_row = temp_row - 1;
                min_cul = temp_cul;
            }
            temp_row = min_row;
            temp_cul = min_cul;
            c_battery--;
        }
    }
    //Back to charge point
    while(temp_row != o_row || temp_cul!= o_cul){
        if(temp_cul + 1 < cul && path[temp_row][temp_cul + 1] == path[temp_row][temp_cul] - 1){//right
            path_row.push(temp_row);
            path_cul.push(temp_cul + 1);
            temp_cul = temp_cul + 1;
        }
        else if(temp_row + 1 < row && path[temp_row + 1][temp_cul] == path[temp_row][temp_cul] - 1){//down
            path_row.push(temp_row + 1);
            path_cul.push(temp_cul);
            temp_row = temp_row + 1;
        }
        else if(temp_cul - 1 >= 0 && path[temp_row][temp_cul - 1] == path[temp_row][temp_cul] - 1){//left
            path_row.push(temp_row);
            path_cul.push(temp_cul - 1);
            temp_cul = temp_cul - 1;
        }
        else if(temp_row - 1 >= 0 && path[temp_row - 1][temp_cul] == path[temp_row][temp_cul] - 1){//up
            path_row.push(temp_row - 1);
            path_cul.push(temp_cul);
            temp_row = temp_row - 1;
        }
    }
    //Output
    Out.open("final.path", fstream::out);
    stack<int> real_row, real_cul;
    int steps = 0;
    while(!path_row.empty()){
        real_row.push(path_row.top());
        real_cul.push(path_cul.top());
        path_row.pop();
        path_cul.pop();
        steps++;
    }
    Out<<steps<<endl;
    while(!real_row.empty()){
        Out<<real_row.top()<<" "<<real_cul.top()<<endl;
        real_row.pop();
        real_cul.pop();
    }
    
    //Test:print the map
    /*for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            cout<<map[i][j]<<" ";
        }
        cout<<endl;
    }*/
    Infile.close();
    Out.close();
}