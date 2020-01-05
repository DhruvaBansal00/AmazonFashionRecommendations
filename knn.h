#include <queue>
#include "splitData.h"

using namespace std;



class ContentKNN {
    public:
        Set *trainset;
        int k;
        ReadData* dataset;
        ContentKNN(Set* trainset, int k, ReadData* dataset);
        void compute_similarity(Set *trainset);
        double estimate_rating(string user, string item);
};