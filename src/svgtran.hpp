#pragma once

#ifndef SVGTRAN_HPP
#define SVGTRAN_HPP

#include <vector>
#include <array>
#include <map>
#include <string>
#include <cmath>
#include <stdexcept>
#include <iostream>


namespace st { // st stands for svg transformation
    typedef std::array<double,2> point;

    point translate(std::vector<double>, point);
    point scale(std::vector<double>, point);
    point rotate(std::vector<double>, point);
    point skewX(std::vector<double>, point);
    point skewY(std::vector<double>, point);
    point matrix(std::vector<double>, point);

    point apply(std::string,std::vector<double>, point);
}
#endif 
