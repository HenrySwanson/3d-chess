#ifndef CHESS_SUBJECT_H
#define CHESS_SUBJECT_H

#include <list>

#include "observer.h"

/** An interface for implementing the Observer pattern. */
class Subject
{
  public:
    /** Virtual destructor. */
    virtual ~Subject() {};

    inline void addObserver(Observer* obs)
    {
        observers_.push_back(obs);
    }

    inline void removeObserver(Observer* obs)
    {
        observers_.remove(obs);
    }

  protected:
    inline void notifyAll()
    {
        std::list<Observer*>::const_iterator it;
        for(it = observers_.begin(); it != observers_.end(); it++)
            (*it)->onNotify();
    }

  private:
    std::list<Observer*> observers_;
};

#endif
