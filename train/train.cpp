#include "train.h"


//  конструктор по умолчанию (явный или неявный);
Train::Train() : containers(nullptr), countContainers(0), maxWeight(0.0), maxVolume(0.0) {};

//  создание экземпляров класса с инициализацией заданным количеством контейнеров из массива контейнеров;
Train::Train(int number, Container (&containersInit)[], double _weight, double _volume) {
    maxWeight = _weight;
    maxVolume = _volume;
    countContainers = 0;
    containers = new Container[number];
    for (int i = 0; i < number; i++) {
        this->operator+=(containersInit[i]);
    }
};

//  создание экземпляров класса с инициализацией одним контейнером;
Train::Train(Container &oneContainer, double _weight, double _volume) {
    containers = &oneContainer;
    maxWeight = _weight;
    maxVolume = _volume;
    countContainers = 1;
};

//  копирующий конструктор
Train::Train(const Train &tr) : countContainers(tr.countContainers), maxVolume(tr.maxVolume), maxWeight(tr.maxWeight) {
    containers = new Container[countContainers];
    std::copy(tr.containers, tr.containers + tr.countContainers, containers);
}

//  перемещающий конструктор
Train::Train(Train &&tr) noexcept: countContainers(tr.countContainers), maxWeight(tr.maxWeight),
                                   maxVolume(tr.maxVolume) {
    tr.containers = nullptr;
}

//  методы ввода состояния класса в входной поток;
std::istream &operator>>(std::istream &is, Train &train) {
    double maxWeight, maxVolume;
    int countContainers;

    is >> maxWeight >> maxVolume >> countContainers;

    train.setMaxWeight(maxWeight);
    train.setMaxVolume(maxVolume);
    train.containers = new Container[countContainers];
    std::cout << "Added weight and volume" << std::endl;
    for (int i = 0; i < countContainers; i++) {
        Container container;
        is >> container;
        train += container;
    }
    return is;
}

//  метод вывода
std::ostream &operator<<(std::ostream &s, Train &train) {
    for (int i = 0; i < train.getCount(); i++) {
        s << train.getContainers()[i];
    }
    s << std::endl;
    return s;
}


//  (+=) добавление нового контейнера;
void Train::operator+=(Container &newContainer) {
    if (countMass() + newContainer.getMass() <= maxWeight && newContainer.getVolume() <= maxVolume) {
        containers[countContainers] = newContainer;
        countContainers++;
    } else {
        std::cout << "Невозможно добавить контейнер из-за ограничений грузоподъемности или объема" << std::endl;
    }
}

//  ([]) получение контейнера по его номеру (возврат по ссылке);
Container &Train::operator[](int index) {
    if (index < countContainers) {
        return containers[index];
    } else {
        std::cout << "Index out of range" << std::endl;
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
    if (countContainers != 0) {
        for (int i = 0; i < countContainers; i++) {
            totalMass += containers[i].getMass();
        }
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
    // Удаление пустых контейнеров
    int i = 0;
    while (i < countContainers) {
        if (containers[i].getCategory() == EMPTY) {
            deleteContainer(i);
        } else {
            i++;
        }
    }
    i = 0;
    while (i < countContainers - 1) {
        int j = i + 1;
        while (j < countContainers) {
            double remainingSpace = containers[j].getVolume() - containers[i].getVolume();
            if (remainingSpace > 0.0) {
                double availableCargo = containers[i].getMass();
                if (availableCargo > 0.0) {
                    containers[i].transferCargo(availableCargo, containers[j]);
                }
            }
            j++;
        }
        i++;
    }
}


//  обеспечение безопасности транспортировки опасных грузов — между двумя контейнерами с опасным грузом должно находиться не менее двух контейнеров с безопасным грузом (обеспечить при помощи перемещения контейнеров, при необходимости добавить пустые контейнеры).
void Train::ensuringSecurity() {
    int i = 0;
    while (i < countContainers) {
        if (containers[i].getCategory() == DANGEROUS) {
            int j = i - 2;
            while (j < i + 3) {
                if (j >= 0 && j < countContainers && containers[j].getCategory() != DANGEROUS) {
                    for (int k = i - 1; k > j; --k) {
                        Container temp = containers[k];
                        containers[k] = containers[k + 1];
                        containers[k + 1] = temp;
                    }
                    Container newCont("Empty Container", 0.0, 0.0, EMPTY);
                    operator+=(newCont);
                    i++;
                    break;
                }
                j++;
            }
        }
        i++;
    }
}
