#pragma once
#pragma execution_character_set("utf-8")
#include "Carinfo.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

class CarPool {
    private:
        std::map<std::string, Car> carpool_byid;
        std::map<Color, Car> carpool_bycolor;
        std::map<std::string, Car> carpool_bytype;

    public:
        CarPool();
        CarPool(Car* begin, Car* end);
        CarPool(std::vector<Car> cars);
        CarPool(const CarPool& cp);
        ~CarPool();
        void addCar(const Car& car);
        void removeCar(const Car& car);
        void removeCar(const std::string& id);
        void updateCar(const Car& original_car, const Car& new_car);
        Car getCarbyId(const std::string& id) const;
        std::vector<Car> getCarbyColor(const Color& color) const;
        std::vector<Car> getCarbyType(const std::string& type) const;
};
