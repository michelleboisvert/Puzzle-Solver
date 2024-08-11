// A8A3A33EF075ACEF9B08F5B9845569ECCB423725

// EECS 281, Project 1 - Puzzle Solver

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <stack>
#include "xcode_redirect.hpp"
using namespace std;

class Solver
{
  public:
    Solver(size_t N, size_t heightOfMap, size_t widthOfMap, bool stackOrQueue, const pair<size_t, size_t> &startingPos, const vector<vector<char>> &mymap)
      :numColors(N + 1), height(heightOfMap), width(widthOfMap), isStack(stackOrQueue), startPos(startingPos) {
        currentColor = '^';
        currentState = make_pair(currentColor, startPos);
        puzzlemap = mymap;
        noSolMap = mymap;
        mybacktraces.resize(numColors, vector<vector<char>>(height, vector<char>(width, '0')));

        mybacktraces[0][startPos.first][startPos.second] =  '@';//adding start to default bt
        noSolMap[startPos.first][startPos.second] = '!';
        searchContainer.push_back(currentState);
      }
    
    void findPath()
    {
      while(!searchContainer.empty()){
        if(isSolution)
          break;
        if(isStack){//stack
          currentState = searchContainer.back();
          searchContainer.pop_back();
        }
        else {//queue
          currentState = searchContainer.front();
          searchContainer.pop_front();
        }
        currentColor = currentState.first;
        char tempChar = puzzlemap[currentState.second.first][currentState.second.second];
        size_t b = buttons.find(tempChar);
        if(b != string::npos && b != buttons.find(currentState.first)){//is button and not already in this color
          if(mybacktraces[b][currentState.second.first][currentState.second.second] == '0'){//checking if new color backtrace has not been reached
            mybacktraces[buttons.find(tempChar)][currentState.second.first][currentState.second.second] = currentColor;
            noSolMap[currentState.second.first][currentState.second.second] = '!';
            searchContainer.push_back(make_pair(tempChar, make_pair(currentState.second.first, currentState.second.second)));
          }
        }
        else {
          int temprow = static_cast<int>(currentState.second.first);
          int tempcol = static_cast<int>(currentState.second.second);
          temprow--;//north
          investigate(temprow, tempcol, 'S');
          temprow++; tempcol++;//east
          investigate(temprow, tempcol, 'W');
          temprow++; tempcol--;//south
          investigate(temprow, tempcol, 'N');
          temprow--; tempcol--;
          investigate(temprow, tempcol, 'E');
        }
    }
    }
  void makePath(){
    searchContainer.clear();
    pair<char, pair<size_t, size_t>> tempPos = targetPos;
    pair<char, pair<size_t, size_t>> start = make_pair('^', startPos);
    size_t index = buttons.find(tempPos.first);
    char before = mybacktraces[index][tempPos.second.first][tempPos.second.second];
    while(tempPos != start){
    searchContainer.push_back(tempPos);
    index = buttons.find(tempPos.first);
    before = mybacktraces[index][tempPos.second.first][tempPos.second.second];
    if(before == 'N')
      tempPos.second.first--;
    else if(before == 'E')
      tempPos.second.second++;
    else if(before == 'S')
      tempPos.second.first++;
    else if(before == 'W')
      tempPos.second.second--;
    else//is a button
      tempPos.first = before;
    }
    searchContainer.push_back(make_pair('^', startPos));

    reverse(searchContainer.begin(), searchContainer.end());
  }
  deque<pair<char, pair<size_t, size_t>>> &getPath(){
      return searchContainer;
    }
  void investigate(int row, int col, const char &cameFrom){
      size_t index = buttons.find(currentColor);
      size_t temprow = static_cast<size_t>(row);
      size_t tempcol = static_cast<size_t>(col);
      bool inBounds = true;
      bool isWalkable = true;

      if(int(temprow) < 0 || int(temprow) > (int(height) - 1))
        inBounds = false;
      if(int(tempcol) < 0 || int(tempcol) > (int(width) - 1))
        inBounds = false;
      if(inBounds){
        char temp = puzzlemap[size_t(row)][size_t(col)];
        size_t d = doors.find(temp);
        if(temp == '#')
          isWalkable = false;
        else if(d != string::npos && buttons.find(currentColor) != d)//is door and not on current color
          isWalkable = false;
      }

      if(inBounds && isWalkable && mybacktraces[index][temprow][tempcol] == '0'){
        pair<char, pair<size_t, size_t>> newPosition = make_pair(currentColor, make_pair(temprow, tempcol));
        if(puzzlemap[temprow][tempcol] == '?'){//is target
          isSolution = true;
          targetPos = make_pair(currentColor, make_pair(temprow, tempcol));
          mybacktraces[index][temprow][tempcol] = cameFrom;
        }
        else{
          searchContainer.push_back(newPosition);
          mybacktraces[index][temprow][tempcol] = cameFrom;
          noSolMap[temprow][tempcol] = '!';
        }
      }
    }
  bool getSolution(){
      return isSolution;
    }
  vector<vector<vector<char>>> &getMap(){
    return mybacktraces;
  }

