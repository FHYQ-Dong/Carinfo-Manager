/**
 * @file src/carpool.cpp
 * @brief Implementation of class Car, class Color, and class CarPool
 * 
 * @details
 * This file contains the implementation of the Car, Color, and CarPool classes.
 * The Car class represents a car object with properties such as ID, type, color, year, and image path.
 * The Color class represents a color object with RGB values.
 * The CarPool class represents a collection of cars and provides operations to add and remove cars.
 * It also provides iterators to iterate over the cars in different orders (by ID, color, or type).
 * The implementation of these classes is provided in this file.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "carinfo-manager/carpool.hpp"
#include <fstream>
#include "json/json.hpp"
using nlohmann::json;

Car::Car() {
    car_id = "";
    car_type = "";
    car_color = "";
    car_owner = "";
    car_year = 0;
    car_img_path = "";
}

Car::Car(const std::string& id, const std::string& type, const std::string& owner, \
    const std::string& color, const int year, const std::string& img_path) {
    car_id = id;
    car_type = type;
    car_owner = owner;
    car_color = color;
    car_year = year;
    car_img_path = img_path;
}

Car::Car(const Car& c) : \
    car_id(c.car_id), car_type(c.car_type), car_color(c.car_color), \
    car_owner(c.car_owner), car_year(c.car_year), car_img_path(c.car_img_path) {}

Car::~Car() {}

void Car::setId(const std::string& id) {
    car_id = id;
}

void Car::setType(const std::string& type) {
    car_type = type;
}

void Car::setOwner(const std::string& owner) {
    car_owner = owner;
}

void Car::setColor(const std::string& color) {
    car_color = color;
}

void Car::setYear(const int year) {
    car_year = year;
}

void Car::setImagePath(const std::string& img_path) {
    car_img_path = img_path;
}

std::string Car::getId() const {
    return car_id;
}

std::string Car::getType() const {
    return car_type;
}

std::string Car::getOwner() const {
    return car_owner;
}

std::string Car::getColor() const {
    return car_color;
}

int Car::getYear() const {
    return car_year;
}

std::string Car::getImagePath() const {
    return car_img_path;
}

bool Car::operator == (const Car& c) const {
    return car_id == c.car_id;
}

bool Car::operator != (const Car& c) const {
    return car_id != c.car_id;
}

bool Car::operator < (const Car& c) const {
    return car_id < c.car_id;
}

bool Car::operator > (const Car& c) const {
    return car_id > c.car_id;
}

bool Car::operator <= (const Car& c) const {
    return car_id <= c.car_id;
}

bool Car::operator >= (const Car& c) const {
    return car_id >= c.car_id;
}

Car& Car::operator = (const Car& c) {
    car_id = c.car_id;
    car_type = c.car_type;
    car_owner = c.car_owner;
    car_color = c.car_color;
    car_year = c.car_year;
    car_img_path = c.car_img_path;
    return *this;
}

const Car Car::NULL_CAR = Car();

/**
 * @brief Constructs a new CarPool object.
 * 
 * This constructor initializes the CarPool object by creating empty maps for carpool_byid, carpool_bycolor, and carpool_bytype.
 * It also sets the initial size of the carpool to 0.
 */
CarPool::CarPool() {
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::multimap<std::string, Car>();
    carpool_bytype = std::multimap<std::string, Car>();
    carpool_byowner = std::multimap<std::string, Car>();
    sz = 0;
}

/**
 * @brief Constructs a new CarPool object with cars in the range [begin, end).
 * 
 * This constructor initializes the CarPool object by adding cars in the range [begin, end) to the carpool.
 * 
 * @param begin An iterator pointing to the first car in the range.
 * @param end An iterator pointing to the end of the range.
 */
CarPool::CarPool(Car* begin, Car* end) {
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::multimap<std::string, Car>();
    carpool_bytype = std::multimap<std::string, Car>();
    carpool_byowner = std::multimap<std::string, Car>();
    sz = 0;
    for (Car* i = begin; i != end; i++) addCar(*i);
}

/**
 * @brief Constructs a new CarPool object with cars in the vector.
 * 
 * This constructor initializes the CarPool object by adding cars in the vector to the carpool.
 * 
 * @param cars A vector containing the cars to be added to the carpool.
 */
CarPool::CarPool(const std::vector<Car>& cars) {
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::multimap<std::string, Car>();
    carpool_bytype = std::multimap<std::string, Car>();
    carpool_byowner = std::multimap<std::string, Car>();
    sz = 0;
    for (Car car : cars) addCar(car);
}

/**
 * @brief Constructs a new CarPool object by copying an existing CarPool object.
 * 
 * This constructor initializes the CarPool object by copying the contents of the provided CarPool object.
 * 
 * @param cp The CarPool object to be copied.
 */
