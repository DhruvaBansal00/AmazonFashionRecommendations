#include "splitData.h"
#include <iostream>

int main() {
    ReadData *data = new ReadData();
    SplitDataset *testAndTrain = new SplitDataset(data, 0.15);
    return 0;
}