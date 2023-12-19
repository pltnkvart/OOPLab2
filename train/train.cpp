#include "train.h"

/*
почему >> не член класса
когда ошибка в вводе состояние класса меняться не должно
показать когда будет перемещение вместо копирования
 */

/**
 * @brief Empty class constructor
 */
Train::Train() : containers(nullptr), countContainers(0), maxWeight(0.0), maxVolume(0.0) {};

/**
 * @brief Class constructor with initialization of a given number containers from an array
 * @param containersInit Array containers
 * @param _volume Max volume for train
 * @param _weight Max weight for train
 * @param number Number of containers
 */
Train::Train(int number, Container (&containersInit)[], double _weight, double _volume) {
    maxWeight = _weight;
    maxVolume = _volume;
    countContainers = 0;
    containers = new Container[number];
    for (int i = 0; i < number; i++) {
        this->operator+=(containersInit[i]);
    }
};

/**
 * @brief Class constructor with initialization by one container
 * @param oneContainer One container
 * @param _weight Max weight for train
 * @param _volume Max volume for train
 */
Train::Train(Container &oneContainer, double _weight, double _volume) {
    containers = new Container[1];
    containers[0] = oneContainer;
    maxWeight = _weight;
    maxVolume = _volume;
    countContainers = 1;
};

/**
 * @brief Copy constructor
 * @param tr Element to be copied
 */
Train::Train(const Train &tr) : countContainers(tr.countContainers), maxVolume(tr.maxVolume), maxWeight(tr.maxWeight) {
    containers = new Container[countContainers];
    std::copy(tr.containers, tr.containers + tr.countContainers, containers);
}

/**
 * @brief Moving constructor
 * @param tr Element to be moved
 */
Train::Train(Train &&tr) noexcept: countContainers(tr.countContainers), maxWeight(tr.maxWeight),
                                   maxVolume(tr.maxVolume) {
    tr.containers = nullptr;
}

/**
 * @brief Overload input function
 * @param is Stream into which data is entered
 * @param train Writable structure
 * @return Stream
 */
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


/**
 * @brief Overload output function
 * @param s Stream from which data is entered
 * @param train Readable structure
 * @return Stream
 */
std::ostream &operator<<(std::ostream &s, const Train &train) {
    s << "Max Weight - " << train.getMaxWeight() << "; Max Volume - " << train.getMaxVolume() << "; Containers - "
      << train.getCount() << std::endl;
    for (int i = 0; i < train.getCount(); i++) {
        s << train.getContainers()[i];
    }
    s << std::endl;
    return s;
}

/**
 * @brief Adding new container
 * @param newContainer Exemplar new container
 * @throw std::runtime_error Capacity or volume limitation
 * @return Self pointer
 */
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


/**
 * @brief Getting a container by its index
 * @param index Index of container to be received
 * @throw std::out_of_range Index of container to be received out of range
 * @return Reference to the container
 */
Container &Train::operator[](int index) {
    if (index < countContainers) {
        return containers[index];
    } else {
        throw std::out_of_range("Index out of range");
    }
}

/**
 * @brief Deleting container by its index
 * @param index Index of container to be deleted
 * @throw std::out_of_range Index of container to be deleted out of range
 */
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

/**
 * @brief Total mass calculation
 * @return Total mass
 */
double Train::countMass() {
    double totalMass = 0.0;
    if (countContainers != 0 || containers != nullptr) {
        for (int i = 0; i < countContainers; i++) {
            totalMass += containers[i].getMass();
        }
    }
    return totalMass;
}

/**
 * @brief Calculating the center of mass of a train, measured in containers
 * @return Center of mass of a train
 */
double Train::countCenterMass() {
    if (countContainers != 0) {
        double totalMass = countMass();
        double massMoment = 0.0;
        for (int i = 1; i <= countContainers; ++i) {
            massMoment += containers[i].getMass() * i;
        }
        return massMoment / totalMass;
    }
    return 0.0;
}

/**
 * @brief Minimizing the number of containers function by reloading cargo between them and removing empty containers
 */
void Train::minContainers() {
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


/**
 * @brief Ensuring the safety of transportation of dangerous goods function
 */
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


/**
 * @brief Copy assignment operator
 */
Train &Train::operator=(const Train &tr) {
    if (this != &tr) {
        delete[] containers;
        containers = new Container[tr.countContainers];
        maxVolume = tr.maxVolume;
        maxWeight = tr.maxWeight;
        countContainers = tr.countContainers;
        for (int i = 0; i < countContainers; i++) {
            containers[i] = tr.containers[i];
        }
    }
    return *this;
}


/**
 * @brief Moving assignment operator
 */
Train &Train::operator=(Train &&st) noexcept {
    std::swap(maxWeight, st.maxWeight);
    std::swap(maxVolume, st.maxVolume);
    std::swap(countContainers, st.countContainers);
    std::swap(containers, st.containers);
    return *this;
}