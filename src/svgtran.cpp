#include "svgtran.hpp"


st::point st::translate(std::vector<double> info, point ptr) {
    return {ptr[0] + info[0], ptr[1] + info[1]};
}

st::point st::scale(std::vector<double> info, point ptr) {
    return {ptr[0] * info[0], ptr[1] * info[1]};
}

st::point st::rotate(std::vector<double> info, point ptr) {
    double angle=info.size() > 0 ? info[0] : 0; // Angle in degrees
    double cx=info.size() > 1 ? info[1] : 0;
    double cy=info.size() > 2 ? info[2] : 0;

    double radians=angle * M_PI / 180.0; // Convert degrees to radians
    double x=ptr[0] - cx;
    double y=ptr[1] - cy;

    double xNew=x * cos(radians) - y * sin(radians) + cx;
    double yNew=x * sin(radians) + y * cos(radians) + cy;
    return {xNew, yNew};
}

st::point st::skewX(std::vector<double> info, point ptr) {
    double angle=info[0];
    double radians=angle * M_PI / 180.0;
    return {ptr[0] + ptr[1] * tan(radians), ptr[1]};
}

st::point st::skewY(std::vector<double> info, point ptr) {
    double angle=info.size() > 0 ? info[0] : 0; // Angle in degrees
    double radians=angle * M_PI / 180.0;
    return {ptr[0], ptr[1] + ptr[0] * tan(radians)};
}

st::point st::matrix(std::vector<double> info, point ptr) {
    if (info.size() != 6) {
        throw std::invalid_argument("Matrix transformation requires 6 parameters.");
    }
    double xNew=ptr[0]*info[0]+ptr[1]*info[1]+info[2];
    double yNew=ptr[0]*info[3]+ptr[1]*info[4]+info[5];
    return {xNew, yNew};
}

st::point st::apply(std::string command, std::vector<double> info, point ptr) {
    point result = ptr;
    if (command.find("translate") != std::string::npos) {
        result = translate(info, ptr);
    } 
    else if (command.find("rotate") != std::string::npos) {
        result = rotate(info, ptr);
    } 
    else if (command.find("scale") != std::string::npos) {
        result = scale(info, ptr);
    } 
    else if (command.find("skewX") != std::string::npos) {
        result = skewX(info, ptr);
    } 
    else if (command.find("skewY") != std::string::npos) {
        result = skewY(info, ptr);
    } 
    else if (command.find("matrix") != std::string::npos) {
        result = matrix(info, ptr);
    } 
    else {
        throw std::invalid_argument("Unknown transformation command.");
    }
    return result;
}
