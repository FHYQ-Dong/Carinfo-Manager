#include "CarPool.hpp"

Color::Color() : red(0), green(0), blue(0) {
    // Default constructor
}

Color::Color(char r, char g, char b) : red(r), green(g), blue(b) {
    // Parameterized constructor
}

Color::Color(const Color& c) : red(c.red), green(c.green), blue(c.blue) {
    // Copy constructor
}

Color::~Color() {
    // Destructor
}

bool Color::operator == (const Color& c) const {
    return (red == c.red && green == c.green && blue == c.blue);
}

bool Color::operator != (const Color& c) const {
    return (red != c.red || green != c.green || blue != c.blue);
}

bool Color::operator < (const Color& c) const {
    if (red < c.red) return true;
    else if (red == c.red) {
        if (green < c.green) return true;
        else if (green == c.green) {
            if (blue < c.blue) return true;
        }
    }
    return false;
}

bool Color::operator > (const Color& c) const {
    if (red > c.red) return true;
    else if (red == c.red) {
        if (green > c.green) return true;
        else if (green == c.green) {
            if (blue > c.blue) return true;
        }
    }
    return false;
}

bool Color::operator <= (const Color& c) const {
    if (red < c.red) return true;
    else if (red == c.red) {
        if (green < c.green) return true;
        else if (green == c.green) {
            if (blue < c.blue) return true;
            else if (blue == c.blue) return true;
        }
    }
    return false;
}

bool Color::operator >= (const Color& c) const {
    if (red > c.red) return true;
    else if (red == c.red) {
        if (green > c.green) return true;
        else if (green == c.green) {
            if (blue > c.blue) return true;
            else if (blue == c.blue) return true;
        }
    }
    return false;
}

Car::Car() {
    car_id = "N/A";
    car_type = "N/A";
    car_color.red = 0;
    car_color.green = 0;
    car_color.blue = 0;
}

Car::Car(std::string id, std::string type, Color c) {
    car_id = id;
    car_type = type;
    car_color = c;
}

Car::Car(const Car& c) : car_id(c.car_id), car_type(c.car_type), car_color(c.car_color) {
    // Copy constructor
}

Car::~Car() {
    // Destructor
}

void Car::setId(const std::string& id) {
    car_id = id;
}

void Car::setType(const std::string& type) {
    car_type = type;
}

void Car::setColor(const Color& c) {
    car_color = c;
}

std::string Car::getId() const {
    return car_id;
}

std::string Car::getType() const {
    return car_type;
}

Color Car::getColor() const {
    return car_color;
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

const Car Car::NULL_CAR = Car();

CarPool::CarPool() {
    // Default constructor
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::map<Color, Car>();
    carpool_bytype = std::map<std::string, Car>();
    sz = 0;
}

CarPool::CarPool(Car* begin, Car* end) {
    // Parameterized constructor
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::map<Color, Car>();
    carpool_bytype = std::map<std::string, Car>();
    sz = 0;
    for (Car* i = begin; i != end; i++) addCar(*i);
}

CarPool::CarPool(std::vector<Car> cars) {
    // Parameterized constructor
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::map<Color, Car>();
    carpool_bytype = std::map<std::string, Car>();
    sz = 0;
    for (Car car : cars) addCar(car);
}

CarPool::CarPool(const CarPool& cp) {
    // Copy constructor
    carpool_byid = cp.carpool_byid;
    carpool_bycolor = cp.carpool_bycolor;
    carpool_bytype = cp.carpool_bytype;
    sz = cp.sz;
}

CarPool::~CarPool() {
    // Destructor
    carpool_byid.clear();
    carpool_bycolor.clear();
    carpool_bytype.clear();
    sz = 0;
}

int CarPool::addCar(const Car& car) {
    try {
        if (carpool_byid.find(car.getId()) != carpool_byid.end()) return 1;
        carpool_byid[car.getId()] = car;
        carpool_bycolor[car.getColor()] = car;
        carpool_bytype[car.getType()] = car;
        sz++;
        return 0;
    }
    catch (...) {
        return 1;
    }
}

int CarPool::removeCar(const Car& car) {
    return removeCar(car.getId());
}

int CarPool::removeCar(const std::string& id) {
    try{
        auto it_id = carpool_byid.find(id);
        if (it_id == carpool_byid.end()) return 1;

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
        sz--;
        return 0;
    }
    catch (...) {
        return 1;
    }
}

int CarPool::updateCar(const Car& original_car, const Car& new_car) {
    try {
        if (removeCar(original_car) == 1) return 1;
        if (addCar(new_car) == 1) return 1;
        return 0;
    }
    catch (...) {
        return 1;
    }
}

Car CarPool::getCarbyId(const std::string& id) const {
    if (carpool_byid.find(id) == carpool_byid.end()) return Car::NULL_CAR;
    return carpool_byid.at(id);
}

CarPool CarPool::getCarbyColor(const Color& color) const {
    CarPool cars;
    auto it_bg = carpool_bycolor.lower_bound(color), it_ed = carpool_bycolor.upper_bound(color);
    for (auto it = it_bg; it != it_ed; it++) cars.addCar(it->second);
    return cars;
}

CarPool CarPool::getCarbyType(const std::string& type) const {
    CarPool cars;
    auto it_bg = carpool_bytype.lower_bound(type), it_ed = carpool_bytype.upper_bound(type);
    for (auto it = it_bg; it != it_ed; it++) cars.addCar(it->second);
    return cars;
}

size_t CarPool::size() const {
    return sz;
}

bool CarPool::empty() const {
    return sz == 0;
}

bool CarPool::operator == (const CarPool& cp) const {
    return sz == cp.sz && cp.carpool_byid == cp.carpool_byid && carpool_bycolor == cp.carpool_bycolor && carpool_bytype == cp.carpool_bytype;
}

bool CarPool::operator != (const CarPool& cp) const {
    return sz != cp.sz || cp.carpool_byid != cp.carpool_byid || carpool_bycolor != cp.carpool_bycolor || carpool_bytype != cp.carpool_bytype;
}

CarPool& CarPool::operator = (const CarPool& cp) {
    sz = cp.sz;
    carpool_byid = cp.carpool_byid;
    carpool_bycolor = cp.carpool_bycolor;
    carpool_bytype = cp.carpool_bytype;
    return *this;
}
