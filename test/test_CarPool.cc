/**
 * @file test/test_CarPool.cc
 * @brief Test for CarPool class in carinfo-manager project, including constructors, addCar, removeCar, updateCar, getCarbyId, getCarbyColor, getCarbyType, save, load, operator==, operator!=, operator=, using Google Test framework.
 * @author donghy23@mails.tsinghua.edu.cn
 * @version 1.0
 */

#include "gtest/gtest.h"
#include "carinfo-manager/carpool.hpp"
#include <fstream>
#include <vector>

TEST(Test_CarPool, test_CarPool_default_constructor) {
    CarPool carpool;
    EXPECT_EQ(carpool.size(), 0);
    EXPECT_TRUE(carpool.empty());
}

TEST(Test_CarPool, test_CarPool_parameterized_constructor) {
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    Car cars[] = {car1, car2, car3};
    CarPool carpool(cars, cars + 3);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
}

TEST(Test_CarPool, test_CarPool_parameterized_constructor_vector) {
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    std::vector<Car> cars = {car1, car2, car3};
    CarPool carpool(cars);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
}

TEST(Test_CarPool, test_CarPool_copy_constructor) {
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    Car cars[] = {car1, car2, car3};
    CarPool carpool1(cars, cars + 3);
    CarPool carpool2(carpool1);
    EXPECT_EQ(carpool2.size(), 3);
    EXPECT_FALSE(carpool2.empty());
}

TEST(Test_CarPool, test_CarPool_addCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_EQ(carpool.addCar(car1), 0x70);
    EXPECT_EQ(carpool.addCar(car2), 0x70);
    EXPECT_EQ(carpool.addCar(car3), 0x70);
}

TEST(Test_CarPool, test_CarPool_removeCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
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
    EXPECT_EQ(carpool.removeCar(car1), 0x80);
    EXPECT_EQ(carpool.removeCar(car2), 0x80);
    EXPECT_EQ(carpool.removeCar(car3), 0x80);
}

TEST(Test_CarPool, test_CarPool_updateCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());

    Car car4("京D13579", "SUV", "Dick", "green", 2021, "img4.jpg");
    Car car5("京E24680", "Sedan", "Eva", "orange", 2020, "img5.jpg");
    Car car6("京F35791", "Hatchback", "Frank", "purple", 2019, "img6.jpg");
    EXPECT_EQ(carpool.updateCar(car1, car4), 0);
    EXPECT_EQ(carpool.updateCar(car2, car5), 0);
    EXPECT_EQ(carpool.updateCar(car3, car6), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_EQ(carpool.updateCar(car1, car4), 0x90);
    EXPECT_EQ(carpool.updateCar(car2, car5), 0x90);
    EXPECT_EQ(carpool.updateCar(car3, car6), 0x90);

    EXPECT_EQ(carpool.updateCar("京D13579", car1), 0);
    EXPECT_EQ(carpool.updateCar("京E24680", car2), 0);
    EXPECT_EQ(carpool.updateCar("京F35791", car3), 0);
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    EXPECT_EQ(carpool.updateCar("京D13579", car1), 0x90);
    EXPECT_EQ(carpool.updateCar("京E24680", car2), 0x90);
    EXPECT_EQ(carpool.updateCar("京F35791", car3), 0x90);
}

TEST(Test_CarPool, test_CarPool_getCarbyId) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    EXPECT_TRUE(carpool.getCarbyId("京A12345") == CarPool((Car*)&car1, (Car*)&car1 + 1));
    EXPECT_TRUE(carpool.getCarbyId("京B67890") == CarPool((Car*)&car2, (Car*)&car2 + 1));
    EXPECT_TRUE(carpool.getCarbyId("京C24680") == CarPool((Car*)&car3, (Car*)&car3 + 1));
}

TEST(Test_CarPool, test_CarPool_getCarbyColor) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    CarPool redCars = carpool.getCarbyColor("red"), redCarsExpected((Car*)&car1, (Car*)&car1 + 1);
    CarPool greenCars = carpool.getCarbyColor("yellow"), greenCarsExpected((Car*)&car2, (Car*)&car2 + 1);
    CarPool blueCars = carpool.getCarbyColor("blue"), blueCarsExpected((Car*)&car3, (Car*)&car3 + 1);
    EXPECT_TRUE(redCars == redCarsExpected);
    EXPECT_TRUE(greenCars == greenCarsExpected);
    EXPECT_TRUE(blueCars == blueCarsExpected);
}

