#include "gtest/gtest.h"
#include "CarPool.h"

TEST(Test_CarPool, test_CarPool_getCarbyId) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    carpool.addCar(car1);
    carpool.addCar(car2);
    carpool.addCar(car3);
    EXPECT_TRUE(carpool.getCarbyId("京A12345") == car1);
    EXPECT_TRUE(carpool.getCarbyId("京B67890") == car2);
    EXPECT_TRUE(carpool.getCarbyId("京C24680") == car3);
}

TEST(Test_CarPool, test_CarPool_getCarbyColor) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    carpool.addCar(car1);
    carpool.addCar(car2);
    carpool.addCar(car3);
    std::vector<Car> redCars = carpool.getCarbyColor(Color(255, 0, 0)), redCarsExpected = std::vector<Car>{car1};
    std::vector<Car> greenCars = carpool.getCarbyColor(Color(0, 255, 0)), greenCarsExpected = std::vector<Car>{car2};
    std::vector<Car> blueCars = carpool.getCarbyColor(Color(0, 0, 255)), blueCarsExpected = std::vector<Car>{car3};
    EXPECT_TRUE(redCars == redCarsExpected);
    EXPECT_TRUE(greenCars == greenCarsExpected);
    EXPECT_TRUE(blueCars == blueCarsExpected);
}

TEST(Test_CarPool, test_CarPool_getCarbyType) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    carpool.addCar(car1);
    carpool.addCar(car2);
    carpool.addCar(car3);
    std::vector<Car> suvs = carpool.getCarbyType("SUV"), suvsExpected = std::vector<Car>{car1};
    std::vector<Car> sedans = carpool.getCarbyType("Sedan"), sedansExpected = std::vector<Car>{car2};
    std::vector<Car> hatchbacks = carpool.getCarbyType("Hatchback"), hatchbacksExpected = std::vector<Car>{car3};
    EXPECT_TRUE(suvs == suvsExpected);
    EXPECT_TRUE(sedans == sedansExpected);
    EXPECT_TRUE(hatchbacks == hatchbacksExpected);
}
