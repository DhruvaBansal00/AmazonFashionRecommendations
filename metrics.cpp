#include "metrics.h"
#include "math.h"

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

