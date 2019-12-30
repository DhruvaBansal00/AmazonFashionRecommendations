#include "math.h"
#include "knn.h"
#include <unordered_set>
#include <queue>

ContentKNN::ContentKNN(ReadData *dataset, int k) {
    similarity_scores = new unordered_map<string, unordered_map<string, double>*>();
    (*this).dataset = dataset;
    (*this).k = k;
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

typedef struct  {
    string item;
    double similarity;
    string rating;
} Neighbor;

struct compareNeighbor{
    bool operator()(Neighbor a, Neighbor b) {
        return a.similarity > b.similarity;
    }
}; 

double ContentKNN::estimate_rating(string user, string item) {
    unordered_map<string, string> curr_user_ratings = *(*(*dataset).user_to_item_rating)[user];
    priority_queue<Neighbor, vector<Neighbor>, compareNeighbor> closestNeighbors;

    for (auto const & pair : curr_user_ratings) {
        closestNeighbors.push({pair.first, (*(*similarity_scores)[item])[pair.first], pair.second});
    }

    vector<Neighbor> closestKNeighbors;
    int neighbors = k;
    while (neighbors > 0 && closestNeighbors.size() != 0) {
        closestNeighbors.push(closestNeighbors.top());
        closestNeighbors.pop();
        neighbors--;
    }

    double similarityTotal = 0;
    double weightedSum = 0;
    
    for (Neighbor n : closestKNeighbors) {
        similarityTotal += n.similarity;
        weightedSum += stoi(n.rating)*n.similarity;
    }

    return weightedSum/similarityTotal;

}