#include "train.h"

/*
почему >> не член класса
когда ошибка в вводе состояние класса меняться не должно
показать когда будет перемещение вместо копирования
 */

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

    if (is.fail()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    train.setMaxWeight(maxWeight);
    train.setMaxVolume(maxVolume);
    train.containers = new Container[countContainers];
    for (int i = 0; i < countContainers; i++) {
        Container container;
        is >> container;
        train += container;
    }
    train.setCountContainers(countContainers);
    return is;
}

//  метод вывода
std::ostream &operator<<(std::ostream &s, const Train &train) {
    s << "Max Weight - " << train.getMaxWeight() << "; Max Volume - " << train.getMaxVolume() << "; Containers - "
      << train.getCount() << std::endl;
    for (int i = 0; i < train.getCount(); i++) {
        s << train.getContainers()[i];
    }
    s << std::endl;
    return s;
}


//  (+=) добавление нового контейнера;
Train &Train::operator+=(Container newContainer) {
    if (countMass() + newContainer.getMass() <= maxWeight && newContainer.getVolume() <= maxVolume) {
        Container *tmp = new Container[countContainers + 1];
        for (int i = 0; i < countContainers; i++) {
            tmp[i] = containers[i];
        }

        tmp[countContainers] = std::move(newContainer); //перемещающий (если не стд мув то копирующий)
        countContainers++;
        delete[] containers;
        containers = tmp;
        return *this;
    } else {
        throw std::runtime_error("Capacity or volume limitation");
    }
}


//  ([]) получение контейнера по его номеру (возврат по ссылке);
Container &Train::operator[](int index) {
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
    if (countContainers != 0 || containers != nullptr) {
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

// копирующий оператор присваивания
Train &Train::operator=(const Train &tr) {
    if (this != &tr) {
        auto *new_ar = new Container[countContainers];
        maxVolume = tr.maxVolume;
        maxWeight = tr.maxWeight;
        delete[] containers;
        containers = new_ar;
        std::copy(tr.containers, tr.containers + tr.countContainers, containers);
    }
    return *this;
}

// перемещающий оператор присваивания
Train &Train::operator=(Train &&st) noexcept {
    std::swap(maxWeight, st.maxWeight);
    std::swap(maxVolume, st.maxVolume);
    std::swap(containers, st.containers);
    return *this;
}