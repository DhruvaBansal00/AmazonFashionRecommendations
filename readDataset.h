#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class ReadData {
    public:
        unordered_map<string, string> *id_to_product;
        unordered_map<string, unordered_map<string, string>*> *user_to_item_rating;
        unordered_map<string, vector<int>*> *id_to_category_vector;
        unordered_map<string, int> *id_to_category_rank;
        int max_categories;
        ReadData();
        void user_ratings(string ratings);
        void id_stats(string metadata);
        ~ReadData();
};


void print_map(unordered_map<string, unordered_map<string, string>> map);