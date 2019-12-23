#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <experimental/filesystem>

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
        
};


ReadData::ReadData() {
    string ratings = "../Datasets/Ratings";
    string metadata = "../Datasets/Metadata";
    user_ratings(ratings);
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




int main() {
    ReadData x = ReadData();
    return 0;
}