#include "gtest/gtest.h"
#include "CarPool.hpp"

TEST(Test_CarPool, test_CarPool_default_constructor) {
    CarPool carpool;
    EXPECT_EQ(carpool.size(), 0);
    EXPECT_TRUE(carpool.empty());
}

TEST(Test_CarPool, test_CarPool_parameterized_constructor) {
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    Car cars[] = {car1, car2, car3};
    CarPool carpool(cars, cars + 3);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
}

TEST(Test_CarPool, test_CarPool_parameterized_constructor_vector) {
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    std::vector<Car> cars = {car1, car2, car3};
    CarPool carpool(cars);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
}

TEST(Test_CarPool, test_CarPool_copy_constructor) {
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    Car cars[] = {car1, car2, car3};
    CarPool carpool1(cars, cars + 3);
    CarPool carpool2(carpool1);
    EXPECT_EQ(carpool2.size(), 3);
    EXPECT_FALSE(carpool2.empty());
}

TEST(Test_CarPool, test_CarPool_addCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_EQ(carpool.addCar(car1), 1);
    EXPECT_EQ(carpool.addCar(car2), 1);
    EXPECT_EQ(carpool.addCar(car3), 1);
}

TEST(Test_CarPool, test_CarPool_removeCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_EQ(carpool.removeCar(car1), 0);
    EXPECT_EQ(carpool.removeCar(car2), 0);
    EXPECT_EQ(carpool.removeCar(car3), 0);
    EXPECT_EQ(carpool.size(), 0);
    EXPECT_TRUE(carpool.empty());
    EXPECT_EQ(carpool.removeCar(car1), 1);
    EXPECT_EQ(carpool.removeCar(car2), 1);
    EXPECT_EQ(carpool.removeCar(car3), 1);
}

TEST(Test_CarPool, test_CarPool_updateCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    Car car4("京D13579", "SUV", Color(255, 0, 0));
    Car car5("京E24680", "Sedan", Color(0, 255, 0));
    Car car6("京F35791", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.updateCar(car1, car4), 0);
    EXPECT_EQ(carpool.updateCar(car2, car5), 0);
    EXPECT_EQ(carpool.updateCar(car3, car6), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_EQ(carpool.updateCar(car1, car4), 1);
    EXPECT_EQ(carpool.updateCar(car2, car5), 1);
    EXPECT_EQ(carpool.updateCar(car3, car6), 1);
}

TEST(Test_CarPool, test_CarPool_getCarbyId) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_TRUE(carpool.getCarbyId("京A12345") == car1);
    EXPECT_TRUE(carpool.getCarbyId("京B67890") == car2);
    EXPECT_TRUE(carpool.getCarbyId("京C24680") == car3);
}

TEST(Test_CarPool, test_CarPool_getCarbyColor) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    CarPool redCars = carpool.getCarbyColor(Color(255, 0, 0)), redCarsExpected((Car*)&car1, (Car*)&car1 + 1);
    CarPool greenCars = carpool.getCarbyColor(Color(0, 255, 0)), greenCarsExpected((Car*)&car2, (Car*)&car2 + 1);
    CarPool blueCars = carpool.getCarbyColor(Color(0, 0, 255)), blueCarsExpected((Car*)&car3, (Car*)&car3 + 1);
    EXPECT_TRUE(redCars == redCarsExpected);
    EXPECT_TRUE(greenCars == greenCarsExpected);
    EXPECT_TRUE(blueCars == blueCarsExpected);
}

TEST(Test_CarPool, test_CarPool_getCarbyType) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    CarPool suvs = carpool.getCarbyType("SUV"), suvsExpected((Car*)&car1, (Car*)&car1 + 1);
    CarPool sedans = carpool.getCarbyType("Sedan"), sedansExpected((Car*)&car2, (Car*)&car2 + 1);
    CarPool hatchbacks = carpool.getCarbyType("Hatchback"), hatchbacksExpected((Car*)&car3, (Car*)&car3 + 1);
    EXPECT_TRUE(suvs == suvsExpected);
    EXPECT_TRUE(sedans == sedansExpected);
    EXPECT_TRUE(hatchbacks == hatchbacksExpected);
}

TEST(Test_CarPool, test_CarPool_save) {
    CarPool carpool;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    Car car3("京C24680", "Hatchback", Color(0, 0, 255));
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    ASSERT_EQ(carpool.save("test_CarPool_save.dat"), 0);
}

TEST(Test_CarPool, test_CarPool_load) {
    CarPool carpool;
    ASSERT_EQ(carpool.load("test_CarPool_save.dat"), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_TRUE(carpool.getCarbyId("京A12345") == Car("京A12345", "SUV", Color(255, 0, 0)));
    EXPECT_TRUE(carpool.getCarbyId("京B67890") == Car("京B67890", "Sedan", Color(0, 255, 0)));
    EXPECT_TRUE(carpool.getCarbyId("京C24680") == Car("京C24680", "Hatchback", Color(0, 0, 255)));
}

TEST(Test_CarPool, test_CarPool_operator_eq) {
    CarPool carpool1, carpool2;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    carpool1.addCar(car1); carpool1.addCar(car2);
    EXPECT_TRUE(carpool1 == carpool1);
    EXPECT_FALSE(carpool1 == carpool2);
}

TEST(Test_CarPool, test_CarPool_operator_ne) {
    CarPool carpool1, carpool2;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    carpool1.addCar(car1); carpool1.addCar(car2);
    EXPECT_FALSE(carpool1 != carpool1);
    EXPECT_TRUE(carpool1 != carpool2);
}

TEST(Test_CarPool, test_CarPool_operator_assign) {
    CarPool carpool1, carpool2;
    Car car1("京A12345", "SUV", Color(255, 0, 0));
    Car car2("京B67890", "Sedan", Color(0, 255, 0));
    carpool1.addCar(car1); carpool1.addCar(car2);
    carpool2 = carpool1;
    EXPECT_TRUE(carpool1 == carpool2);
}
