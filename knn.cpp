#include "math.h"
#include "knn.h"

ContentKNN::ContentKNN(ReadData *dataset) {
    similarity_scores = new unordered_map<string, unordered_map<string, double>*>();
    compute_similarity(dataset);
}

ContentKNN::~ContentKNN() {
    for (auto const & pair : *similarity_scores) {
        delete pair.second;
    }
    delete similarity_scores;
}
void ContentKNN::compute_similarity(ReadData *dataset) {

}
double ContentKNN::estimate_rating(string user, string item) {
    return 0.0;
}