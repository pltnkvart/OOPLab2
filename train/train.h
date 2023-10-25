#ifndef LAB2_B_TRAIN_H
#define LAB2_B_TRAIN_H

#include "../container/container.h"

/**
 * @brief Class Train
 */
class Train {
private:
    //! Array containers
    Container *containers = nullptr;
    //! Number of containers in train
    int countContainers;
    //! Max weight of containers in train
    double maxWeight;
    //! Limit volume of container in train
    double maxVolume;
public:
    Train();
    Train(int, Container (&containers)[], double, double);
    Train(Container &, double, double);
    Train(const Train &);
    Train(Train &&) noexcept;
    /**
    * @brief Destructor
    */
    ~Train() { delete[] containers; };

    //  setters
    /**
     * @brief Setting weight
     */
    void setMaxWeight(double weight) { maxWeight = weight; };
    /**
    * @brief Setting volume
    */
    void setMaxVolume(double volume) { maxVolume = volume; };
    /**
     * @brief Setting count containers
     */
    void setCountContainers(int count) { countContainers = count; };

    //  getters
    /**
     * @brief Getting munber of containers in train
     */
    int getCount() const { return countContainers; };
    /**
     * @brief Getting max weight of train
     */
    int getMaxWeight() const { return maxWeight; };
    /**
     * @brief Getting max volume of train
     */
    int getMaxVolume() const { return maxVolume; };
    /**
     * @brief Getting array of containers in train
     */
    const Container *getContainers() const { return containers; };

    friend std::istream &operator>>(std::istream &is, Train &train);

    Train &operator+=(Container);
    Container &operator[](int);
    void deleteContainer(int);
    double countMass();
    double countCenterMass();
    void minContainers();
    void ensuringSecurity();

    Train &operator=(const Train &);
    Train &operator=(Train &&) noexcept;
};

std::ostream &operator<<(std::ostream &s, const Train &train);

#endif //LAB2_B_TRAIN_H
