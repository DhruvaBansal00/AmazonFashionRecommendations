#include "knn.h"
#include <iostream>
#include <queue>
#define N 5

typedef struct {
    bool operator()(Prediction p1, Prediction p2) 
    { 
        return (stoi(p2.estimated_rating) > stoi(p1.estimated_rating)); 
    } 
} Compare_Predictions;

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


    ContentKNN knn(testAndTrain ->loocv_train_set, 5, data);
    unordered_map<string,vector<Prediction>> *topN_per_user;
    for (auto const & pair1 : *(testAndTrain->complete_dataset->user_to_item_rating)) {
        priority_queue<Prediction, vector<Prediction>, Compare_Predictions> topN;
        for (string product : *testAndTrain->loocv_test_set->products) {
            double rating = knn.estimate_rating(pair1.first, product);
            if (topN.size() < N) {
                topN.push(Prediction{pair1.first, product, to_string(rating),  (*pair1.second)[product]});
            } else {
                Prediction topPred = topN.top;
                if (stod(topPred.estimated_rating) < rating) {
                    topN.pop;
                    topN.push(Prediction{pair1.first, product, to_string(rating),  (*pair1.second)[product]});
                }
            }
        }
        while (topN.size() != 0) {
            Prediction currPred = topN.top;
            topN.pop;
            (*topN_per_user)[pair1.first].push_back(currPred);
        }
    }

    delete testAndTrain;
    delete data;
    return 0;
}