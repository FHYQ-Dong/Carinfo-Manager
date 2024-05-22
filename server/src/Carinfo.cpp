#include "Carinfo.h"

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