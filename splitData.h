#include "metrics.h"

using namespace std;

class SplitDataset {
    public:
        unordered_map<string, unordered_map<string, string>*> *trainset;
        unordered_map<string, unordered_map<string, string>*> *testset;

        SplitDataset(ReadData *data, double testProp);
        ~SplitDataset();
    private:
        void add(string user, string item, string rating, unordered_map<string, unordered_map<string, string>*>* set);
};