CarPool::CarPool(const CarPool& cp) {
    carpool_byid = cp.carpool_byid;
    carpool_bycolor = cp.carpool_bycolor;
    carpool_bytype = cp.carpool_bytype;
    carpool_byowner = cp.carpool_byowner;
    sz = cp.sz;
}

/**
 * @brief Destroys the CarPool object.
 * 
 * This destructor clears the carpool data by removing all cars from the carpool containers.
 */
CarPool::~CarPool() {
    carpool_byid.clear();
    carpool_bycolor.clear();
    carpool_bytype.clear();
    carpool_byowner.clear();
    sz = 0;
}

/**
 * Adds a car to the carpool.
 * 
 * @param car The car object to be added.
 * @return Returns 0 if the car is added successfully, else an error code:
 *         - 0x70: If the car with the same ID already exists in the carpool.
 *         - 0x7F: If an unknown exception occurs while adding the car.
 */
int CarPool::addCar(const Car& car) {
    try {
        if (carpool_byid.find(car.getId()) != carpool_byid.end()) return 0x70;
        carpool_byid[car.getId()] = car;
        carpool_bycolor.insert(std::make_pair(car.getColor(), car));
        carpool_bytype.insert(std::make_pair(car.getType(), car));
        carpool_byowner.insert(std::make_pair(car.getOwner(), car));
        sz++;
        return 0;
    }
    catch (...) {
        return 0x7F;
    }
}

/**
 * Removes a car from the car pool.
 *
 * @param car The car to be removed.
 * @return The number of cars removed (0 or 1).
 */
int CarPool::removeCar(const Car& car) {
    return removeCar(car.getId());
}


/**
 * @brief Removes a car from the carpool based on its ID.
 * 
 * This function removes a car from the carpool based on the provided ID. It searches for the car with the given ID in the carpool_byid map and removes it if found. Additionally, it removes the car from the carpool_bycolor and carpool_bytype maps if it exists in those maps as well. Finally, it decrements the size of the carpool by one.
 * 
 * @param id The ID of the car to be removed.
 * @return Returns 0 if the car was successfully removed, else an error code:
 *        - 0x80: If the car with the specified ID does not exist in the carpool.
 *        - 0x8F: If an unknown exception occurs while removing the car.
 */
int CarPool::removeCar(const std::string& id) {
    try{
        auto it_id = carpool_byid.find(id);
        if (it_id == carpool_byid.end()) return 0x80;

        Car car = it_id->second;
        carpool_byid.erase(it_id);
        auto it_cl_bg = carpool_bycolor.lower_bound(car.getColor()), it_cl_ed = carpool_bycolor.upper_bound(car.getColor());
        for (auto it_cl = it_cl_bg; it_cl != it_cl_ed; it_cl++) {
            if (it_cl->second.getId() == car.getId()) {
                carpool_bycolor.erase(it_cl);
                break;
            }
        }
        auto it_on_bg = carpool_byowner.lower_bound(car.getOwner()), it_on_ed = carpool_byowner.upper_bound(car.getOwner());
        for (auto it_on = it_on_bg; it_on != it_on_ed; it_on++) {
            if (it_on->second.getId() == car.getId()) {
                carpool_byowner.erase(it_on);
                break;
            }
        }
        auto it_ty_bg = carpool_bytype.lower_bound(car.getType()), it_ty_ed = carpool_bytype.upper_bound(car.getType());
        for (auto it_ty = it_ty_bg; it_ty != it_ty_ed; it_ty++) {
            if (it_ty->second.getId() == car.getId()) {
                carpool_bytype.erase(it_ty);
                break;
            }
        }
        sz--;
        return 0;
    }
    catch (...) {
        return 0x8F;
    }
}

/**
 * @brief Updates a car in the car pool.
 * 
 * This function replaces the original car with the new car in the car pool.
 * It first removes the original car from the car pool using the removeCar function,
 * and then adds the new car to the car pool using the addCar function.
 * 
 * @param original_car The original car to be replaced.
 * @param new_car The new car to replace the original car.
 * @return Returns 0 if the update is successful, else an error code:
 *         - 0x90: If the removal of the original car is successful but the addition of the new car fails.
 *         - 0x91: If the addition of the new car fails.
 *         - 0x9F: If an exception occurs during the update process.
 */
int CarPool::updateCar(const Car& original_car, const Car& new_car) {
    try {
        if (removeCar(original_car)) return 0x90;
        if (addCar(new_car)) return 0x91;
        return 0;
    }
    catch (...) {
        return 0x9F;
    }
}

