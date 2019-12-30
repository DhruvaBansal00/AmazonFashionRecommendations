#include "splitData.h"
#include <iostream>

void SplitDataset::add(string user, string item, string rating, Set *set) {
    if ((*(*set).user_product_rating).count(user) == 0) {
        (*(*set).user_product_rating)[user] = new unordered_map<string, string>;
    }
    (*(*(*set).user_product_rating)[user])[item] = rating;
    (*(*set).products).insert(item);
}

SplitDataset::SplitDataset(ReadData* data, double testProp) {
    cout << "Splitting data\n";
    testset = new Set();
    trainset = new Set();
    (*testset).user_product_rating = new unordered_map<string, unordered_map<string, string>*>;
    (*testset).products = new unordered_set<string>;
    (*trainset).user_product_rating = new unordered_map<string, unordered_map<string, string>*>;
    (*trainset).products = new unordered_set<string>;
    srand(time(0));
    for (auto const & pair1 : *(*data).user_to_item_rating) {
        for (auto const & pair2 : *pair1.second) {
            double thresh = random()/double(RAND_MAX);
            if ( thresh <= testProp) {
                // cout << "Adding to test set\n";
                add(pair1.first, pair2.first, pair2.second, testset);
            } else {
                // cout << "Adding to train set\n";
                add(pair1.first, pair2.first, pair2.second, trainset);
            }
        }
    }
}

SplitDataset::~SplitDataset() {
    for (auto const & pair : *(*testset).user_product_rating) {
        delete pair.second;
    }
    delete (*testset).user_product_rating;
    delete (*testset).products;

    for (auto const & pair : *(*trainset).user_product_rating) {
        delete pair.second;
    }
    delete (*trainset).user_product_rating;
    delete (*trainset).products;
}

