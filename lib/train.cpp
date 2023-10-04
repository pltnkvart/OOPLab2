#include "train.h"


//  конструктор по умолчанию (явный или неявный);
Train::Train() : containers(nullptr), countContainers(0), maxWeight(0.0), maxVolume(0.0) {};

//  создание экземпляров класса с инициализацией заданным количеством контейнеров из массива контейнеров;
Train::Train(int number) : containers(nullptr), countContainers(number), maxWeight(0.0), maxVolume(0.0) {};

//  создание экземпляров класса с инициализацией одним контейнером;
//Train::Train(MyContainer::Container &) : {};

//  копирующий конструктор
Train::Train(const Train &tr) : countContainers(tr.countContainers), maxVolume(tr.maxVolume), maxWeight(tr.maxWeight) {
    containers = new MyContainer::Container[countContainers];
    std::copy(tr.containers, tr.containers + tr.countContainers, containers);
}

//  перемещающий конструктор
Train::Train(Train &&tr) noexcept: countContainers(tr.countContainers), maxWeight(tr.maxWeight),
                                   maxVolume(tr.maxVolume) {
    tr.containers = nullptr;
}

//  методы ввода и вывода состояния класса в входной/выходной поток;
void Train::inputState() {

}

void Train::printState() {
    for (int i = 0; i < countContainers; ++i) {
        containers[i].print();
    }
}

//  (+=) добавление нового контейнера;
void Train::operator+=(MyContainer::Container &newContainer) {
    if (newContainer.getMass() > maxWeight || newContainer.getVolume() > maxVolume) {
        std::cout << "Невозможно добавить контейнер из-за ограничений грузоподъемности или объема" << std::endl;
        return;
    }
    auto *newArray = new MyContainer::Container[countContainers + 1];
    for (int i = 0; i < countContainers; ++i) {
        newArray[i] = containers[i];
    }
    newArray[countContainers] = newContainer;
    delete[] containers;
    containers = newArray;
    countContainers++;
}

//  ([]) получение контейнера по его номеру (возврат по ссылке);
MyContainer::Container &Train::operator[](int index) {
    if (index < countContainers) {
        return containers[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

//  удаление контейнера по его номеру;
void Train::deleteContainer(int index) {
    if (index < countContainers) {
        for (int i = index; i < countContainers - 1; ++i) {
            containers[i] = containers[i + 1];
        }
        countContainers--;
    } else {
        throw std::out_of_range("Index out of range");
    }
}

//  подсчёт суммарной массы всех контейнеров;
double Train::countMass() {
    double totalMass = 0.0;
    for (int i = 0; i < countContainers; i++) {
        totalMass += containers[i].getMass();
    }
    return totalMass;
}

//  вычисление центра массы поезда, измеряемой в контейнерах (считаем, что все контейнеры имеют одинаковую длину);
double Train::countCenterMass() {
    if (countContainers != 0) {
        double totalMass = countMass();
        double massMoment = 0.0;
        for (int i = 0; i < countContainers; ++i) {
            massMoment += containers[i].getMass() * i;
        }
        return massMoment / totalMass;
    }
    return 0.0;
}

//  минимизация количества контейнеров за счёт перегрузки груза между ними и удаления пустых контейнеров;
void Train::minContainers() {

}

//  обеспечение безопасности транспортировки опасных грузов — между двумя контейнерами с опасным грузом должно находиться не менее двух контейнеров с безопасным грузом (обеспечить при помощи перемещения контейнеров, при необходимости добавить пустые контейнеры).
void Train::ensuringSecurity() {

}
