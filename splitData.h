#include "metrics.h"
#include <unordered_set>

using namespace std;

typedef struct {
    unordered_map<string, unordered_map<string, string>*> *user_product_rating;
    unordered_set<string> *products;
} Set;

class SplitDataset {
    public:
        Set *trainset;
        Set *testset;

        SplitDataset(ReadData *data, double testProp);
        ~SplitDataset();
    private:
        void add(string user, string item, string rating, Set* set);
};