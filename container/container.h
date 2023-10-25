#ifndef LAB2_CONTAINER_H
#define LAB2_CONTAINER_H

#include <iostream>

enum CargoCategory {
    EMPTY,      //  пустой
    PIECES,     //  штучные
    BULK,       //  насыпные
    LIQUID,     //  жидкости
    GAS,        //  газы
    DANGEROUS   //  опасные
};

/**
 * @brief Class Container
 */
class Container {
private:
    //! Name of container
    std::string name;
    //! Mass of container
    double mass;
    //! Volume of container
    double volume;
    //! Category of container
    CargoCategory category;
public:
    Container();
    Container(std::string, double, double, CargoCategory);
    Container(double, double);

    // getters
    /**
    * @brief Getting name of container
    */
    std::string getName() const { return this->name; }
    /**
    * @brief Getting mass of container
    */
    double getMass() const { return mass; }
    /**
    * @brief Getting volume of container
    */
    double getVolume() const { return volume; }
    /**
    * @brief Getting category of container
    */
    int getCategory() const { return category; }

    // setters
    Container &setName(std::string);
    Container &setMass(double);
    Container &setVolume(double);
    Container &setCategory(CargoCategory);

    friend std::istream &operator>>(std::istream &is, Container &container);
    friend std::ostream &operator<<(std::ostream &is, const Container &container);

    void transferCargo(double, Container &);
    void operator>>(Container &);
    const double getDensity();
    void addCargo(double);
};


#endif //LAB2_CONTAINER_H
