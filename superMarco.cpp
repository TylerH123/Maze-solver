//Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque> 

using namespace std;

class rescue_countess {
private: 
    struct tile {
        char val; 
        bool discovered; 
        char direction; //which direction marco came from
    };
    struct loc {
        uint32_t room; 
        uint32_t row;
        uint32_t col; 
    };
    char input_mode;
    uint32_t room_size, room_numbers;
    string out_file = "M";
    bool stack = false;  
    bool found = false; 
    loc current, countess; 
    deque<loc> discovered_tile;
    deque<char> route; 
    uint32_t tiles_discovered = 0; 
    vector< vector< vector<tile> > > maze;

    bool isValidInput(const char &in, uint32_t room, uint32_t row, uint32_t col) {
        if (inBoundsInput(room,row,col)) {
            if (in == '.' || in == '#' || in == '!' || 
                in == 'S' || in == 'C' || isdigit(in)) 
                return true;
        }
        cerr << "Unknown map character" << endl;  
        exit(1);
        return false; 
    }
    
    bool inBoundsInput(uint32_t room, uint32_t row, uint32_t col) {
        if (room >= room_numbers) {
            cerr << "Invalid room number" << endl;  
            exit(1);
        }
        if (row >= room_size){
            cerr << "Invalid row number" << endl;  
            exit(1);
        }
        if (col >= room_size) {
            cerr << "Invalid column number" << endl;  
            exit(1);
        }
        return true; 
    }
    
    bool inBounds(uint32_t room, uint32_t row, uint32_t col) {
        return room < room_numbers && row < room_size && col < room_size; 
    }
    
    bool isValidTile(uint32_t room, uint32_t row, uint32_t col) {
        if (inBounds(room,row,col)) { //in bounds
            tile &curr = maze[room][row][col];
            if (curr.val == 'C') { //found countess
                found = true; 
                countess = loc{room,row,col}; 
                return true; 
            } // if 
            if (curr.val == '.' || isdigit(curr.val)) { //valid walking space
                if (curr.discovered == false) { //not discovered
                    return true; 
                } // if
            } // if 
        } // if
        return false; 
    }   

    void readToMap() {
        //reading first 3 lines of file
        string in; 
        getline(cin, in);
        input_mode = in[0];
        getline(cin, in); 
        room_numbers = stoi(in);
        getline(cin, in); 
        room_size = stoi(in);
        maze = vector< vector< vector<tile> > >
        (room_numbers, vector< vector<tile> >
        (room_size, vector<tile>(room_size, tile{'.', false, '0'}))); 
        uint32_t room = 0; 
        uint32_t row = 0; 
        while (getline(cin, in)) { 
            if (in[0] == '/'){
                if (row + 1 >= room_size) 
                    ++room; 
                row = 0; 
            } // if 
            else {
                if (input_mode == 'M') { //M input mode
                    parseMInput(in, room, row);
                } // if 
                else { //L input mode
                    parseLInput(in); 
                } // else
                ++row; 
            } // else
        } // while
    }
    
    void parseMInput(string &in, uint32_t room, uint32_t row){
        uint32_t col = 0; 
        for (char &c : in) {
            if (c !='\r') {
                if (isValidInput(c, room, row, col)) { 
                    if (c == 'S') {
                        current = loc{room, row, col}; 
                        maze[room][row][col] = tile{c, true, ' '};
                        ++tiles_discovered;
                    }
                    else 
                        maze[room][row][col] = tile{c, false, ' '};
                    ++col; 
                }
            }
        } // for
    }
    
    void parseLInput(string &in) {
        in = in.substr(1,in.length()-2); // remove parenthesis from front and back and the return char
        char tile_val;
        size_t pos = in.find(","); 
        vector<string> spliced_string; 
        spliced_string.reserve(4);
        for (int i = 0; i < 3; ++i) {
            pos = in.find(","); 
            if (pos == string::npos) {
                cerr << "Invalid coordinate input" << endl;
                exit(1); 
            }
            string split_string = in.substr(0, pos);
            spliced_string.push_back(split_string); 
            in.erase(0, pos + 1);
        }
        spliced_string.push_back(in);
        uint32_t room = static_cast<uint32_t>(stoul(spliced_string[0]));
        uint32_t row = static_cast<uint32_t>(stoul(spliced_string[1]));
        uint32_t col = static_cast<uint32_t>(stoul(spliced_string[2]));
        tile_val = spliced_string[3][0]; // getting the tile input
        if (isValidInput(tile_val, room, row, col)) {
            if (tile_val == 'S') {
                current = loc{room, row, col}; 
                maze[room][row][col] = tile{tile_val, true, ' '};
                ++tiles_discovered;
            } 
            else 
                maze[room][row][col] = tile{tile_val, false, ' '};
        }
    }
    
