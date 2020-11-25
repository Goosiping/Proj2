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
int path_i;
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
    

    for(int i = 0; i < row; i++){
        for(int j = 0; j < cul; j++){
            cout<<map[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
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
    
    //Output
    Out.open("final.path", fstream::out);
    while(!path_row.empty()){
        Out<<path_row.top()<<" "<<path_cul.top()<<endl;
        path_row.pop();
        path_cul.pop();
    }
    //Start
    path_i = 0;
    //visit(o_row, o_cul, battery);
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