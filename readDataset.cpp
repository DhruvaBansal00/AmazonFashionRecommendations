#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <experimental/filesystem>

using namespace std;
namespace fs = experimental::filesystem;

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
    for (const auto & entry : fs::directory_iterator(ratings))
        cout << entry.path() << std::endl;
}


int main() {
    ReadData x = ReadData();
    return 0;
}