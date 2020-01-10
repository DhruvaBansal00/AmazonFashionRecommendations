#include "math.h"
#include "knn.h"
#include <unordered_set>
#include <queue>
#include <iostream>


ContentKNN::ContentKNN(Set* trainset, int k, ReadData* dataset) {
    (*this).trainset = trainset;
    (*this).dataset = dataset;
    (*this).k = k;
}

double category_similarity(vector<int>* product1, vector<int>* product2) {
    uint sumxx = uint((*product1).capacity());
    uint sumyy = uint((*product2).capacity());
    unordered_set<int> product2_categories;;
    for (int temp : *product2) {product2_categories.insert(temp);}
    int sumxy = 0;
    for (int cat1 : *product1) {
        if (product2_categories.count(cat1) != 0) {
            sumxy++;
        }
    }
    return sumxx*sumyy == 0 ? 0 : double(sumxy)/sqrt(sumxx*sumyy);
}

typedef struct  {
    string item;
    double similarity;
    string rating;
} Neighbor;

struct compareNeighbor{
    bool operator()(Neighbor a, Neighbor b) {
        return a.similarity < b.similarity;
    }
}; 

double ContentKNN::estimate_rating(string user, string item, bool verbose=false) {
    if (verbose) {
        cout << "Estimating Rating of " << user << " and " << item << "\n"; 
    }
    unordered_map<string, string> curr_user_ratings = *(*(*trainset).user_product_rating)[user];
    if (verbose) {
        cout << "Found curr user ratings\n"; 
    }
    priority_queue<Neighbor, vector<Neighbor>, compareNeighbor> closestNeighbors;
    for (auto const & pair : curr_user_ratings) {
        closestNeighbors.push({pair.first, 
        category_similarity((*(*dataset).id_to_category_vector)[item], (*(*dataset).id_to_category_vector)[pair.first]), 
        pair.second});
    }

    vector<Neighbor> closestKNeighbors;
    int neighbors = k;
    while (neighbors > 0 && closestNeighbors.size() != 0) {
        closestKNeighbors.push_back(closestNeighbors.top());
        closestNeighbors.pop();
        neighbors--;
    }

    if (verbose) {
        cout << "Found " << k << " neighbors" << "\n"; 
    }

    double similarityTotal = 0;
    double weightedSum = 0;
    
    for (Neighbor n : closestKNeighbors) {
        if (verbose) {
            cout << n.item << " " << n.rating << " " << n.similarity << "\n";
        }
        similarityTotal += n.similarity;
        weightedSum += stoi(n.rating)*n.similarity;
    }

    // if (similarityTotal == 0) {
    //     cout << "NOT ENOUGH DATA FOR THIS INPUT\n";
    // }
    return similarityTotal == 0 ? 0 : weightedSum/similarityTotal;
}