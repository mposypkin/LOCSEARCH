/* 
 * File:   tesgfsdesc.cpp
 * Author: medved
 *
 * Created on February 22, 2016, 3:17 PM
 */

#include <iostream>
#include <oneobj/contboxconstr/dejong.hpp>
#include <funccnt.hpp>
#include "gfsdesc.hpp"

class MyStopper : public LOCSEARCH::GFSDesc<double>::Stopper {
public:

    bool stopnow(double xdiff, double fdiff, double gmin, double fval, int n) {
        cnt++;
        if (fval < 1e-3)
            return true;
        else
            return false;
    }

    int cnt = 0;
};

/*
 * 
 */
int main(int argc, char** argv) {
    const int n = 10;
    OPTITEST::DejongProblemFactory fact(n, -4, 8);
    COMPI::MPProblem<double> *mpp = fact.getProblem();
    COMPI::FuncCnt<double> *obj = new COMPI::FuncCnt<double>(*mpp->mObjectives.at(0));
    mpp->mObjectives.pop_back();    
    mpp->mObjectives.push_back(obj);
    
    MyStopper stp;
    LOCSEARCH::GFSDesc<double> desc(*mpp, stp);

    //desc.getOptions().mOnlyCoordinateDescent = true;

    double x[n];

    for (int i = 0; i < n; i++)
        x[i] = i;
    double v;
    bool rv = desc.search(x, v);
    std::cout << "In " << stp.cnt << " iterations found v = " << v << " at " << snowgoose::VecUtils::vecPrint(n, x) << "\n";
    std::cout << "Number of objective calls is " << obj->mCounters.mFuncCalls << "\n";

    return 0;
}

