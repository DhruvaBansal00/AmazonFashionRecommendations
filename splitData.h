#include "metrics.h"
#include <unordered_set>

using namespace std;

class Set {
    public:
        unordered_map<string, unordered_map<string, string>*> *user_product_rating;
        unordered_set<string> *products;
};

class SplitDataset {
    public:
        Set *trainset;
        Set *testset;

        Set *loocv_train_set;
        Set *loocv_test_set;
        ReadData *complete_dataset;

        SplitDataset(ReadData *data, double testProp);
        ~SplitDataset();
    private:
        void split_test_train(ReadData* data, double testProp);
        void add(string user, string item, string rating, Set* set);
};