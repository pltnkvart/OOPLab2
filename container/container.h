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

class Container {
private:
    std::string name;
    double mass;
    double volume;
    CargoCategory category;
public:
    Container();

    // конструктор экземпляра класса с инициализацией наименования, массы, объёма и категории;
    Container(std::string, double, double, CargoCategory);

    //конструктор экземпляра класса с инициализацией только массой и объёмом
    Container(double, double);

    // getters
    std::string getName() { return name; }

    double getMass() { return mass; }

    double getVolume() { return volume; }

    int getCategory() { return category; }

    // setters
    Container &setName(std::string);

    Container &setMass(double);

    Container &setVolume(double);

    Container &setCategory(CargoCategory);

    //  output
    void print();

    //  перегрузка заданной массы груза из одного контейнера в другой;
    void transferCargo(double, Container &);

    //  (>>) перегрузка всего груза из одного контейнера в другой;
    void operator>>(Container &);

    // получение плотности перевозимого груза
    const double getDensity();

    //  добавление указанной массы груза в контейнер
    void addCargo(double);
};


#endif //LAB2_CONTAINER_H
