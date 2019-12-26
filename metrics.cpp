#include "metrics.h"

using namespace std;

double MAE(vector<Prediction> predictions) {
    uint abs_err = 0;
    uint num = 0;
    for (Prediction p : predictions) {
        abs_err += abs(stoi(p.actual_rating) - stoi(p.estimated_rating));
        num++;
    }
    return abs_err/num;
}