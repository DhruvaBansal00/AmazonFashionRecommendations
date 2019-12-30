#include <queue>
#include "splitData.h"

using namespace std;

class ContentKNN {
    public:
        unordered_map<string, unordered_map<string, double>*> *similarity_scores;
        Set *trainset;
        int k;
        ReadData* dataset;
        ContentKNN(Set* trainset, int k, ReadData* dataset);
        ~ContentKNN();
        void compute_similarity(Set *trainset);
        double estimate_rating(string user, string item);
};