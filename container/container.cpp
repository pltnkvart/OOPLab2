#include "container.h"
#include "../input/inputFunc.h"


Container::Container() : mass(0), volume(0), category(EMPTY) {};

// конструктор экземпляра класса с инициализацией наименования, массы, объёма и категории;
Container::Container(const std::string p_name,
                     const double p_volume,
                     const double p_mass,
                     const CargoCategory p_category) :
        name(p_name),
        mass(p_mass),
        volume(p_volume),
        category(p_category) {}

//конструктор экземпляра класса с инициализацией только массой и объёмом
Container::Container(double p_mass, double p_volume) : name("Undefined"),
                                                       mass(p_mass),
                                                       volume(p_volume),
                                                       category(DANGEROUS) {}

void Container::print() {
    std::cout << "'" << name << "' " << "Mass - " << mass << " Volume - " << volume
              << " Category - "
              << convertToenum(category)
              << std::endl;
}

// setters
Container &Container::setName(std::string p_name) {
    name = p_name;
    return *this;
}

Container &Container::setMass(double p_mass) {
    mass = p_mass;
    return *this;
}

Container &Container::setVolume(double p_volume) {
    volume = p_volume;
    return *this;
}

Container &Container::setCategory(CargoCategory p_category) {
    category = p_category;
    return *this;
}

//    перегрузка заданной массы груза из одного контейнера в другой;
void Container::transferCargo(double massToTransfer, Container &otherContainer) {
    if (category == DANGEROUS || otherContainer.category == DANGEROUS) {
        std::cout << "Нельзя объединять опасные грузы!" << std::endl;
        return;
    }
    if (category != EMPTY && otherContainer.category != EMPTY && category != otherContainer.category) {
        std::cout << "Нельзя объединять грузы разных категорий." << std::endl;
        return;
    }
    if (name != otherContainer.name && (category == BULK || category == LIQUID || category == GAS)) {
        std::cout << "Нельзя объединять насыпные грузы, жидкости и газы разных наименований." << std::endl;
        return;
    }
    if (otherContainer.category == EMPTY) {
        otherContainer.name = name;
        otherContainer.category = category;
        category = EMPTY;
        name = "";
    }
    mass -= massToTransfer;
    otherContainer.mass += massToTransfer;
    volume -= getDensity() / mass;
    otherContainer.volume += getDensity() / mass;
}

//    (>>) перегрузка всего груза из одного контейнера в другой;
void Container::operator>>(Container &otherContainer) {
    transferCargo(mass, otherContainer);
}

// получение плотности перевозимого груза
const double Container::getDensity() {
    if (volume == 0) return 0;
    return mass / volume;
}

//    добавление указанной массы груза в контейнер
void Container::addCargo(double addedMass) {
    double addedVolume = addedMass / getDensity();
    volume += addedVolume;
    mass += addedMass;
}
