#include "carinfo-manager/carpool.hpp"
#include <fstream>

Color::Color() : red(-1), green(-1), blue(-1) {
    // Default constructor
}

Color::Color(char r, char g, char b) : red(r), green(g), blue(b) {
    // Parameterized constructor
}

Color::Color(const std::string& color_string) {
    // Parameterized constructor
    if (color_string[0] == '(' && color_string[color_string.length() - 1] == ')') {
        std::string color_str = color_string.substr(1, color_string.length() - 2);
        size_t pos = color_str.find(',');
        if (pos == std::string::npos) {
            red = green = blue = -1;
            return;
        }
        std::string red_str = color_str.substr(0, pos);
        color_str = color_str.substr(pos + 1);
        pos = color_str.find(',');
        if (pos == std::string::npos) {
            red = green = blue = -1;
            return;
        }
        std::string green_str = color_str.substr(0, pos);
        std::string blue_str = color_str.substr(pos + 1);
        try {
            red = std::stoi(red_str);
            green = std::stoi(green_str);
            blue = std::stoi(blue_str);
        }
        catch (...) {
            red = green = blue = -1;
        }
    }
    else {
        red = green = blue = -1;
    }
}

Color::Color(const Color& c) : red(c.red), green(c.green), blue(c.blue) {
    // Copy constructor
}

Color::~Color() {
    // Destructor
}

const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::Cyan = Color(0, 255, 255);
const Color Color::Magenta = Color(255, 0, 255);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
const Color Color::Gray = Color(128, 128, 128);
const Color Color::LightGray = Color(192, 192, 192);
const Color Color::DarkGray = Color(64, 64, 64);
const Color Color::Brown = Color(165, 42, 42);
const Color Color::Orange = Color(255, 165, 0);
const Color Color::Pink = Color(255, 192, 203);
const Color Color::Purple = Color(128, 0, 128);
const Color Color::LightBlue = Color(173, 216, 230);
const Color Color::LightGreen = Color(144, 238, 144);
const Color Color::LightYellow = Color(255, 255, 224);
const Color Color::LightCyan = Color(224, 255, 255);
const Color Color::LightMagenta = Color(255, 224, 255);
const Color Color::NULL_COLOR = Color();

Color::operator std::string() const {
    return "(" + std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue) + ")";
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

Color& Color::operator = (const Color& c) {
    red = c.red;
    green = c.green;
    blue = c.blue;
    return *this;
}

Car::Car() {
    car_id = "N/A";
    car_type = "N/A";
    car_color.red = 0;
    car_color.green = 0;
    car_color.blue = 0;
    car_year = 0;
    car_img_path = "N/A";
}

Car::Car(const std::string& id, const std::string& type, const Color& c, const int year, const std::string& img_path) {
    car_id = id;
    car_type = type;
    car_color = c;
    car_year = year;
    car_img_path = img_path;
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

Color Car::getColor() const {
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
    car_color = c.car_color;
    car_year = c.car_year;
    car_img_path = c.car_img_path;
    return *this;
}

const Car Car::NULL_CAR = Car();

CarPool::CarPool() {
    // Default constructor
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::multimap<Color, Car>();
    carpool_bytype = std::multimap<std::string, Car>();
    sz = 0;
}

CarPool::CarPool(Car* begin, Car* end) {
    // Parameterized constructor
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::multimap<Color, Car>();
    carpool_bytype = std::multimap<std::string, Car>();
    sz = 0;
    for (Car* i = begin; i != end; i++) addCar(*i);
}

CarPool::CarPool(const std::vector<Car>& cars) {
    // Parameterized constructor
    carpool_byid = std::map<std::string, Car>();
    carpool_bycolor = std::multimap<Color, Car>();
    carpool_bytype = std::multimap<std::string, Car>();
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
        if (carpool_byid.find(car.getId()) != carpool_byid.end()) return 0x70;
        carpool_byid[car.getId()] = car;
        carpool_bycolor.insert(std::make_pair(car.getColor(), car));
        carpool_bytype.insert(std::make_pair(car.getType(), car));
        sz++;
        return 0;
    }
    catch (...) {
        return 0x7F;
    }
}

