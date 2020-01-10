#include "readDataset.h"

using namespace std;
typedef struct {
    string user_id;
    string product_id;
    string estimated_rating;
    string actual_rating;
}Prediction;

double MAE(vector<Prediction>);
double RMSE(vector<Prediction>);
// void topN(vector<Prediction> predictions, int n, unordered_map<string, vector<Prediction>> *topN_per_user);
double hitRate(unordered_map<string,vector<Prediction>*> *topN_per_user, vector<Prediction> left_out_predictions, int topN);
double cummlativeHitRate(unordered_map<string,vector<Prediction>*> *topN_per_user, vector<Prediction> left_out_predictions, int ratingCutoff, int topN);
void ratingHitRate(unordered_map<string,vector<Prediction>*> *topN_per_user, vector<Prediction> left_out_predictions);
double averageReciprocalHitRank(unordered_map<string,vector<Prediction>*> *topN_per_user, vector<Prediction> left_out_predictions);
double userCoverage(unordered_map<string,vector<Prediction>*> *topN_per_user, int numUsers, int ratingCutoff);
double novelty(unordered_map<string,vector<Prediction>*> *topN_per_user, ReadData *dataset);
