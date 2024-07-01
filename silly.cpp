// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D

#include "TableEntry.h"
#include <functional>
#include <cassert>
#include <iostream>
#include <utility>
#include <exception>
#include <algorithm>
#include <deque>
#include <iterator>
#include <limits>
#include <list>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>
#include <getopt.h>
#include <unordered_map>
#include <map>

using namespace std;

// For input into table
enum class TableType {
    STRING,
    DOUBLE,
    INT,
    BOOL
};

// For finding rows with a greater value in a column
struct greaterFunctor {

    size_t column;
    TableEntry compareTo;

    bool operator()(const vector<TableEntry>& a) const {
        return a[column] > compareTo;
    }
};

// For finding rows with a lower value in a column
struct lessFunctor {

    size_t column;
    TableEntry compareTo;

    bool operator()(const vector<TableEntry>& a) const {
        return a[column] < compareTo;
    }
};

// For finding rows with an equivalent value in a column
struct equalFunctor {

    size_t column;
    TableEntry compareTo;

    bool operator()(const vector<TableEntry>& a) const {
        return a[column] == compareTo;
    }
};

// Keeps track of a table
struct Table {
    vector<vector<TableEntry>> entries;
    vector<pair<TableType, string>> colNames;
    unordered_map<TableEntry, vector<size_t>> hash;
    multimap<TableEntry, size_t> bst;
    
    int indexCol;
    bool isBst;
};

// All tables
class Nincompoop {

    private:
    // Name to table
    unordered_map<string, Table> ht;

    // For when joi is used
    unordered_map<TableEntry, vector<size_t>> joinHash;

    vector<pair<size_t, bool>> joinCols;

    vector<size_t> colsUsed;

    // Various purposes throuhgout
    string cmd;
    string word;
    string type;
    string readString;
    string name;
    string gross;
    string name2;
    string type2;

    double readDouble;
    int readInt;
    size_t amount;
    size_t whichTable;
    size_t columnIn = 0;
    size_t columnIn2 = 0;
    size_t rowsPrinted;

    char compare;
    

    bool isQuiet = false;
    bool readBool;
    bool isPrint;


    public:

