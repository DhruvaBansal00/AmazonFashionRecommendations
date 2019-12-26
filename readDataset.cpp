#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include "readDataset.h"

using namespace std;
namespace fs = experimental::filesystem;

void print_map(unordered_map<string, unordered_map<string, string>> map) {
    for (auto const& pair : map) {
        cout << "User = " << pair.first << "\n";
        for (auto const& pair2 : pair.second) {
            cout << "\t" << pair2.first << " : " << pair2.second << "\n";
        }
    }
}

template <typename T>
void print_vec(vector<T> x) {
    for (T s : x) {
        cout << s << " ";
    }
    cout <<"\n";
}

ReadData::ReadData() {
    string ratings = "../Datasets/Ratings";
    string metadata = "../Datasets/Metadata";
    id_to_product = new unordered_map<string, string>;
    user_to_item_rating = new unordered_map<string, unordered_map<string, string>*>;
    id_to_category_vector = new unordered_map<string, vector<int>*>;
    id_to_category_rank = new unordered_map<string, int>; 
    user_ratings(ratings);
    id_stats(metadata);
}

ReadData::~ReadData() {
    cout << "Destructor called \n";
    delete id_to_product;
    for (auto const & entry : *user_to_item_rating) {
        delete entry.second;
    }
    delete user_to_item_rating;
    for (auto const & entry : *id_to_category_vector) {
        delete entry.second;
    }
    delete id_to_category_vector;
    delete id_to_category_rank;
}


void ReadData::user_ratings(string ratings) {
    for (const auto & entry : fs::directory_iterator(ratings)) {
        cout << "Starting " << entry.path() << "\n";
        ifstream infile(entry.path());
        vector<string> line;
        string temp, strline, word;
        while (getline(infile, strline, '\n')) {
            istringstream br(strline);
            while (getline(br, word, ',')) {
                line.push_back(word);
            }
            if ((*user_to_item_rating).count(line[0]) == 0) {
                (*user_to_item_rating)[line[0]] = new unordered_map<string, string>;
            }
            (*(*user_to_item_rating)[line[0]])[line[1]] = line[2];
            line.clear();
        }
        cout << "Total number of Users: " << (*user_to_item_rating).size() << "\n";
    }
}
//Malformed JSONs in dataset. 
//Extracting only neccessary data from each line.

string get_asin(string line) {
    size_t index = line.find("asin");
    int i = (int)index + 8;
    if (index >= line.size()) {return "";}
    stringstream s;
    while (line[i] != '\'') {
        s << line[i];
        i++;
    }
    return s.str();
}

string get_title(string line) {
    size_t index = line.find("title");
    int i = (int)index + 9;
    if (index >= line.size()) {return "";}
    stringstream s;
    while (line[i] != '\'') {
        s << line[i];
        i++;
    }
    return s.str();
}

vector<string> get_categories(string line) {
    size_t index = line.find("\'categories\':");
    vector<string> categories;
    if (index >= line.size()) {return categories;}
    int i = (int)index + 15;
    stringstream s;
    while (line[i] != ']') {
        if (line[i] == ',')  {
            categories.push_back(s.str());
            s.str("");
            i++;
        }
        if (line[i] != '\'' && line[i] != '[' && line[i] != ',' && line[i] != ' ')
            s << line[i];
        i++;
    }
    categories.push_back(s.str());
    return categories;
}

vector<string> get_rank(string line) {
    size_t index = line.find("salesRank");
    int i = (int)index + 13;
    stringstream s;
    vector<string> cells;
    if (index >= line.size()) {return cells;}
    while (line[i] != '}') {
        if (line[i] == ':')  {
            cells.push_back(s.str());
            s.str("");
            i++;
        }
        if (line[i] != '\'' && line[i] != '[' && line[i] != ',')
            s << line[i];
        i++;
    }
    if (s.str().size() > 0) {
        cells.push_back(s.str());
    }
    return cells;
}


void ReadData::id_stats(string metadata) {
    int curr_cat = 0;
    unordered_map<string, vector<string>> raw_category_map;
    unordered_map<string, int> category_num_map;
    for (const auto& entry : fs::directory_iterator(metadata)) {
        cout << "Starting" << entry.path() << "\n";
        ifstream infile(entry.path(), ifstream::binary);
        string line;
        while (getline(infile, line, '\n')) {
            string asin = get_asin(line);
            string title = get_title(line);
            vector<string> categories = get_categories(line);
            vector<string> cat_rank = get_rank(line);
            (*id_to_product)[asin] = title;
            raw_category_map[asin] = categories;
            (*id_to_category_rank)[asin] = cat_rank.size() > 0 ? stoi(cat_rank[1]) : INT32_MAX;
            for (string s : categories) {
                if (category_num_map.count(s) == 0) {
                    category_num_map[s] = curr_cat;
                    curr_cat++;
                }
            }
        }
        cout << "Total number of Categories: " << curr_cat << "\n";
    }
    max_categories = curr_cat;
    for (auto const & pair : *id_to_product) {
        vector<string> curr_categories = raw_category_map[pair.first];
        vector<int> *category_nums = new vector<int>;
        for (string cat : curr_categories) {
            category_nums -> push_back(category_num_map[cat]);
        }
        (*id_to_category_vector)[pair.first] = category_nums;
    }
}