  vector<vector<char>> const  &getNonDiscoveredMap(){
    for(size_t i = 0; i < height; ++i){
      for(size_t j = 0; j < width; ++j){
        if(noSolMap[i][j] != '!')//hasnt been discovered
          noSolMap[i][j] = '#';
        else  
          noSolMap[i][j] = puzzlemap[i][j];
      }
    }
    return noSolMap;
  }

  private:
    size_t numColors;
    size_t height;
    size_t width;
    bool isStack;
    pair<size_t, size_t> startPos;
    pair<char, pair<size_t, size_t>> targetPos;
    vector<vector<char>> puzzlemap;
    char currentColor;
    bool isSolution = false;
    pair<char, pair<size_t, size_t>> currentState;
    vector<vector<vector<char>>> mybacktraces;//3d vec
    vector<vector<char>> noSolMap;
    string buttons = "^abcdefghijklmnopqrstuvwxyz";
    string doors = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // _ instead of ^ bc this is just a button
    deque<pair<char, pair<size_t, size_t>>> searchContainer; //searchContainer and path
};


// These outputs represent the different ways to process the input data.
// There are three modes and the state where no mode has been selected.
enum class Mode {
  kNone = 0,
  kMap,
  kList,
};  // Mode{}

enum class ContainerType{
  kNone = 0,
  kQueue,
  kStack,
}; // ContainerType{}


struct Options {
  Mode mode = Mode::kNone;
  ContainerType type = ContainerType::kNone;
};  // Options{}


// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void getMode(int argc, char * argv[], Options &options) {
  // These are used with getopt_long()
  opterr = false; // Let us handle all error output for command line options
  int choice;
  int index = 0;
  option long_options[] = {
    { "output",  required_argument, nullptr, 'o'  },
    { "queue", no_argument,       nullptr, 'q'},
    { "stack", no_argument,       nullptr, 's'},
    { "help",  no_argument,       nullptr, 'h'  },
    { nullptr, 0,                 nullptr, '\0' }
  };  // long_options[]
  bool queueOrStack = false;
  bool mapOrList = false;

  while ((choice = getopt_long(argc, argv, "o:qsh", long_options, &index)) != -1) {
    switch (choice) {
      case 'h':
        cout << "This program is to find the solution path of a given map,\n";
        cout << "starting at @ and ending at ? and outputting the path\n";
        cout << "in different formats, map or list." << endl;
        exit(0);
      case 'o':{
        string arg{optarg};
        if(arg != "map" && arg != "list"){
          cerr << "Error: invalid output choice. Please include list or map" << endl;
          exit(1);
        }
        else if(mapOrList){
          cerr << "Error: both map and list outputs have been specified" << endl;
          exit(1);
        }
        else{
          if(arg == "map")
            options.mode = Mode::kMap;
          else
            options.mode = Mode::kList;
          mapOrList = true;
        }
        break;
      }// case 'o'
      case 'q':
        if(queueOrStack){//this indicates there are two in the command line
          cerr << "Error: Queue or Stack has already been specified" << endl;
          exit(1);
        }
        else{
          queueOrStack = true;
          options.type = ContainerType::kQueue;
        }
        break;
      // case 'q'
      case 's':
        if(queueOrStack){
          cerr << "Error: Queue or Stack has already been specified" << endl;
          exit(1);
        }
        else{
          queueOrStack = true;
          options.type = ContainerType::kStack;
        }
        break;
      //case 's'
      default:
        cerr << "Error: invalid option" << endl;
        exit(1);
      }  // switch ..choice
  } // while

  /*if(options.mode == Mode::kNone) {
    cerr << "Error: no mode specified" << endl;
    exit(1);
  }  // if ..mode*/ // should instead assume -o map
  if(options.type == ContainerType::kNone){
    cerr << "Error: no container type specified" << endl;
    exit(1);
  }
}  // getMode()


