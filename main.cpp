#include "knn.h"
#include <iostream>

int main() {
    srand(time(0));
    ReadData *data = new ReadData();
    SplitDataset *testAndTrain = new SplitDataset(data, 0.10);
    ContentKNN knn(testAndTrain ->trainset, 5, data);
    vector<Prediction> predictions;
    vector<Prediction> random_predictions;
    for (auto const & pair1 : (*(*(*testAndTrain).testset).user_product_rating)) {
        for (auto const & pair2 : *pair1.second) {
            double predicted_rating = knn.estimate_rating(pair1.first, pair2.first);
            double random_prediction = 5*random()/((double)RAND_MAX);
            // cout << "Predicted Rating = " << predicted_rating << " REAL RATING = " << pair2.second << "\n";
            predictions.push_back(Prediction{pair1.first, pair2.first, to_string(predicted_rating), pair2.second});
            random_predictions.push_back(Prediction{pair1.first, pair2.first, to_string(random_prediction), pair2.second});
        }
    }
    cout << "KNN STATS\n";
    cout << "\tMAE = "<<MAE(predictions)<<"\n";
    cout << "\tRMSE = "<<RMSE(predictions)<<"\n";
    cout << "RANDOM STATS\n";
    cout << "\tMAE = "<<MAE(random_predictions)<<"\n";
    cout << "\tRMSE = "<<RMSE(random_predictions)<<"\n";
    delete testAndTrain;
    delete data;
    return 0;
}