TEST(Test_CarPool, test_CarPool_getCarbyType) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
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

TEST(Test_CarPool, test_CarPool_getCarbyOwner) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    CarPool aliceCars = carpool.getCarbyOwner("Alice"), aliceCarsExpected((Car*)&car1, (Car*)&car1 + 1);
    CarPool bobCars = carpool.getCarbyOwner("Bob"), bobCarsExpected((Car*)&car2, (Car*)&car2 + 1);
    CarPool catherineCars = carpool.getCarbyOwner("Catherine"), catherineCarsExpected((Car*)&car3, (Car*)&car3 + 1);
    EXPECT_TRUE(aliceCars == aliceCarsExpected);
    EXPECT_TRUE(bobCars == bobCarsExpected);
    EXPECT_TRUE(catherineCars == catherineCarsExpected);
}

TEST(Test_CarPool, test_CarPool_getCar) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    CarPool redSUVs = carpool.getCar("", "red", "", "SUV"), redSUVsExpected((Car*)&car1, (Car*)&car1 + 1);
    CarPool yellowSUVs = carpool.getCar("", "yellow", "", "SUV"), yellowSUVsExpected;
    CarPool blueSUVs = carpool.getCar("", "blue", "", "SUV"), blueSUVsExpected;
    EXPECT_TRUE(redSUVs == redSUVsExpected);
    EXPECT_TRUE(yellowSUVs == yellowSUVsExpected);
    EXPECT_TRUE(blueSUVs == blueSUVsExpected);
}

TEST(Test_CarPool, test_CarPool_save) {
    CarPool carpool;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    
    EXPECT_EQ(carpool.addCar(car1), 0);
    EXPECT_EQ(carpool.addCar(car2), 0);
    EXPECT_EQ(carpool.addCar(car3), 0);
    std::ofstream ofs("test_CarPool_save.json");
    std::stringstream ss;
    ASSERT_EQ(carpool.save(ofs), 0);
}

TEST(Test_CarPool, test_CarPool_load) {
    CarPool carpool;
    std::ifstream ifs("test_CarPool_save.json");
    ASSERT_EQ(carpool.load(ifs), 0);
    ifs.close();
    EXPECT_EQ(carpool.size(), 3);
    EXPECT_FALSE(carpool.empty());
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    Car car3("京C24680", "Hatchback", "Catherine", "blue", 2019, "img3.jpg");
    EXPECT_TRUE(carpool.getCarbyId("京A12345") == CarPool((Car*)&car1, (Car*)&car1 + 1));
    EXPECT_TRUE(carpool.getCarbyId("京B67890") == CarPool((Car*)&car2, (Car*)&car2 + 1));
    EXPECT_TRUE(carpool.getCarbyId("京C24680") == CarPool((Car*)&car3, (Car*)&car3 + 1));
}

TEST(Test_CarPool, test_CarPool_operator_eq) {
    CarPool carpool1, carpool2;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    carpool1.addCar(car1); carpool1.addCar(car2);
    EXPECT_TRUE(carpool1 == carpool1);
    EXPECT_FALSE(carpool1 == carpool2);
}

TEST(Test_CarPool, test_CarPool_operator_ne) {
    CarPool carpool1, carpool2;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    carpool1.addCar(car1); carpool1.addCar(car2);
    EXPECT_FALSE(carpool1 != carpool1);
    EXPECT_TRUE(carpool1 != carpool2);
}

TEST(Test_CarPool, test_CarPool_operator_assign) {
    CarPool carpool1, carpool2;
    Car car1("京A12345", "SUV", "Alice", "red", 2021, "img1.jpg");
    Car car2("京B67890", "Sedan", "Bob", "yellow", 2020, "img2.jpg");
    carpool1.addCar(car1); carpool1.addCar(car2);
    carpool2 = carpool1;
    EXPECT_TRUE(carpool1 == carpool2);
}
