#include "container.h"
#include "../input/inputFunc.h"

/**
 * @brief Empty constructor
 */
Container::Container() : mass(0), volume(0), category(EMPTY) {};

/**
 * @brief Constructor with initialization of name, mass, volume and category
 * @param p_name Name
 * @param p_volume Volume
 * @param p_mass Mass
 * @param p_category Category
 */
Container::Container(const std::string p_name,
                     const double p_volume,
                     const double p_mass,
                     const CargoCategory p_category) :
        name(p_name),
        mass(p_mass),
        volume(p_volume),
        category(p_category) {}

/**
 * @brief Constructor with initialization of mass and volume
 * @param p_mass Mass
 * @param p_volume Volume
 */
Container::Container(double p_mass, double p_volume) : name("Undefined"),
                                                       mass(p_mass),
                                                       volume(p_volume),
                                                       category(DANGEROUS) {}

/**
 * @brief Overload output function
 * @param is Stream from which data is entered
 * @param container Readable structure
 * @return Stream
 */
std::ostream &operator<<(std::ostream &is, const Container &container) {
    is << container.name << " Mass - " << container.mass << " Volume - " << container.volume
       << " Category - " << convertToenum(container.category) << std::endl;
    return is;
}


/**
 * @brief Setting name
 */
Container &Container::setName(std::string p_name) {
    name = p_name;
    return *this;
}

/**
 * @brief Setting mass
 */
Container &Container::setMass(double p_mass) {
    mass = p_mass;
    return *this;
}

/**
 * @brief Setting volume
 */
Container &Container::setVolume(double p_volume) {
    volume = p_volume;
    return *this;
}

/**
 * @brief Setting cargo category
 */
Container &Container::setCategory(CargoCategory p_category) {
    category = p_category;
    return *this;
}

/**
 * @brief Overload input function
 * @param is Stream into which data is entered
 * @param container Writable structure
 * @return Stream
 */
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

/**
 * @brief Overloading a given mass of cargo from one container to another
 * @param massToTransfer Mass of cargo to overload
 * @throw std::runtime_error If not transition not possible
 * @param otherContainer Another container to overload
 */
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

/**
 * @brief Overload all the cargo from one container to another
 * @param otherContainer Another container to overload
 */
void Container::operator>>(Container &otherContainer) {
    transferCargo(mass, otherContainer);
}

/**
 * @brief Getting the density of container
 * @return Density
 */
const double Container::getDensity() {
    if (volume == 0) return 0;
    return mass / volume;
}

/**
 * @brief Adding the mass of cargo to the container
 * @param addedMass Mass of cargo to be added
 */
void Container::addCargo(double addedMass) {
    double addedVolume = addedMass / getDensity();
    volume += addedVolume;
    mass += addedMass;
}
