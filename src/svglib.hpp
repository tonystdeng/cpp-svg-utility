#pragma once

#ifndef SVGLIB_HPP
#define SVGLIB_HPP

#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cctype>
#include <array>
#include <cmath>
#include <string>
#include <algorithm>

#include <tinyxml2.h>
using namespace tinyxml2;

const double PI=3.141592653589793238462643383279502884197;
const double toRadians=PI/180.,toDegree=180./PI;

// #include "svgtran.hpp"
// failed :<

namespace sl{

    typedef std::array<std::vector<std::string>,2> filepath;//(list of path,list transform)
    typedef std::vector<filepath> fileinfo;// typedef std::array<std::vector<std::string>,2> filepath

    typedef std::vector<std::pair<std::string,std::vector<double>>> transforminfo;
    typedef std::vector<std::pair<char,std::vector<double>>> pathinfo;
    typedef std::vector<std::pair<pathinfo,transforminfo>> svginfo;

    typedef std::array<double,2> point;
    typedef std::vector<point> pointgraph;

    fileinfo fileLoader(char *);
    void transformExtract(XMLElement *,std::string&);
    void pathSearch(XMLElement *,fileinfo&,std::string="");
    void printFile(fileinfo);

    svginfo file2vector(fileinfo);
    void path2vector(pathinfo&, std::string);
    void trans2vector(transforminfo&, std::vector<std::string>);

    std::vector<pointgraph> getPointGraph(std::pair<pathinfo,transforminfo>,point*);
    pointgraph bcgpg(pointgraph);// stands for bezier curve get point graph
    //pointgraph applyTran(pointgraph,transforminfo);// last try, let see it; update: failed
    point dca(pointgraph,float);// stands for de casteljau algorithm, basicly recursive lerp, aka how bezier curve works
    pointgraph eagpg(std::vector<double>,point,bool,point* travel=nullptr);// stands for Elliptical Arc get point graph
    pointgraph simplify(pointgraph,double=1.5,double=12,double=120);// simplify pointgraph

    // the following function are moved from view.cpp, thought can use them more commonly
    std::vector<pointgraph> svg2points(svginfo);// using getPointGraph, loop through svginfo
    std::array<double, 7> getrepoinfo(std::vector<pointgraph>,double=0);// get reposition info, 
    // ^{xmax,xmin,ymax,ymin,x/ylargest,resoRate}
    std::vector<pointgraph> resizeRepo(std::vector<pointgraph>,double=0,double=0,double=1);
    // input list: original, xmin,ymin,resRate; fuc name: resize + reposition


    void printInfo(svginfo);// print all svg info in console
    // void printPtrGraph(pointgraph);// print all pointgraph info in console
    pointgraph array2Ptr(std::vector<double>);
    point lerp2d(point,point,float);// for whatever reason a lerp function isn't included in cmath

    struct ellipticalArc
    {
        point start;
        point radius;
        point end;
        double angle;
        bool largeArc;
        bool clockWise;

        void normal(std::vector<double> info,point startPtr){
            start=startPtr;
            radius[0]=info[0];radius[1]=info[1];
            angle=info[2];largeArc=info[3];clockWise=info[4];
            end[0]=info[5];end[1]=info[6];
        }
    };
    
}
#endif