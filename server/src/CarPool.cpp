#include "CarPool.h"

CarPool::CarPool() {
    // Default constructor
}

CarPool::CarPool(Car* begin, Car* end) {
    // Parameterized constructor
    for (Car* i = begin; i != end; i++) addCar(*i);
}

CarPool::CarPool(std::vector<Car> cars) {
    // Parameterized constructor
    for (Car car : cars) addCar(car);
}

CarPool::CarPool(const CarPool& cp) {
    // Copy constructor
    carpool_byid = cp.carpool_byid;
    carpool_bycolor = cp.carpool_bycolor;
    carpool_bytype = cp.carpool_bytype;
}

CarPool::~CarPool() {
    // Destructor
}

void CarPool::addCar(const Car& car) {
    carpool_byid[car.getId()] = car;
    carpool_bycolor[car.getColor()] = car;
    carpool_bytype[car.getType()] = car;
}

void CarPool::removeCar(const Car& car) {
    auto it_id = carpool_byid.find(car.getId());
    if (it_id == carpool_byid.end()) return;

    carpool_byid.erase(it_id);
    auto it_cl_bg = carpool_bycolor.lower_bound(car.getColor()), it_cl_ed = carpool_bycolor.upper_bound(car.getColor());
    for (auto it_cl = it_cl_bg; it_cl != it_cl_ed; it_cl++) {
        if (it_cl->second.getId() == car.getId()) {
            carpool_bycolor.erase(it_cl);
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
}

void CarPool::removeCar(const std::string& id) {
    auto it_id = carpool_byid.find(id);
    if (it_id == carpool_byid.end()) return;

    Car car = it_id->second;
    carpool_byid.erase(it_id);
    auto it_cl_bg = carpool_bycolor.lower_bound(car.getColor()), it_cl_ed = carpool_bycolor.upper_bound(car.getColor());
    for (auto it_cl = it_cl_bg; it_cl != it_cl_ed; it_cl++) {
        if (it_cl->second.getId() == car.getId()) {
            carpool_bycolor.erase(it_cl);
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
}

void CarPool::updateCar(const Car& original_car, const Car& new_car) {
    removeCar(original_car);
    addCar(new_car);
}

Car CarPool::getCarbyId(const std::string& id) const {
    return carpool_byid.at(id);
}

std::vector<Car> CarPool::getCarbyColor(const Color& color) const {
    std::vector<Car> cars;
    auto it_bg = carpool_bycolor.lower_bound(color), it_ed = carpool_bycolor.upper_bound(color);
    for (auto it = it_bg; it != it_ed; it++) cars.push_back(it->second);
    return cars;
}

std::vector<Car> CarPool::getCarbyType(const std::string& type) const {
    std::vector<Car> cars;
    auto it_bg = carpool_bytype.lower_bound(type), it_ed = carpool_bytype.upper_bound(type);
    for (auto it = it_bg; it != it_ed; it++) cars.push_back(it->second);
    return cars;
}