    void search() {
        discovered_tile.push_back(current); 
        while (!discovered_tile.empty()) {
            if (stack) {  
                current = discovered_tile.back();
                discovered_tile.pop_back();
            }
            else {
                current = discovered_tile.front(); 
                discovered_tile.pop_front(); 
            }
            uint32_t room = current.room; 
            uint32_t row = current.row; 
            uint32_t col = current.col;
            char val = (maze[room][row][col]).val;
            if (isdigit(val)) { //if pipe
                room = static_cast<uint32_t>(val - '0'); 
                if (isValidTile(room, row, col)) {
                    tile &curr = maze[room][row][col]; //new room
                    discovered_tile.push_back(loc{room,row,col}); 
                    curr.discovered = true;  
                    curr.direction = static_cast<char>(current.room + '0'); 
                    ++tiles_discovered;
                } // if 
            } // if 
            else {
                if (isValidTile(room, row-1, col)) { //check north
                    tile &curr = maze[room][row-1][col];
                    discovered_tile.push_back(loc{room,row-1,col});
                    curr.discovered = true; 
                    curr.direction = 'n'; 
                    ++tiles_discovered;
                    if (found) {
                        break; 
                    }
                }
                if (isValidTile(room, row, col+1)) { //check east
                    tile &curr = maze[room][row][col+1];
                    discovered_tile.push_back(loc{room,row,col+1}); 
                    curr.discovered = true; 
                    curr.direction = 'e'; 
                    ++tiles_discovered;
                    if (found) {
                        break;
                    }
                }
                if (isValidTile(room, row+1, col)) { //check south
                    tile &curr = maze[room][row+1][col];
                    discovered_tile.push_back(loc{room,row+1,col}); 
                    curr.discovered = true; 
                    curr.direction = 's'; 
                    ++tiles_discovered;
                    if (found) {
                        break;
                    }
                }
                if (isValidTile(room, row, col-1)) { //check west
                    tile &curr = maze[room][row][col-1];
                    discovered_tile.push_back(loc{room,row,col-1}); 
                    curr.discovered = true; 
                    curr.direction = 'w'; 
                    ++tiles_discovered;
                    if (found) {
                        break;
                    }
                }
            } // else
        }
    }

    void backTrace() {
        current = countess; 
        found = false; 
        while (!found) {
            char dir = (maze[current.room][current.row][current.col]).direction;
            if (dir == 'n') {
                tile &next_tile = maze[current.room][++current.row][current.col];
                if (next_tile.val == 'S') 
                    found = true; 
                if (out_file == "M") 
                    next_tile.val = 'n';
                route.push_back('n');
            }
            else if (dir == 'e') {
                tile &next_tile = maze[current.room][current.row][--current.col];
                if (next_tile.val == 'S') 
                    found = true;
                if (out_file == "M") 
                    next_tile.val = 'e';
                route.push_back('e');
            }
            else if (dir == 's') {
                tile &next_tile = maze[current.room][--current.row][current.col];
                if (next_tile.val == 'S') 
                    found = true; 
                if (out_file == "M") 
                    next_tile.val = 's';
                route.push_back('s');
            }
            else if (dir == 'w') {
                tile &next_tile = maze[current.room][current.row][++current.col];
                if (next_tile.val == 'S') 
                    found = true; 
                if (out_file == "M") 
                    next_tile.val = 'w';
                route.push_back('w');
            }
            else if (isdigit(dir)) { //pipe
                current.room = static_cast<uint32_t>(dir-'0'); 
                tile &next_tile = maze[current.room][current.row][current.col];
                if (next_tile.val == 'S') 
                    found = true; 
                if (out_file == "M") 
                    next_tile.val = 'p';
                route.push_back('p');
            }
        } // while
    } 
    
