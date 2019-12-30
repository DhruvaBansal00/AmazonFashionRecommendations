#include "knn.h"
#include <iostream>

int main() {
    ReadData *data = new ReadData();
    SplitDataset *testAndTrain = new SplitDataset(data, 0.15);
    ContentKNN knn(testAndTrain ->trainset, 5, data);
    for (auto const & pair1 : (*(*(*testAndTrain).testset).user_product_rating)) {
        for (auto const & pair2 : *pair1.second) {
            cout << "RATING = " << knn.estimate_rating(pair1.first, pair2.first) << " REAL RATING = " << pair2.second;
        }
    }
    delete testAndTrain;
    delete data;
    return 0;
}