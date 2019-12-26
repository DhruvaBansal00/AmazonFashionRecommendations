#include "metrics.h"
#include "math.h"
#include <algorithm>


using namespace std;

double MAE(vector<Prediction> predictions) {
    double abs_err = 0;
    double num = 0;
    for (Prediction p : predictions) {
        abs_err += abs(stoi(p.actual_rating) - stoi(p.estimated_rating));
        num++;
    }
    return abs_err/num;
}

double RMSE(vector<Prediction> predictions) {
    double abs_sqr_err = 0;
    double num = 0;
    for (Prediction p : predictions) {
        abs_sqr_err += pow(stoi(p.actual_rating) - stoi(p.estimated_rating), 2);
        num++;
    }
    return sqrt(abs_sqr_err/num);
}

bool comparePredictions(Prediction p1, Prediction p2) 
{ 
    return (stoi(p2.estimated_rating) < stoi(p1.estimated_rating)); 
} 

void topN(vector<Prediction> predictions, int n, unordered_map<string, vector<Prediction>> *topN_per_user) {
    for (Prediction p : predictions) {
        (*topN_per_user)[p.user_id].push_back(p);
    }
    for (auto const & pair : (*topN_per_user)) {
        sort(pair.second.begin(), pair.second.end(), comparePredictions);
    }
}