    // Take in arguments
    void readInTheArgs(int argc, char** argv) {
        int option_index = 0, option = 0;
    
        opterr = false;

        struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "quiet", no_argument, nullptr, 'q'}};

        while 
        ((option =
        getopt_long(argc, argv, "hq", longOpts, &option_index)) != -1) {
            switch (option) {
                case 'h':
                    cout << "You need help hoe" << endl;
                    exit(0);

                case 'q':
                    isQuiet = true;
                    break;
            }
        }
    }

    // Create a table
    void create() {
        cin >> word;
        auto dude = ht.find(word);
        if (dude != ht.end()) {
            cout << "Error during CREATE: Cannot create already existing table " 
            << word << '\n';
            getline(cin, gross);
            return;
        }

        cin >> amount;
        vector<pair<TableType, string>> columns(amount);
        // Read in table elements
        for (size_t i = 0; i < amount; i++) {
            cin >> type;
            if (type == "int") {
                columns[i].first = TableType::INT;
            }
            else if (type == "double") {
                columns[i].first = TableType::DOUBLE;
            }
            else if (type == "bool") {
                columns[i].first = TableType::BOOL;
            }
            else {
                columns[i].first = TableType::STRING;
            }
        }

        cout << "New table " << word << " with column(s) ";

        for (size_t i = 0; i < amount; i++) {
            cin >> type;
            cout << type << " ";
            columns[i].second = type;
        }

        cout << "created\n";

        vector<vector<TableEntry>> entries;
        // Place table into hash
        ht[word] = {entries, columns, {}, {}, -1, false};

    }
        

    // For when the game ends
    void quit() {
        auto it = ht.begin();
        while (it != ht.end()) {
            auto currentIt = it; 
            ++it;  
            killTable(currentIt);
        }

        std::cout << "Thanks for being silly!" << std::endl;
    }

    // Helper for deleting a table
    void killTable(unordered_map<string, Table>::iterator &it) {
        it->second.entries.clear();
        it->second.colNames.clear();
        it->second.bst.clear();
        it->second.hash.clear();

        it->second.entries.shrink_to_fit();
        it->second.colNames.shrink_to_fit();
        ht.erase(it);
    }

    // Helper for finding a table
    bool lookForTable(unordered_map<string, Table>::iterator &it, const string &move,
                      const string &wordChose) {
        if (it == ht.end()) {
            cout << "Error during "<< move << ": " << wordChose << 
            " does not name a table in the database\n";
            getline(cin, gross);
            return true;
        }
        return false;
    }

    // Removes a table
    void remove() {
        cin >> word;
        auto dude = ht.find(word);
        // Make sure table exists
        if (lookForTable(dude, "REMOVE", word)) return;

        killTable(dude);

        cout << "Table " << word << " removed\n";
    }

    // For adding rows to table
    void insert() {
        cin >> type >> word;
        auto dude = ht.find(word);
        if (lookForTable(dude, "INSERT", word)) return;

        cin >> amount >> type;

        cout << "Added " << amount << " rows to " << word << " from position " <<
             dude->second.entries.size() << " to " 
             << amount + dude->second.entries.size() - 1 << '\n';

        size_t guy = dude->second.entries.size();
        
        dude->second.entries.resize(amount + guy);

        for (size_t i = guy; i < amount + guy; i++) {
            dude->second.entries[i].reserve(dude->second.colNames.size());
            for (size_t j = 0; j < dude->second.colNames.size(); j++) {
                switch (dude->second.colNames[j].first) {
                    case TableType::INT: {
                        cin >> readInt;
                        dude->second.entries[i].emplace_back(readInt);
                        break;
                    }
                    case TableType::BOOL: {
                        cin >> readString;
                        readBool = (readString == "true" ? true : false);
                        dude->second.entries[i].emplace_back(readBool);
                        break;
                    }
                    case TableType::DOUBLE: {
                        cin >> readDouble;
                        dude->second.entries[i].emplace_back(readDouble);
                        break;
                    }
                    case TableType::STRING: {
                        cin >> readString;
                        dude->second.entries[i].emplace_back(readString);
                        break;
                    }
                }    
            }
            // Check for index
            if (dude->second.indexCol != -1) {
                auto entry = 
                dude->second.entries[i][static_cast<size_t>(dude->second.indexCol)];
                if (dude->second.isBst) dude->second.bst.insert({entry, i});
                else dude->second.hash[entry].push_back(i);
            }
        }
    }

    // If a column does not exist
    void notACol(const string &move, const string &colName, const string &tableName) {
        cout << "Error during " << move << ": " << colName << 
             " does not name a column in " << tableName << '\n';
        getline(cin, gross);
    }

    // Printing applicable elements
    template<typename Functor>
    void basicPrint(const Functor& functor, unordered_map<string, Table>::iterator &it) {
        for (const auto& row : it->second.entries) {
            if (functor(row)) {
                if (!isQuiet) {
                    for (size_t colIndex : colsUsed) {
                        if (it->second.colNames[colIndex].first != TableType::BOOL) {
                            cout << row[colIndex];
                        } else if (row[colIndex] == true) {
                            cout << "true";
                        } else cout << "false";
                        cout << " ";
                    }
                    cout << '\n';
                }
            } else {
                rowsPrinted--;
            }
        }
           
    }

    
    // For deleting certain rows
    template<typename Functor>
    void deleteStuff(const Functor& functor, 
                          unordered_map<string, Table>::iterator &it) {

        auto removeBegin = remove_if(it->second.entries.begin(),
                                     it->second.entries.end(), functor);
        cout << "Deleted " << distance(removeBegin, it->second.entries.end()) << 
             " rows from " << word << '\n';

        it->second.entries.erase(removeBegin, it->second.entries.end());     
    }

    // Finding rows that are greater
    void greaterBst(const TableEntry& toCompare, 
                    unordered_map<string, Table>::iterator &it) {
        auto search = it->second.bst.upper_bound(toCompare);
        rowsPrinted = 0;
        for (; search != it->second.bst.end(); ++search) {
            rowsPrinted++;
            if (!isQuiet) {
                for (size_t colIndex : colsUsed) {
                    if (it->second.colNames[colIndex].first != TableType::BOOL) {
                        cout << it->second.entries[search->second][colIndex];
                    } else if (it->second.entries[search->second][colIndex] == true) {
                        cout << "true";
                    } else cout << "false";
                    cout << " ";
                }
                cout << '\n';
            }
        }
    }

    // Finding rows that are less than
    void lessBst(const TableEntry& toCompare, 
                    unordered_map<string, Table>::iterator &it) {
        auto search = it->second.bst.begin();
        rowsPrinted = 0;
        for (; search != it->second.bst.end() && search->first < toCompare; search++) {
            rowsPrinted++;
            if (!isQuiet) {
                for (size_t colIndex : colsUsed) {
                    if (it->second.colNames[colIndex].first != TableType::BOOL) {
                        cout << it->second.entries[search->second][colIndex];
                    } else if (it->second.entries[search->second][colIndex] == true) {
                        cout << "true";
                    } else cout << "false";
                    cout << " ";
                }
                cout << '\n';
            }
        }
    }

    // Finding rows that are equal with bst index
    void equalBst(const TableEntry& toCompare, 
                    unordered_map<string, Table>::iterator &it) {
        auto search = it->second.bst.find(toCompare);
        rowsPrinted = 0;
        for (; search != it->second.bst.end() && search->first == toCompare; search++) {
            rowsPrinted++;
            if (!isQuiet) {
                for (size_t colIndex : colsUsed) {
                    if (it->second.colNames[colIndex].first != TableType::BOOL) {
                        cout << it->second.entries[search->second][colIndex];
                    } else if (it->second.entries[search->second][colIndex] == true) {
                        cout << "true";
                    } else cout << "false";
                    cout << " ";
                }
                cout << '\n';
            }
        }
    }

    // Finding rows that are equal with hash index
    void equalHash(const TableEntry& toCompare, unordered_map<string, Table>::iterator& it) {
        auto search = it->second.hash.find(toCompare);
        rowsPrinted = 0;
        if (search != it->second.hash.end()) {
            for (size_t index : search->second) {
                rowsPrinted++;
                if (!isQuiet) {
                    for (size_t colIndex : colsUsed) {
                        if (it->second.colNames[colIndex].first != TableType::BOOL) {
                            cout << it->second.entries[index][colIndex];
                        } else if (it->second.entries[index][colIndex] == true) {
                            cout << "true";
                        } else {
                            cout << "false";
                        }
                        cout << " ";
                    }
                    cout << '\n';
                }   
            }
        }
    }

    // Deleting greater rows
    void greaterBstDelete(const TableEntry& toCompare,
                          unordered_map<string, Table>::iterator& it) {
        size_t deletedCount = 0;

        auto search = it->second.bst.upper_bound(toCompare);
        

        while (search != it->second.bst.end()) {
            colsUsed.push_back(search->second);
            search++;
            deletedCount++;
        }

        sort(colsUsed.rbegin(), colsUsed.rend());

        for (auto index : colsUsed) {
            it->second.entries.erase(it->second.entries.begin() + 
            static_cast<std::vector<TableEntry>::difference_type>(index));
        }

        colsUsed.clear();
        colsUsed.shrink_to_fit();

        cout << "Deleted " << deletedCount << " rows from " << it->first << '\n';
    }

    // Deleting less than rows
    void lessBstDelete(const TableEntry& toCompare,
                     unordered_map<string, Table>::iterator& it) {
        size_t deletedCount = 0;

        auto search = it->second.bst.begin();

        while (search != it->second.bst.end() && search->first < toCompare) {
            colsUsed.push_back(search->second);
            deletedCount++;
            search++;
        }

        sort(colsUsed.rbegin(), colsUsed.rend());

        for (auto index : colsUsed) {
            it->second.entries.erase(it->second.entries.begin() + 
            static_cast<std::vector<TableEntry>::difference_type>(index));
        }

        colsUsed.clear();
        colsUsed.shrink_to_fit();

        cout << "Deleted " << deletedCount << " rows from " << it->first << '\n';
    }

    // Deleting equal to rows with bst index
    void equalBstDelete(const TableEntry& toCompare,
                         unordered_map<string, Table>::iterator& it) {
        size_t deletedCount = 0;

        auto search = it->second.bst.find(toCompare);
    
        while (search != it->second.bst.end() && search->first == toCompare) {
            colsUsed.push_back(search->second);
            deletedCount++;
            search++;
        }

        sort(colsUsed.rbegin(), colsUsed.rend());

        for (auto index : colsUsed) {
            it->second.entries.erase(it->second.entries.begin() + 
                static_cast<std::vector<TableEntry>::difference_type>(index));
        }

        colsUsed.clear();
        colsUsed.shrink_to_fit();

        cout << "Deleted " << deletedCount << " rows from " << it->first << '\n';
    }

    // Deleting equal to rows with hash
    void deleteEqualHash(const TableEntry& toCompare, unordered_map<string, Table>::iterator& it) {
        auto search = it->second.hash.find(toCompare);
        size_t deletedCount = 0;

        if (search != it->second.hash.end()) {
            for (size_t index : search->second) {
                colsUsed.push_back(index);
                deletedCount++;
            }
        }

        sort(colsUsed.rbegin(), colsUsed.rend());

        for (auto index : colsUsed) {
            it->second.entries.erase(it->second.entries.begin() + 
                static_cast<std::vector<TableEntry>::difference_type>(index));
        }

        colsUsed.clear();
        colsUsed.shrink_to_fit();

        cout << "Deleted " << deletedCount << " rows from " << it->first << '\n';
    }



    // Calling necessary function based on symbol on index
    template<typename T>
    void compareThem(const T& makeEntry, unordered_map<string, Table>::iterator &it) {
        TableEntry toCompare{makeEntry};
        switch (compare) {
            case '>': {
                greaterFunctor greater = {columnIn, toCompare};
                if (isPrint) {
                    if (it->second.indexCol == static_cast<int>(columnIn) &&
                        it->second.isBst) {
                        greaterBst(toCompare, it);
                    }
                    else basicPrint(greater, it);    
                } else if (it->second.indexCol == static_cast<int>(columnIn) &&
                           it->second.isBst) {
                    greaterBstDelete(toCompare, it);
                } else deleteStuff(greater, it);
                break;
            }
            case '<': {
                lessFunctor less = {columnIn, toCompare};
                if (isPrint) {
                    if (it->second.indexCol == static_cast<int>(columnIn) &&
                        it->second.isBst) {
                        lessBst(toCompare, it);
                    }
                    else basicPrint(less, it); 
                } else if (it->second.indexCol == static_cast<int>(columnIn) &&
                           it->second.isBst) {
                    lessBstDelete(toCompare, it);       
                } else { 
                    deleteStuff(less, it);
                }
                break;
            } 
            default: {
                equalFunctor equal = {columnIn, toCompare};
                if (isPrint) {
                    if (it->second.indexCol == static_cast<int>(columnIn)) {
                        if (it->second.isBst) {
                            equalBst(toCompare, it);
                        } else equalHash(toCompare, it);
                    }
                    else basicPrint(equal, it); 
                } else if (it->second.indexCol == static_cast<int>(columnIn)) {
                    if (it->second.isBst) equalBstDelete(toCompare, it);
                    else deleteEqualHash(toCompare, it);
                         
                } else deleteStuff(equal, it);
                break;
            } 
        }
    }
    
    // Finding type of Table Type
    void findWhere(unordered_map<string, Table>::iterator &it) {
        switch (it->second.colNames[columnIn].first) {
            case TableType::INT: {
                cin >> readInt;
                compareThem(readInt, it);
                break;
            }
            case TableType::BOOL: {
                cin >> readString;
                readBool = (readString == "true" ? true : false);
                compareThem(readBool, it);
                break;
            }
            case TableType::DOUBLE: {
                cin >> readDouble;
                compareThem(readDouble, it);
                break;
            }
            case TableType::STRING: {
                cin >> readString;
                compareThem(readString, it);
                break;
            }
        }
    }

    // Printing rows based on input
    void print() {
        cin >> type >> word;
        auto dude = ht.find(word);
        if (lookForTable(dude, "PRINT", word)) return;

        cin >> amount;

        colsUsed.reserve(amount);

        for (size_t i = 0; i < amount; i++) {
            cin >> type;
            size_t j = 0;
            while (true) {
                if (type == dude->second.colNames[j].second) {
                    colsUsed.push_back(j);
                    break;
                } else if (j == dude->second.colNames.size() - 1) {
                    notACol("PRINT", type, word);
                    colsUsed.clear();
                    colsUsed.shrink_to_fit();
                    return;
                }
                j++;
            }
        }

        cin >> type;

        rowsPrinted = dude->second.entries.size();

        if (type[0] == 'W') {
            cin >> name >> compare;
            columnIn = 0;
            while (true) {
                if (name == dude->second.colNames[columnIn].second) {
                    break;
                } else if (columnIn == dude->second.colNames.size() - 1) {
                    notACol("PRINT", name, word);
                    colsUsed.clear();
                    colsUsed.shrink_to_fit();
                    return;
                }
                columnIn++;
            }
            if (!isQuiet) {
            
                for (size_t colIndex : colsUsed) {
                    cout << dude->second.colNames[colIndex].second << " ";
                }
                cout << '\n';
            }
            isPrint = true;
            findWhere(dude);
        }


        if (!isQuiet && type[0] != 'W') {
            for (size_t colIndex : colsUsed) {
                cout << dude->second.colNames[colIndex].second << " ";
            }
            cout << '\n';
            for (const auto& row : dude->second.entries) {
                for (size_t colIndex : colsUsed) {
                    if (dude->second.colNames[colIndex].first != TableType::BOOL) {
                        cout << row[colIndex];
                    } else if (row[colIndex] == true) {
                        cout << "true";
                    } else cout << "false";
                    cout << " ";
                }
                cout << '\n';
            }
        }

        colsUsed.clear();
        colsUsed.shrink_to_fit();

        cout << "Printed " << rowsPrinted << " matching rows from " << word << '\n';


    }

    // Deleting rows based on input
    void deleteRows() {
        cin >> type >> word;
        auto dude = ht.find(word);
        if (lookForTable(dude, "DELETE", word)) return;

        cin >> type >> type >> compare;

        columnIn = 0;
        while (true) {
            if (type == dude->second.colNames[columnIn].second) {
                break;
            } else if (columnIn == dude->second.colNames.size() - 1) {
                notACol("DELETE", type, word);
                return;
            }
            columnIn++;
        }
        isPrint = false;
        findWhere(dude);
        
        if (dude->second.indexCol != -1) {
            if (dude->second.isBst) {
                dude->second.bst.clear();
                for (size_t i = 0; i < dude->second.entries.size(); ++i) {
                    auto entry = 
                    dude->second.entries[i][static_cast<size_t>(dude->second.indexCol)];
                    dude->second.bst.insert({entry, i});
                }
            } else {
                dude->second.hash.clear();
                for (size_t i = 0; i < dude->second.entries.size(); ++i) {
                    auto entry = 
                    dude->second.entries[i][static_cast<size_t>(dude->second.indexCol)];
                    dude->second.hash[entry].push_back(i);
                }
            }
        }
    }

    // Function for joining tables
    void join() {
        cin >> name;
        auto dude = ht.find(name);
        if (lookForTable(dude, "JOIN", name)) return;

        cin >> type >> name2;
        auto dude2 = ht.find(name2);
        if (lookForTable(dude2, "JOIN", name2)) return;

        cin >> type >> type;

        columnIn = 0;
        while (true) {
            if (type == dude->second.colNames[columnIn].second) {
                break;
            } else if (columnIn == dude->second.colNames.size() - 1) {
                notACol("JOIN", type, name);
                return;
            }
            columnIn++;
        }

        cin >> compare >> type2;

        columnIn2 = 0;
        while (true) {
            if (type2 == dude2->second.colNames[columnIn2].second) {
                break;
            } else if (columnIn2 == dude2->second.colNames.size() - 1) {
                notACol("JOIN", type2, name2);
                return;
            }
            columnIn2++;
        }

        cin >> gross >> gross >> amount;

        if (!isQuiet) joinCols.reserve(amount);
        rowsPrinted = 0;

        for (size_t i = 0; i < amount; i++) {
            cin >> word >> whichTable;
            if (whichTable == 1) {
                size_t j = 0;
                while (true) {
                    if (word == dude->second.colNames[j].second) {
                        if (!isQuiet) joinCols.push_back({j, true});
                        break;
                    } else if (j == dude->second.colNames.size() - 1) {
                        notACol("JOIN", word, name);
                        joinCols.clear();
                        joinCols.shrink_to_fit();
                        return;
                    }
                    j++;
                }   
            } else {
                size_t j = 0;
                while (true) {
                    if (word == dude2->second.colNames[j].second) {
                        if (!isQuiet) joinCols.push_back({j, false});
                        break;
                    } else if (j == dude2->second.colNames.size() - 1) {
                        notACol("JOIN", word, name2);
                        joinCols.clear();
                        joinCols.shrink_to_fit();
                        return;
                    }
                    j++;
                }   
            }
        }

        if (!isQuiet) {
            for (auto pair : joinCols) {
                if (pair.second) {
                    cout << dude->second.colNames[pair.first].second;
                } else {
                    cout << dude2->second.colNames[pair.first].second;
                }
                cout << " ";
            }
            cout << '\n';
        }
        

        // Make an index for efficient join
        if (dude->second.colNames[columnIn].first == 
            dude2->second.colNames[columnIn2].first) {   

            if (dude2->second.indexCol != static_cast<int>(columnIn2)) {
                for (size_t i = 0; i < dude2->second.entries.size(); ++i) {
                    auto entry = dude2->second.entries[i][static_cast<size_t>(columnIn2)];
                    joinHash[entry].push_back(i);
                } 
            }     

            for (size_t i = 0; i < dude->second.entries.size(); ++i) {
                if (dude2->second.indexCol != static_cast<int>(columnIn2)) {
                    auto search = joinHash.find(dude->second.entries[i][columnIn]);
                    if (search != dude2->second.hash.end()) {
                        for (size_t index : search->second) {
                            if (!isQuiet) joinOutput(i, index, dude, dude2);
                            rowsPrinted++;
                        }
                    }
                } else if (dude2->second.isBst) {
                    auto search = 
                    dude2->second.bst.find(dude->second.entries[i][columnIn]);
                    while (search != dude2->second.bst.end() &&
                           dude->second.entries[i][columnIn] == search->first) {
                        if (!isQuiet) joinOutput(i, search->second, dude, dude2);
                        rowsPrinted++;
                        search++;
                    }
                } else {
                    auto search = 
                    dude2->second.hash.find(dude->second.entries[i][columnIn]);
                    if (search != dude2->second.hash.end()) {
                        for (size_t index : search->second) {
                            if (!isQuiet) joinOutput(i, index, dude, dude2);
                            rowsPrinted++;
                        }
                    }
                }
            }
        }
        
            

        cout << "Printed " << rowsPrinted << " rows from joining " <<
             name << " to " << name2 << '\n';


        joinCols.clear();
        joinCols.shrink_to_fit();
        joinHash.clear();

    }

    // Outputting when joining
    void joinOutput(const size_t& number1, const size_t& number2,
                    unordered_map<string, Table>::iterator& dude,
                    unordered_map<string, Table>::iterator& dude2) {
        for (auto pair : joinCols) {
            if (pair.second) {
                if (dude->second.colNames[pair.first].first != TableType::BOOL) {
                    cout << dude->second.entries[number1][pair.first];
                } else if (dude->second.entries[number1][pair.first] == true) {
                    cout << "true";
                } else cout << "false";
            } else {
                if (dude2->second.colNames[pair.first].first != TableType::BOOL) {
                    cout << dude2->second.entries[number2][pair.first];
                } else if (dude2->second.entries[number2][pair.first] == true) {
                    cout << "true";
                } else cout << "false";
            }
            cout << " ";
        }
        cout << '\n';
    }

    // Generating an index
    void generate() {
        cin >> type >> name;
        auto dude = ht.find(name);
        if (lookForTable(dude, "GENERATE", name)) return;

        cin >> readString >> gross >> gross >> type;

        columnIn = 0;
        while (true) {
            if (type == dude->second.colNames[columnIn].second) {
                break;
            } else if (columnIn == dude->second.colNames.size() - 1) {
                notACol("GENERATE", type, name);
                return;
            }
            columnIn++;
        }

        cout << "Created " << readString << " index for table " << name << " on column "
             << type << ", with ";

        // Make sure there isn't another index
        if (dude->second.indexCol != -1) {
            if (dude->second.isBst) dude->second.bst.clear();
            else dude->second.hash.clear();
        }

        dude->second.indexCol = static_cast<int>(columnIn);
        if (readString.at(0) == 'b')  {
            dude->second.isBst = true;
            size_t distinctKeysCount = 0;
            for (size_t i = 0; i < dude->second.entries.size(); ++i) {
                auto entry = dude->second.entries[i][static_cast<size_t>(columnIn)];
                if (dude->second.bst.find(entry) == dude->second.bst.end()) {
                    ++distinctKeysCount;
                }
                dude->second.bst.insert({entry, i});
            }
            cout << distinctKeysCount;
        }
        else {
            dude->second.isBst = false;
            for (size_t i = 0; i < dude->second.entries.size(); ++i) {
                auto entry = dude->second.entries[i][static_cast<size_t>(columnIn)];
                dude->second.hash[entry].push_back(i);
            }
            cout << dude->second.hash.size();
        }

        cout << " distinct keys\n";

    }

    // Take input
    void eatInput() {

        do {
            cout << "% ";
            cin >> cmd;

            switch(cmd.at(0)) {
                case 'C':
                    create();
                    break;
                case 'Q':
                    quit();
                    break;
                case '#':
                    getline(std::cin, word);
                    break;
                case 'R':
                    remove();
                    break;
                case 'I':
                    insert();
                    break;
                case 'P':
                    print();
                    break;
                case 'D':
                    deleteRows();
                    break;
                case 'J':
                    join();
                    break;
                case 'G':
                    generate();
                    break;
                default:
                    getline(cin, gross);
                    cout << "Error: unrecognized command\n";
            }


         } while (cmd.at(0) != 'Q');

    }


};


int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);

    Nincompoop dimwit;

    dimwit.readInTheArgs(argc, argv);

    dimwit.eatInput();

    return 0;
}

