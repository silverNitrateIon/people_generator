/* 
 * People Generator
 * Copyright © 2019 BackPack Technologies
 * All rights reserved.
 * Author: Russell Gill
 * Email: russell@silvernitrateion.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 *
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <iostream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <string>

class DataSynthesizer{
    
    std::default_random_engine gen{std::random_device{}()};

    private:
        int from_dist(int mean, int stDev, int max, int min){
            std::normal_distribution<double> dist(mean, stDev), max(max), min(min);
            int result = (int)dist(gen);
            if (result < 0){
                return (result * -1);
            }
            return result;
        }

    public:

        double MarketParticipation(long int income){
            return from_dist(100, 100, 0, 100);
        }

        int NumberOfChildern(){
            return from_dist(2, 2, 0, 10);
        }

        int DebtGenerator(int income){
            int maxVal, minVal;
            int mean = (int)(income * 1.7);
            int stDev = 100000;
            std::normal_distribution<double> dist(mean, stDev);
            int debt = (int)dist(gen);
            if (debt < 0){
                debt = rand() % 1000;
            }
            return debt;
        }

        int IncomeGenerator(std::string gender, int age){
            int mean;
            int stDev = 100000;
            if (gender == "F"){
                mean = 32100;
            } else if (gender == "M"){
                mean = 48100;
            }
            std::normal_distribution<double> dist(mean, stDev), min(1000), max(300000);
            int income = (int)dist(gen);
            if (income < 0){
                income = income * -1;
            }
            return income;
        }

        int AgeGenerator(std::string gender){
            int mean;
            double stDev = 10;
            if (gender == "F"){
                mean = 31;
            } else if (gender == "M"){
                mean = 39;
            }
            std::normal_distribution<double> dist(mean, stDev), min(18), max(85);
            int age = (int)dist(gen);
            if (age < 18){
                age = 18;
            } else if ((age < 0) && (age < -18)){
                age = age * -1;
            } else if ((age > -18) && (age < 0)){
                age = 18;
            }
            return age;
        }

        std::string EmploymentGenerator(int roll, std::string gender, int income){
            if (income < 3000){
                if ((gender == "M") && (roll >= 6.1)){
                    return "Y";
                } else if ((gender == "F") && (roll >= 5.3)){
                    return "Y";
                } else {
                    return "N";
                }
            } else{
                return "Y";
            }
        }

        std::string GenderGeneration(int roll){
            if (roll <= 51){
                return "M";
            } else if (roll > 49){
                return "F";
            }
        }
};

class DataStream{

    public:

    std::fstream filestream;
    std::string delim = ",";

        void Open(std::string filename){
            filestream.open(filename, std::fstream::out | std::fstream::app);
        }

        void Start(){
            filestream << "Gender" << delim << "Age" << delim << "Employed" << delim << "Income" << delim << "Total Debt" << "\n";
        }

        void Write(int age, int income, std::string employment, std::string gender, int debt){
            if (age > 18){
                filestream << gender << delim << age << delim << employment << delim << income << delim << debt << "\n";
            }
        }

        void Close(){
            filestream.close();
        }
};

int main(int argc, char *argv[]){
    srand(std::random_device{}());
    DataStream file;
    DataSynthesizer data;
    int total;
    int roll;
    std::cout << "Enter total people" << std::endl;
    std::cin >> total;
    file.Open("test.csv");
    file.Start();
    for (int i = 0; i < total; i++){
        roll = rand() % 100;
        std::string gender = data.GenderGeneration(roll);
        int age = data.AgeGenerator(gender);
        int income = data.IncomeGenerator("F", age);
        roll = rand() % 100;
        std::string emp = data.EmploymentGenerator(roll, gender, income);
        int debt = data.DebtGenerator(income);
        std::cout << gender << " " << age << " " << income << " " << emp << " " << debt<< std::endl;
        file.Write(age, income, emp, gender, debt);
    }
    file.Close();
    return 0;
}
