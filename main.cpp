#include "knn.h"
#include <iostream>

int main() {
    ReadData *data = new ReadData();
    SplitDataset *testAndTrain = new SplitDataset(data, 0.10);
    ContentKNN knn(testAndTrain ->trainset, 5, data);
    vector<Prediction> predictions;
    for (auto const & pair1 : (*(*(*testAndTrain).testset).user_product_rating)) {
        for (auto const & pair2 : *pair1.second) {
            double predicted_rating = knn.estimate_rating(pair1.first, pair2.first);
            // cout << "Predicted Rating = " << predicted_rating << " REAL RATING = " << pair2.second << "\n";
            predictions.push_back(Prediction{pair1.first, pair2.first, to_string(predicted_rating), pair2.second});
        }
    }
    cout << "MAE = "<<MAE(predictions)<<"\n";
    cout << "RMSE = "<<RMSE(predictions)<<"\n";
    delete testAndTrain;
    delete data;
    return 0;
}