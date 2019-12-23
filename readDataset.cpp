#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <experimental/filesystem>
// #include <jsoncpp/json/json.h>

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

class ReadData {
    public:
        unordered_map<string, string> id_to_product;
        unordered_map<string, unordered_map<string, string>> user_to_item_rating;
        unordered_map<string, vector<int>> id_to_category_vector;
        unordered_map<string, int> id_to_category_rank;

        ReadData();
        void user_ratings(string ratings);
        void id_stats(string metadata);
};


ReadData::ReadData() {
    string ratings = "../Datasets/Ratings";
    string metadata = "../Datasets/Metadata";
    // user_ratings(ratings);
    id_stats(metadata);
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
            user_to_item_rating[line[0]][line[1]] = line[2];
            line.clear();
        }
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
    size_t index = line.find("categories");
    vector<string> categories;
    if (index >= line.size()) {return categories;}
    int i = (int)index + 14;
    stringstream s;
    while (line[i] != ']') {
        if (line[i] != '\'')
            s << line[i];
        if (line[i] == ',')  {
            categories.push_back(s.str());
            s.str("");
        }
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
        if (line[i] != '\'')
            s << line[i];
        i++;
    }
    cells.push_back(s.str());
    return cells;
}

void print_vec(vector<string> x) {
    for (string s : x) {
        cout << s << "\n";
    }
}

void ReadData::id_stats(string metadata) {
    for (const auto& entry : fs::directory_iterator(metadata)) {
        cout << "Starting" << entry.path() << "\n";
        ifstream infile(entry.path(), ifstream::binary);
        string line;
        unordered_map<string, vector<string>> raw_category_map;
        unordered_map<string, int> category_num_map;
        int curr_cat = 0;
        while (getline(infile, line, '\n')) {
            string asin = get_asin(line);
            string title = get_title(line);
            vector<string> categories = get_categories(line);
            vector<string> cat_rank = get_rank(line);
            id_to_product[asin] = title;
            raw_category_map[asin] = categories;
            cout << asin << "\n";
            print_vec(categories);
            cout << line << "\n";
            id_to_category_rank[asin] = cat_rank.size() > 0 ? stoi(cat_rank[1]) : INT32_MAX;
            for (string s : categories) {
                if (category_num_map.count(s) == 0) {
                    category_num_map[s] = curr_cat;
                    curr_cat++;
                }
            }
            cout << "CurrCat num " << curr_cat << "\n";
        }
    }
}

int main() {
    ReadData x = ReadData();
    return 0;
}