/**
 * @brief Updates a car in the car pool.
 * 
 * This function updates a car in the car pool by first removing the car with the specified ID and then adding the new car.
 * 
 * @param id The ID of the car to be updated.
 * @param new_car The new car object to replace the existing car.
 * @return Returns 0 if the car was successfully updated, else an error code:
 *         - 0x90: If the removal of the original car is successful but the addition of the new car fails.
 *         - 0x91: If the addition of the new car fails.
 *         - 0x9F: If an exception occurs during the update process.
 */
int CarPool::updateCar(const std::string& id, const Car& new_car) {
    try {
        if (removeCar(id)) return 0x90;
        if (addCar(new_car)) return 0x91;
        return 0;
    }
    catch (...) {
        return 0x9F;
    }
}

/**
 * Retrieves a CarPool object containing the car with the specified ID.
 *
 * @param id The ID of the car to retrieve.
 * @return A CarPool object containing the car with the specified ID, or an empty CarPool object if the car is not found.
 */
CarPool CarPool::getCarbyId(const std::string& id) const {
    CarPool cars;
    if (carpool_byid.find(id) != carpool_byid.end())
        cars.addCar(carpool_byid.at(id));
    return cars;
}

/**
 * Retrieves a CarPool object containing all cars with the specified color.
 *
 * @param color The color of the cars to retrieve.
 * @return A CarPool object containing all cars with the specified color.
 */
CarPool CarPool::getCarbyColor(const std::string& color) const {
    CarPool cars;
    auto it_bg = carpool_bycolor.lower_bound(color), it_ed = carpool_bycolor.upper_bound(color);
    for (auto it = it_bg; it != it_ed; it++) cars.addCar(it->second);
    return cars;
}

/**
 * Retrieves a CarPool object containing all cars owned by a specific owner.
 *
 * @param owner The owner of the cars to retrieve.
 * @return A CarPool object containing all cars owned by the specified owner.
 */
CarPool CarPool::getCarbyOwner(const std::string& owner) const {
    CarPool cars;
    auto it_bg = carpool_byowner.lower_bound(owner), it_ed = carpool_byowner.upper_bound(owner);
    for (auto it = it_bg; it != it_ed; it++) cars.addCar(it->second);
    return cars;
}

/**
 * Retrieves a CarPool object containing all cars of a specific type.
 *
 * @param type The type of cars to retrieve.
 * @return A CarPool object containing all cars of the specified type.
 */
CarPool CarPool::getCarbyType(const std::string& type) const {
    CarPool cars;
    auto it_bg = carpool_bytype.lower_bound(type), it_ed = carpool_bytype.upper_bound(type);
    for (auto it = it_bg; it != it_ed; it++) cars.addCar(it->second);
    return cars;
}

/**
 * Retrieves a car from the car pool based on the specified criteria.
 *
 * @param id The ID of the car to search for.
 * @param color The color of the car to search for.
 * @param owner The owner of the car to search for.
 * @param type The type of the car to search for.
 * @return The car that matches the specified criteria.
 */
CarPool CarPool::getCar(const std::string& id, const std::string& color, const std::string& owner, const std::string& type) const {
    std::string _id = id, _color = color, _type = type, _owner = owner;
    CarPool cars = *this;
    begin:
    if (_id != "") {
        cars = cars.getCarbyId(_id);
        _id = "";
        goto begin;
    }
    else if (_owner != "") {
        cars = cars.getCarbyOwner(_owner);
        _owner = "";
        goto begin;
    }
    else if (_color != "") {
        cars = cars.getCarbyColor(_color);
        _color = "";
        goto begin;
    }
    else if (_type != "") {
        cars = cars.getCarbyType(_type);
        _type = "";
        goto begin;
    }
    else return cars;
}

/**
 * @brief Retrieves the number of cars in the carpool.
 * 
 * This function returns the number of cars in the carpool.
 * 
 * @return The number of cars in the carpool.
 */
size_t CarPool::size() const {
    return sz;
}

/**
 * @brief Checks if the carpool is empty.
 * 
 * This function checks if the carpool is empty by checking if the size of the carpool is zero.
 * 
 * @return True if the carpool is empty, otherwise false.
 */
bool CarPool::empty() const {
    return sz == 0;
}

/**
 * @brief Clears the carpool data.
 * 
 * This function clears the carpool data by removing all cars from the carpool containers and resetting the size to zero.
 * 
 * @return 0 if the carpool data is cleared successfully, 0xAF if an exception occurs.
 */
int CarPool::clear() {
    try {
        carpool_byid.clear();
        carpool_bycolor.clear();
        carpool_bytype.clear();
        sz = 0;
        return 0;
    }
    catch (...) {
        return 0xAF;
    }
}

