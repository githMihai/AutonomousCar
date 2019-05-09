#ifndef OBSERVER_H
#define OBSERVER_H

#include <iostream>
#include <set>

class Subject;
class IObserver
{
public:
    IObserver()     {};
    ~IObserver()    {};
    virtual void update(Subject* subject) = 0;
};

class Subject
{
public:
    Subject()    {};
    ~Subject()   {};
    virtual void attachObserver(IObserver *o)    { observers.insert(o); }
    virtual void detachObserver(IObserver *o)    { observers.erase(o); }
    virtual void notifyObservers()
    {
        for (auto &o : observers)
        {
            o->update(this);
        }
    }
private:
    std::set<IObserver*> observers;

};

#endif // OBSERVER_H