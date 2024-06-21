#pragma once
#pragma execution_character_set("utf-8")
#include "BasicPool.hpp"
#include <map>
#include <set>
#include <string>
#include <cstdint>
#include <vector>

class Color {
    public:
        char red, green, blue;
        Color();
        Color(char r, char g, char b);
        Color(const Color& c);
        ~Color();
        bool operator == (const Color& c) const;
        bool operator != (const Color& c) const;
        bool operator < (const Color& c) const;
        bool operator > (const Color& c) const;
        bool operator <= (const Color& c) const;
        bool operator >= (const Color& c) const;
};

class Car {
    private:
        std::string car_id;
        std::string car_type;
        Color car_color;

    public:
        Car();
        Car(std::string id, std::string type, Color c);
        Car(const Car& c);
        ~Car();
        void setId(const std::string& id);
        void setType(const std::string& type);
        void setColor(const Color& c);
        std::string getId() const;
        std::string getType() const;
        Color getColor() const;

        bool operator == (const Car& c) const;
        bool operator != (const Car& c) const;
        bool operator < (const Car& c) const;
        bool operator > (const Car& c) const;
        bool operator <= (const Car& c) const;
        bool operator >= (const Car& c) const;
        const static Car NULL_CAR;
};

class CarPool : public BasicPool {
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
        int addCar(const Car& car);
        int removeCar(const Car& car);
        int removeCar(const std::string& id);
        int updateCar(const Car& original_car, const Car& new_car);
        Car getCarbyId(const std::string& id) const;
        CarPool getCarbyColor(const Color& color) const;
        CarPool getCarbyType(const std::string& type) const;
        size_t size() const;
        bool empty() const ;
        int clear();
        int load(std::string filename);
        int save(std::string filename) const;
        std::vector<Car> list() const;

        bool operator == (const CarPool& cp) const;
        bool operator != (const CarPool& cp) const;
        CarPool& operator = (const CarPool& cp);
};
