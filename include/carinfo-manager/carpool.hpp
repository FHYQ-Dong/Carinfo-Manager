#pragma once
#pragma execution_character_set("utf-8")
#include "carinfo-manager/basicpool.hpp"
#include <map>
#include <set>
#include <string>
#include <cstdint>
#include <vector>

class Color {
    public:
        short red, green, blue;
        Color();
        Color(char r, char g, char b);
        Color(const std::string& color_string); // format: "(R, G, B)"
        Color(const Color& c);
        ~Color();
        operator std::string() const;
        bool operator == (const Color& c) const;
        bool operator != (const Color& c) const;
        bool operator < (const Color& c) const;
        bool operator > (const Color& c) const;
        bool operator <= (const Color& c) const;
        bool operator >= (const Color& c) const;
        Color& operator = (const Color& c);

        const static Color Red;
        const static Color Green;
        const static Color Blue;
        const static Color Yellow;
        const static Color Cyan;
        const static Color Magenta;
        const static Color White;
        const static Color Black;
        const static Color Gray;
        const static Color LightGray;
        const static Color DarkGray;
        const static Color Brown;
        const static Color Orange;
        const static Color Pink;
        const static Color Purple;
        const static Color LightBlue;
        const static Color LightGreen;
        const static Color LightYellow;
        const static Color LightCyan;
        const static Color LightMagenta;
        const static Color NULL_COLOR;
};

class Car {
    private:
        std::string car_id;
        std::string car_type;
        Color car_color;
        int car_year;
        std::string car_img_path;

    public:
        Car();
        Car(const std::string& id, const std::string& type, const Color& c, const int year, const std::string& img_path);
        Car(const Car& c);
        ~Car();
        void setId(const std::string& id);
        void setType(const std::string& type);
        void setColor(const Color& c);
        void setYear(const int year);
        void setImagePath(const std::string& img_path);
        std::string getId() const;
        std::string getType() const;
        Color getColor() const;
        int getYear() const;
        std::string getImagePath() const;

        bool operator == (const Car& c) const;
        bool operator != (const Car& c) const;
        bool operator < (const Car& c) const;
        bool operator > (const Car& c) const;
        bool operator <= (const Car& c) const;
        bool operator >= (const Car& c) const;
        Car& operator = (const Car& c);
        const static Car NULL_CAR;
};

class CarPool : public BasicPool {
    private:
        std::map<std::string, Car> carpool_byid;
        std::multimap<Color, Car> carpool_bycolor;
        std::multimap<std::string, Car> carpool_bytype;

    public:
        CarPool();
        CarPool(Car* begin, Car* end);
        CarPool(const std::vector<Car>& cars);
        CarPool(const CarPool& cp);
        ~CarPool();
        int addCar(const Car& car);
        int removeCar(const Car& car);
        int removeCar(const std::string& id);
        int updateCar(const Car& original_car, const Car& new_car);
        CarPool getCarbyId(const std::string& id) const;
        CarPool getCarbyColor(const Color& color) const;
        CarPool getCarbyType(const std::string& type) const;
        CarPool getCar(const std::string& id = "", const Color& color = Color::NULL_COLOR, const std::string& type = "") const;
        size_t size() const;
        bool empty() const ;
        int clear();
        int load(std::istream& is);
        int save(std::ostream& os) const;
        std::vector<Car> list() const;

        operator std::vector<Car>() const;
        bool operator == (const CarPool& cp) const;
        bool operator != (const CarPool& cp) const;
        CarPool& operator = (const CarPool& cp);
};
