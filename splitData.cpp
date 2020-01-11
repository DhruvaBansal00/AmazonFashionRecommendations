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
    loocv_test_set = new Set();
    loocv_train_set = new Set();
    (*testset).user_product_rating = new unordered_map<string, unordered_map<string, string>*>;
    (*testset).products = new unordered_set<string>;
    (*trainset).user_product_rating = new unordered_map<string, unordered_map<string, string>*>;
    (*trainset).products = new unordered_set<string>;
    (*loocv_test_set).user_product_rating = new unordered_map<string, unordered_map<string, string>*>;
    (*loocv_test_set).products = new unordered_set<string>;
    (*loocv_train_set).user_product_rating = new unordered_map<string, unordered_map<string, string>*>;
    (*loocv_train_set).products = new unordered_set<string>;
    split_test_train(data, testProp);
}

void SplitDataset::split_test_train(ReadData* data, double testProp) {
    complete_dataset = data;
    int elements_in_train = 0;
    int elements_in_test = 0;
    srand(time(0));
    for (auto const & pair1 : *(*data).user_to_item_rating) {
        bool first = true;
        if ((*pair1.second).size() >= 100) { //Filtering out users with less than 5 ratings
            for (auto const & pair2 : *pair1.second) {
                if (first) {
                    add(pair1.first, pair2.first, pair2.second, loocv_test_set);
                    first = false;
                } else {
                    add(pair1.first, pair2.first, pair2.second, loocv_train_set);
                }
                double thresh = random()/double(RAND_MAX);
                if ( thresh <= testProp) {
                    // cout << "Adding to test set\n";
                    add(pair1.first, pair2.first, pair2.second, testset);
                    elements_in_test++;
                } else {
                    // cout << "Adding to train set\n";
                    add(pair1.first, pair2.first, pair2.second, trainset);
                    elements_in_train++;
                }
            }
        }
    }
    cout << "Elemenets in train: "<<elements_in_train << " Elemenets in test: "<<elements_in_test <<"\n";
    cout << "User in LOOCV test set: " << loocv_test_set->user_product_rating->size() << " User in LOOCV train set: " << loocv_train_set->user_product_rating->size()<<"\n";
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