int main(int argc, char *argv[]) {
  // This should be in all of your projects, speeds up I/O
  ios_base::sync_with_stdio(false);

  xcode_redirect(argc, argv);

  bool mapOrList; // true - map, false - list
  bool stackOrQueue; // true - stack, false - queue
  size_t N;
  size_t height;
  size_t width;
  pair<size_t, size_t> startPos;
  bool startfound = false;
  bool targetfound = false;

  string validChars = "@?.#^";
  string validbuttons = "^abcdefghijklmnopqrstuvwxyz";
  string validDoors = "^ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  //input gathering
  cin >> N;
  cin >> height;
  cin >> width;

  vector<vector<char>> mymap(height, vector<char>(width, '0'));

  int n = static_cast<int>(N);
  //Error Checking
  if(n < 0 || n > 26){ // 0 <= N <= 26
    cerr << "Error: Number of colors inputted is invalid" << endl;
    exit(1);
  }
  if(height < 1){ // 1 <= height
    cerr << "Error: Invalid height size. Height must be greater than or equal to 1" << endl;
    exit(1);
  }
  if(width < 1){ // 1 <= width
    cerr << "Error: Invalid width size. Width must be greater than or equal to 1" << endl;
    exit(1);
  }

  // Get the mode from the command line and read in the data
  Options options;
  getMode(argc, argv, options);

  if(options.mode == Mode::kMap || options.mode == Mode::kNone)
    mapOrList = true;
  else
    mapOrList = false;
  
  if(options.type == ContainerType::kStack)
    stackOrQueue = true;
  else
    stackOrQueue = false;
  
  //input and read in data
  string line;
  size_t rowcounter = 0;
  size_t colcounter = 0;
  string junk;
  getline(cin, junk);
  while(getline(cin, line)){
    if(line[0] != '/' && line[1] != '/'){//then not a comment
      for(size_t i = 0; i < line.size(); ++i){
        char tempChar = line[i];
        if(tempChar == '@'){
          if(startfound){
            cerr << "Error: starting location has already been found!" << endl;
            exit(1);
          }
          else{
            startPos.first = rowcounter;
            startPos.second = colcounter;
            startfound = true;
          }
        }
        else if(tempChar == '?'){
          if(targetfound){
            cerr << "Error: target location has already been found!" << endl;
            exit(1);
          }
          else{
            targetfound = true;
          }
        }
        else if(validbuttons.find(tempChar) != string::npos){
          size_t index = validbuttons.find(tempChar);
          if(index > N){
            cerr << "Error: invalid button found!" << endl;
            exit(1);
          }
        }
        else if(validDoors.find(tempChar) != string::npos){
          size_t index = validDoors.find(tempChar);
          if(index > N){
            cerr << "Error: invalid door found!" << endl;
            exit(1);
          }
        }
        else{
          if(validChars.find(tempChar) == string::npos){
            cerr << "Error: invalid character found!" << endl;
            exit(1);
          }
        }
        mymap[rowcounter][colcounter] = tempChar;
        colcounter++;
      }
      colcounter = 0;
      rowcounter++;
    }
  }
  if(!startfound){
    cerr << "Error: start has not been found!" << endl;
    exit(1);
  }
  if(!targetfound){
    cerr << "Error: target has not been found!" << endl;
    exit(1);
  }

  //Solving the map!
  Solver solver = Solver(N, height, width, stackOrQueue, startPos, mymap);
  deque<pair<char, pair<size_t, size_t>>> mypath;
  solver.findPath();
  if(solver.getSolution()){
    solver.makePath();
    mypath = solver.getPath();
  }

  //Output data
  if(!solver.getSolution()){//no solution
    cout << "No solution.\n";
    cout << "Discovered:\n";
    vector<vector<char>> noSolMap = solver.getNonDiscoveredMap();
    string row;
    for(size_t i = 0; i < height; ++i){
      for(size_t j = 0; j < width; ++j){
        row += noSolMap[i][j];
      }
      cout << row << '\n';
      row = "";
    }
  }
  else if(mapOrList){//map
  vector<vector<vector<char>>> mappath = solver.getMap();
  sort(mypath.begin(), mypath.end());
  string row;
  for(size_t i = 0; i < mappath.size(); ++i){
    cout << "// color " << validbuttons[i] << "\n";
    for(size_t r = 0; r < height; ++r){
      for(size_t c = 0; c < width; ++c){
        char tempChar = mymap[r][c];
        size_t b = validbuttons.find(tempChar);
        size_t d = validDoors.find(tempChar);
        //add something abt if mypath has stuff still
        if(!mypath.empty() && make_pair(validbuttons[i], make_pair(r, c)) == mypath.front()){//is on path
          if(b != string::npos){//is button
            char direc = mappath[i][r][c];
            if(direc == 'N' || direc == 'E' || direc == 'S' || direc == 'W'){//did not come from button
              if(validbuttons.find(mappath[b][r][c]) != string::npos)//led to new color
                row += '%';
              else if(i == b)
                row += '+'; //button in color is open floor
              else
                row += '.';
            }
            else if(validbuttons.find(mappath[i][r][c]) != string::npos)//came from button
              row += '@';
          }
          else if(d != string::npos){//is door
            if(i == d)
              row += '+';
            else
              row += tempChar;
          }
          else if(tempChar == '.' || (tempChar == '@' && i != 0 && make_pair(r, c) == startPos))//open floor and on path OR @ on map, but not in ^ color also on start bc other time its button stuff
            row += '+';
          else 
            row += tempChar;
          mypath.pop_front();
        }
        else{//not on sol path
          if(i == b || i == d)
            row += '.';
          else if(tempChar == '@' && i != 0 && make_pair(r, c) == startPos)
            row += '.';
          else
            row += tempChar;
        }
      }
      cout << row << '\n';
      row = "";
    }
  }
}
else{//list
  for(size_t i = 0; i < mypath.size(); ++i){
    pair<char, pair<size_t, size_t>> temp = mypath[i];
    cout << "(" << temp.first << ", (" << temp.second.first << ", " << temp.second.second << "))\n";
  }
}

  return 0;
}  // main()