/**
 * @brief Loads car data from an input stream.
 * 
 * This function reads car data from the provided input stream and populates the CarPool object with the loaded cars.
 * 
 * @param is The input stream to read car data from.
 * @return Returns 0 if the car data is successfully loaded, otherwise returns an error code:
 *         - 0xB0: If the input stream is not valid.
 *         - 0xB1: If there is an error while clearing the existing car data in the CarPool object.
 *         - 0xB2: If the JSON object is not an object.
 *         - 0xB3: If the JSON object does not contain the required fields.
 *         - 0xB4: If the JSON object contains fields with incorrect types.
 *         - 0xB5: If there is an error while adding a car to the CarPool object.
 *         - 0xBF: If an unknown exception occurs while loading the car data.
 */
int CarPool::load(std::istream& is) {
    if (!is) return 0xB0;
    try {
        if(clear() != 0) return 0xB1;
        json load_json_obj = json::parse(is);
        for (auto it = load_json_obj.begin(); it != load_json_obj.end(); it++) {
            if (!it.value().is_object()) return 0xB2;
            if (!it.value().contains("id") || !it.value().contains("type") || !it.value().contains("owner") || \
                !it.value().contains("color") || !it.value().contains("year") || !it.value().contains("img_path")) 
                return 0xB3;
            if (!it.value()["id"].is_string() || !it.value()["type"].is_string() || !it.value()["owner"].is_string() || \
                !it.value()["color"].is_string() || !it.value()["year"].is_number_integer() || !it.value()["img_path"].is_string())
                return 0xB4;
            Car car(
                std::string(it.value()["id"]), 
                std::string(it.value()["type"]), 
                std::string(it.value()["owner"]), 
                std::string(it.value()["color"]), 
                int(it.value()["year"]), 
                std::string(it.value()["img_path"])
            );
            if (addCar(car)) return 0xB5;
        }
        return 0;
    }
    catch (...) {
        return 0xBF;
    }
}

/**
 * Saves the CarPool object to an output stream.
 * 
 * @param os The output stream to save the CarPool object to.
 * @return Returns 0 if the CarPool object is successfully saved, else an error code:
 *         - 0xC0: If the output stream is not valid.
 *         - 0xCF: If an unknown exception occurs during the saving process.
 */
int CarPool::save(std::ostream& os) const {
    if (!os) return 0xC0;
    try {
        json save_json_obj;
        for (auto it = carpool_byid.begin(); it != carpool_byid.end(); it++) {
            json car_json_obj;
            car_json_obj["id"] = it->second.getId();
            car_json_obj["type"] = it->second.getType();
            car_json_obj["owner"] = it->second.getOwner();
            car_json_obj["color"] = it->second.getColor();
            car_json_obj["year"] = it->second.getYear();
            car_json_obj["img_path"] = it->second.getImagePath();
            save_json_obj[it->first] = car_json_obj;
        }
        os << save_json_obj.dump(4);
        return 0;
    }
    catch (...) {
        return 0xCF;
    }
}

/**
 * Retrieves a list of cars in the carpool.
 *
 * This function returns a vector containing all the cars in the carpool.
 *
 * @return A vector of Car objects representing the cars in the carpool.
 */
std::vector<Car> CarPool::list() const {
    std::vector<Car> cars;
    for (auto it = carpool_byid.begin(); it != carpool_byid.end(); it++) cars.push_back(it->second);
    return cars;
}

/**
 * @brief Compares two CarPool objects for equality.
 * 
 * This function compares two CarPool objects for equality by comparing the size of the carpool and the cars in the carpool.
 * 
 * @param cp The CarPool object to compare with.
 * @return True if the CarPool objects are equal, otherwise false.
 */
bool CarPool::operator == (const CarPool& cp) const {
    return sz == cp.sz && cp.carpool_byid == cp.carpool_byid && carpool_bycolor == cp.carpool_bycolor && carpool_bytype == cp.carpool_bytype;
}

/**
 * @brief Compares two CarPool objects for inequality.
 * 
 * This function compares two CarPool objects for inequality by comparing the size of the carpool and the cars in the carpool.
 * 
 * @param cp The CarPool object to compare with.
 * @return True if the CarPool objects are not equal, otherwise false.
 */
bool CarPool::operator != (const CarPool& cp) const {
    return sz != cp.sz || cp.carpool_byid != cp.carpool_byid || carpool_bycolor != cp.carpool_bycolor || carpool_bytype != cp.carpool_bytype;
}

/**
 * @brief Compares two CarPool objects for less than.
 * 
 * This function compares two CarPool objects for less than by comparing the size of the carpool.
 * 
 * @param cp The CarPool object to compare with.
 * @return True if the size of the carpool is less than the size of the carpool in the provided CarPool object, otherwise false.
 */
CarPool& CarPool::operator = (const CarPool& cp) {
    sz = cp.sz;
    carpool_byid = cp.carpool_byid;
    carpool_bycolor = cp.carpool_bycolor;
    carpool_bytype = cp.carpool_bytype;
    return *this;
}