    void printSolution() {
        if (out_file == "M") {
            cout << "Start in room " << current.room << ", row " << current.row 
                << ", column " << current.col << "\n";
            vecToString(); 
        } // if
        else {
            cout << "Path taken:" << "\n"; 
            while (!route.empty()) {
                char dir = route.back(); 
                cout << "(" << current.room << "," << current.row << "," 
                    << current.col << "," << dir << ")\n";
                if (dir == 'n') {
                    --current.row; 
                }
                else if (dir == 's') {
                    ++current.row; 
                }
                else if (dir == 'e') {
                    ++current.col; 
                }
                else if (dir == 'w') {
                    --current.col; 
                }
                else if (dir == 'p') {
                    current.room = static_cast<uint32_t>
                                   (maze[current.room][current.row][current.col].val - '0'); 
                }
                route.pop_back(); 
            }
        } // else
    }
    
    void printNoSolution() {
        cout << "No solution, " << tiles_discovered << " tiles discovered." << "\n";
    }

    void vecToString() {
        uint32_t room_num = 0; 
        for( auto& r : maze ) {
            cout << "//castle room " << room_num << "\n";
            for( auto &row : r ) {
                for ( auto &ele : row) {
                    cout << ele.val; 
                }
                cout << "\n"; 
            }
            ++room_num;
        }
    }
    
    void vecDirToString() {
        uint32_t room_num = 0; 
        for( auto& r : maze ) {
            cout << "//castle room " << room_num << "\n";
            for( auto &row : r ) {
                for ( auto &ele : row) {
                    cout << ele.direction; 
                }
                cout << "\n"; 
            }
            ++room_num;
        }
    }    
    
public:
    rescue_countess(int argc, char* argv[]) {
        getMode(argc, argv);
        readToMap(); 
        search(); 
        if (found) {
            backTrace();
            printSolution(); 
        }
        else {
            printNoSolution(); 
        }
    }  

    // Print help for the user when requested.
    // argv[0] is the name of the currently executing program
    void printHelp(char *argv[]) {
        cout << "This program is used to rescue the countess" << endl;
        cout << "in a maze." << endl;
        cout << "Usage: " << argv[0] << " -s|-q|[-o M|L]|-h" << endl;
        cout << "Use -s for stack-based routing scheme." << endl;
        cout << "Use -q for queue-based routing scheme." << endl;
        cout << "Use -o to indicate output file format." << endl;
        cout << "Output file format is optional." << endl;
        cout << "Must include M or L if -o is specified." << endl;
    } // printHelp()

    void getMode(int argc, char * argv[]) {
        bool modeSpecified = false;
        // These are used with getopt_long()
        opterr = false; // Let us handle all error output for command line options
        int choice;
        int count = 0;
        int option_index = 0;
        option long_options[] = {
            // Fill in two lines, for the "mode" ('m') and
            // the "help" ('h') options.
            { "stack", no_argument,        nullptr, 's'},
            { "queue", no_argument,        nullptr, 'q'},
            { "output", required_argument, nullptr, 'o'},
            { "help",  no_argument,        nullptr, 'h'},
            { nullptr, 0,                  nullptr, '\0' }
        };
        // cout << getopt_long(argc, argv, "m:h", long_options, &option_index) << endl; 
        // Fill in the double quotes, to match the mode and help options.
        while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &option_index)) != -1) {
            switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);

            case 's':
                modeSpecified = true;
                stack = true; 
                ++count; 
                break;

            case 'q':
                modeSpecified = true;
                ++count; 
                break;

            case 'o':
                out_file = optarg;
                if (out_file != "M" && out_file != "L") {
                    // The first line of error output has to be a 'fixed' message for the autograder
                    // to show it to you.
                    cerr << "Error: invalid output file format" << endl;
                    // The second line can provide more information, but you won't see it on the AG.
                    cerr << "  I don't know recognize: " << out_file << endl;
                    exit(1);
                } // if
                break;

            default:
                cerr << "Unknown command line option" << endl;
                exit(1);
            } // switch
        } // while
        if (count > 1) {
            cerr << "Stack or queue can only be specified once" << endl; 
            exit(1); 
        }
        if (!modeSpecified) {
            cerr << "Stack or queue must be specified" << endl;
            exit(1);
        } // if
    } // getMode()

};

// This function is already done.
int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);
    rescue_countess res = rescue_countess(argc, argv); 
    return 0;
} // main()
