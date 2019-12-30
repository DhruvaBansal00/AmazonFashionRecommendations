#include <queue>
#include "readDataset.h"

using namespace std;

class ContentKNN {
    public:
        unordered_map<string, unordered_map<string, double>*> *similarity_scores;
        unordered_map<string, unordered_map<string, string>*> *trainset;
        int k;
        ReadData* dataset;
        ContentKNN(unordered_map<string, unordered_map<string, string>*> *trainset, int k, ReadData* dataset);
        ~ContentKNN();
        void compute_similarity(unordered_map<string, unordered_map<string, string>*> *trainset);
        double estimate_rating(string user, string item);
};