int CarPool::removeCar(const Car& car) {
    return removeCar(car.getId());
}

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

CarPool CarPool::getCarbyId(const std::string& id) const {
    CarPool cars;
    if (carpool_byid.find(id) != carpool_byid.end())
        cars.addCar(carpool_byid.at(id));
    return cars;
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

CarPool CarPool::getCar(const std::string& id, const Color& color, const std::string& type) const {
    std::string _id = id; Color _color = color; std::string _type = type;
    CarPool cars = *this;
    begin:
    if (_id != "") {
        cars = cars.getCarbyId(_id);
        _id = "";
        goto begin;
    }
    else if (_color != Color::NULL_COLOR) {
        cars = cars.getCarbyColor(_color);
        _color = Color::NULL_COLOR;
        goto begin;
    }
    else if (_type != "") {
        cars = cars.getCarbyType(_type);
        _type = "";
        goto begin;
    }
    else return cars;
}

size_t CarPool::size() const {
    return sz;
}

bool CarPool::empty() const {
    return sz == 0;
}

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

int CarPool::load(std::istream& is) {
    if (!is) return 0xB0;
    try {
        if(clear()) return 0xB1;
        // Read size
        size_t sz_tmp = 0;
        is.read((char*)&sz_tmp, sizeof(size_t));
        for (size_t i = 0; i < sz_tmp; i++) {
            // Load one Car
            std::string id, type, img_path;
            Color color;
            int year;
            // read car_id
            size_t id_len = 0;
            is.read((char*)&id_len, sizeof(size_t));
            char* id_buf = new char[id_len + 1];
            is.read(id_buf, id_len * sizeof(char));
            id_buf[id_len] = '\0';
            id = id_buf;
            delete[] id_buf;
            // read car_type
            size_t type_len = 0;
            is.read((char*)&type_len, sizeof(size_t));
            char* type_buf = new char[type_len + 1];
            is.read(type_buf, type_len * sizeof(char));
            type_buf[type_len] = '\0';
            type = type_buf;
            delete[] type_buf;
            // read car_color
            is.read((char*)&color, sizeof(Color));
            // read car_year
            is.read((char*)&year, sizeof(int));
            // read car_img_path
            size_t img_path_len = 0;
            is.read((char*)&img_path_len, sizeof(size_t));
            char* img_path_buf = new char[img_path_len + 1];
            is.read(img_path_buf, img_path_len * sizeof(char));
            img_path_buf[img_path_len] = '\0';
            img_path = img_path_buf;
            delete[] img_path_buf;
            Car car(id, type, color, year, img_path);
            if (addCar(car)) return 0xB3;
        }
        return 0;
    }
    catch (...) {
        return 0xBF;
    }
}

int CarPool::save(std::ostream& os) const {
    if (!os) return 0xC0;
    try {
        // Write size
        os.write((char*)&sz, sizeof(size_t));
        for (auto it = carpool_byid.begin(); it != carpool_byid.end(); it++) {
            // Save one Car
            Car car = it->second;
            // write car_id
            size_t id_len = car.getId().length();
            os.write((char*)&id_len, sizeof(size_t));
            os.write(car.getId().c_str(), id_len * sizeof(char));
            // write car_type
            size_t type_len = car.getType().length();
            os.write((char*)&type_len, sizeof(size_t));
            os.write(car.getType().c_str(), type_len * sizeof(char));
            // write car_color
            Color color = car.getColor();
            os.write((char*)&color, sizeof(Color));
            // write car_year
            int year = car.getYear();
            os.write((char*)&year, sizeof(int));
            // write car_img_path
            size_t img_path_len = car.getImagePath().length();
            os.write((char*)&img_path_len, sizeof(size_t));
            os.write(car.getImagePath().c_str(), img_path_len * sizeof(char));
        }
        return 0;
    }
    catch (...) {
        return 0xCF;
    }
}

std::vector<Car> CarPool::list() const {
    std::vector<Car> cars;
    for (auto it = carpool_byid.begin(); it != carpool_byid.end(); it++) cars.push_back(it->second);
    return cars;
}

CarPool::operator std::vector<Car>() const {
    return list();
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
