#include "math.h"
#include "knn.h"
#include <unordered_set>

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

double category_similarity(vector<int>* product1, vector<int>* product2) {
    int sumxx = (*product1).size();
    int sumyy = (*product2).size();
    unordered_set<int> product2_categories((*product2).begin(), (*product2).begin());
    int sumxy = 0;
    for (int cat1 : *product1) {
        if (product2_categories.count(cat1) != 0) {
            sumxy++;
        }
    }
    return double(sumxy)/sqrt(sumxx*sumyy);
}

void ContentKNN::compute_similarity(ReadData *dataset) {
    for (auto const & pair1 : *(*dataset).id_to_category_vector) {
        for (auto const & pair2 : *(*dataset).id_to_category_vector) {
            if (pair1.first.compare(pair2.first) != 0) {
                if ((*(*similarity_scores)[pair1.first]).count(pair2.first) == 0) {
                    (*similarity_scores)[pair1.first] = new unordered_map<string, double>();
                }
                (*(*similarity_scores)[pair1.first])[pair2.first] = category_similarity(pair1.second, pair2.second);
            }
        }
    }
}

double ContentKNN::estimate_rating(string user, string item) {
    return 0.0;
}