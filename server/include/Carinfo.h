#pragma once
#pragma execution_character_set("utf-8")
#include <iostream>
#include <string>

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