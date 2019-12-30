#include "math.h"
#include "knn.h"
#include <unordered_set>
#include <queue>
#include <iostream>

ContentKNN::ContentKNN(Set* trainset, int k, ReadData* dataset) {
    similarity_scores = new unordered_map<string, unordered_map<string, double>*>();
    (*this).trainset = trainset;
    (*this).dataset = dataset;
    (*this).k = k;
    cout << "Computing similarity matrix now\n";
    compute_similarity(trainset);
}

ContentKNN::~ContentKNN() {
    for (auto const & pair : *similarity_scores) {
        delete pair.second;
    }
    delete similarity_scores;
}

double category_similarity(vector<int>* product1, vector<int>* product2) { 
    cout << "Computing current category similarity\n";
    cout << "Curr sizes = " << (*product1).size() << " " << (*product2).size() << "\n";
    uint sumxx = uint((*product1).capacity());
    uint sumyy = uint((*product2).capacity());
    unordered_set<int> product2_categories;;
    cout << "Adding elements\n";
    for (int temp : *product2) {product2_categories.insert(temp);}
    int sumxy = 0;
    for (int cat1 : *product1) {
        if (product2_categories.count(cat1) != 0) {
            sumxy++;
        }
    }
    return sumxx*sumyy == 0 ? 0 : double(sumxy)/sqrt(sumxx*sumyy);
}

void ContentKNN::compute_similarity(Set *trainset) {
    for (string product1 : *(*trainset).products) {
        for (string product2 : *(*trainset).products) {
            cout << product1 << " " << product2<< "\n";
            if ((product1).compare(product2) != 0) {
                if ((*similarity_scores).count(product1) == 0) {
                    (*similarity_scores)[product1] = new unordered_map<string, double>();
                }
                (*(*similarity_scores)[product1])[product2] = 
                    category_similarity((*(*dataset).id_to_category_vector)[product1], (*(*dataset).id_to_category_vector)[product2]);
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
    unordered_map<string, string> curr_user_ratings = *(*(*trainset).user_product_rating)[user];
    priority_queue<Neighbor, vector<Neighbor>, compareNeighbor> closestNeighbors;

    for (auto const & pair : curr_user_ratings) {
        closestNeighbors.push({pair.first, (*(*similarity_scores)[item])[pair.first], pair.second});
    }

    vector<Neighbor> closestKNeighbors;
    int neighbors = k;
    while (neighbors > 0 && closestNeighbors.size() != 0) {
        closestKNeighbors.push_back(closestNeighbors.top());
        closestNeighbors.pop();
        neighbors--;
    }

    double similarityTotal = 0;
    double weightedSum = 0;
    
    for (Neighbor n : closestKNeighbors) {
        cout << n.item << " " << n.rating << " " << n.similarity << "\n";
        similarityTotal += n.similarity;
        weightedSum += stoi(n.rating)*n.similarity;
    }

    if (similarityTotal == 0) {
        cout << "NOT ENOUGH DATA FOR THIS INPUT\n";
    }
    return similarityTotal == 0 ? 0 : weightedSum/similarityTotal;
}