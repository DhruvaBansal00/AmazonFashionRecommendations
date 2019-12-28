#include "metrics.h"
#include "math.h"
#include <algorithm>
#include <iostream>


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
        sort((*topN_per_user)[pair.first].begin(), (*topN_per_user)[pair.first].end(), comparePredictions);
    }
}

double hitRate(unordered_map<string,vector<Prediction>> *topN_per_user, vector<Prediction> left_out_predictions, int topN) {
    uint hits = 0;
    uint total = 0;
    for (Prediction p : left_out_predictions) {
        int rank = 0;
        for (Prediction n : (*topN_per_user)[p.user_id]) {
            if (n.product_id.compare(p.product_id) == 0) {
                if (rank < topN) {
                    hits++;
                }
            }
            rank++;
        }
        total++;
    }
    return double(hits)/total;
}

double cummlativeHitRate(unordered_map<string,vector<Prediction>> *topN_per_user, vector<Prediction> left_out_predictions, int ratingCutoff, int topN) {
    uint hits = 0;
    uint total = 0;
    for (Prediction p : left_out_predictions) {
        if (stoi(p.actual_rating) >= ratingCutoff) {
            int rank = 0;
            for (Prediction n : (*topN_per_user)[p.user_id]) {
                if (n.product_id.compare(p.product_id) == 0) {
                    if (rank < topN) {
                        hits++;
                    }
                }
                rank++;
            }
            total++;
        }
    }
    return double(hits)/total;
}

void ratingHitRate(unordered_map<string,vector<Prediction>> *topN_per_user, vector<Prediction> left_out_predictions) {
    unordered_map<string, int> hits;
    unordered_map<string, int> total;
    for (Prediction p : left_out_predictions) {
        for (Prediction q : (*topN_per_user)[p.user_id]) {
            if (q.product_id.compare(p.product_id) == 0) {
                hits[p.actual_rating]++;
            }
        }
        total[p.actual_rating]++;
    }
    for (auto const & pair : hits) {
        cout << pair.first << " : " << pair.second/total[pair.first] << "\n";
    }
}

double averageReciprocalHitRank(unordered_map<string,vector<Prediction>> *topN_per_user, vector<Prediction> left_out_predictions) {
    uint hits = 0;
    uint total = 0;
    double summation = 0;
    for (Prediction p : left_out_predictions) {
        int rank = 1;
        for (Prediction n : (*topN_per_user)[p.user_id]) {
            if (n.product_id.compare(p.product_id) == 0) {
                summation += 1.0/rank;
            }
            rank++;
        }
        total++;
    }
    return summation/total;
}

double userCoverage(unordered_map<string,vector<Prediction>> *topN_per_user, int numUsers, int ratingCutoff) {
    int hits = 0;

    for (auto const & pair : (*topN_per_user)) {
        for (Prediction p : pair.second) {
            if (stoi(p.estimated_rating) >= ratingCutoff) {
                hits++;
            }
        }
    }

    return double(hits)/numUsers;
}

double novelty(unordered_map<string,vector<Prediction>> *topN_per_user, ReadData *dataset) {
    int n = 0;
    int total = 0;
    for (auto const & pair : (*topN_per_user)) {
        for (Prediction p : pair.second) {
            total += (*(*dataset).id_to_category_rank)[p.user_id];
            n++;
        }
    }
    return double(total)/n;
}