/**
 * @file include/carinfo-manager/carpool.hpp
 * @brief Declaration of class Color, class Car, and class CarPool.
 * 
 * @details
 * This file contains the declarations of the Color, Car, and CarPool classes.
 * The Color class represents a color with red, green, and blue components.
 * The Car class represents a car with an ID, type, color, year, and image path.
 * The CarPool class represents a collection of cars and provides various operations on them.
 * 
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#pragma once
#pragma execution_character_set("utf-8")
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "carinfo-manager/basicpool.hpp"

class Car {
  private:
	std::string car_id;
	std::string car_type;
	std::string car_owner;
	std::string car_color;
	int car_year;
	std::string car_img_path;

  public:
	Car();
	Car(const std::string &id,
		const std::string &type,
		const std::string &owner,
		const std::string &color,
		const int year,
		const std::string &img_path);
	Car(const Car &c);
	~Car();
	void setId(const std::string &id);
	void setType(const std::string &type);
	void setOwner(const std::string &owner);
	void setColor(const std::string &color);
	void setYear(const int year);
	void setImagePath(const std::string &img_path);
	std::string getId() const;
	std::string getType() const;
	std::string getOwner() const;
	std::string getColor() const;
	int getYear() const;
	std::string getImagePath() const;

	bool operator==(const Car &c) const;
	bool operator!=(const Car &c) const;
	bool operator<(const Car &c) const;
	bool operator>(const Car &c) const;
	bool operator<=(const Car &c) const;
	bool operator>=(const Car &c) const;
	Car &operator=(const Car &c);
	const static Car NULL_CAR;
};

class CarPool : public BasicPool {
  private:
	std::map<std::string, Car> carpool_byid;
	std::multimap<std::string, Car> carpool_byowner;
	std::multimap<std::string, Car> carpool_bycolor;
	std::multimap<std::string, Car> carpool_bytype;

  public:
	CarPool();
	CarPool(Car *begin, Car *end);
	CarPool(const std::vector<Car> &cars);
	CarPool(const CarPool &cp);
	~CarPool();
	int addCar(const Car &car);
	int removeCar(const Car &car);
	int removeCar(const std::string &id);
	int updateCar(const Car &original_car, const Car &new_car);
	int updateCar(const std::string &id, const Car &new_car);
	CarPool getCarbyId(const std::string &id) const;
	CarPool getCarbyColor(const std::string &color) const;
	CarPool getCarbyOwner(const std::string &owner) const;
	CarPool getCarbyType(const std::string &type) const;
	CarPool getCar(const std::string &id = "",
				   const std::string &color = "",
				   const std::string &owner = "",
				   const std::string &type = "") const;
	size_t size() const;
	bool empty() const;
	int clear();
	int load(std::istream &is);
	int save(std::ostream &os) const;
	std::vector<Car> list() const;

	// operator std::vector<Car>() const;
	bool operator==(const CarPool &cp) const;
	bool operator!=(const CarPool &cp) const;
	CarPool &operator=(const CarPool &cp);
};
