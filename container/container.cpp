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


std::ostream &operator<<(std::ostream &is, const Container &container) {
    is << container.name << " Mass - " << container.mass << " Volume - " << container.volume
       << " Category - " << convertToenum(container.category) << std::endl;
    return is;
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

//  input container
std::istream& operator>>(std::istream& is, Container& container) {
    std::string name;
    double mass, volume;
    int category;

    is >> name >> mass >> volume >> category;
    if(is.fail()){
        is.setstate(std::ios::failbit);
        return is;
    }
    container.setName(name);
    container.setMass(mass);
    container.setVolume(volume);
    container.setCategory(CargoCategory(category));
    return is;
}


//    перегрузка заданной массы груза из одного контейнера в другой;
void Container::transferCargo(double massToTransfer, Container &otherContainer) {
    if (category == DANGEROUS || otherContainer.category == DANGEROUS) {
        throw std::runtime_error("Dangerous goods must not be combined!");
    }
    if (category != EMPTY && otherContainer.category != EMPTY && category != otherContainer.category) {
        throw std::runtime_error("Different categories!");
    }
    if (name != otherContainer.name && (category == BULK || category == LIQUID || category == GAS)) {
        throw std::runtime_error("Can't combine bulk cargo, liquids and gases of different types!");
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
