#include <iostream>
#include "../train/train.h"

using namespace std;

int main() {
    Train train;
    std::cin >> train;

    std::cout << train;

    std::cout << "Peek 1 container: " << std::endl;
    Container c = train.operator[](1);
    std::cout << c;
    std::cout << std::endl;

    std::cout << "Count all mass: ";
    std::cout << train.countMass() << std::endl << std::endl;

    std::cout << "Count center mass: ";
    std::cout << train.countCenterMass() << std::endl << std::endl;

    Container newContainer("Cargo1", 5.0, 12.0, BULK);

    std::cout << "Adding new container to train: ";
    std::cout << newContainer;
    train += newContainer;

    std::cout << "Train:" << std::endl;
    std::cout << train;
    std::cout << std::endl;

    std::cout << "Removing container 2 from train:" << std::endl;
    train.deleteContainer(2);
    std::cout << "Train:" << std::endl;
    std::cout << train;
}
