#ifndef __DRIVER_H__
#define __DRIVER_H__

#include"types.h"

class Driver
{
public:
    Driver();
    ~Driver();

    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();
};


class DriverManager
{
public:
    DriverManager();
    ~DriverManager();

    void Activate();
    void AddDriver(Driver *driver);

private:
    Driver *drivers[256];
    int numDrivers;
};

#endif