#ifndef CHESS_OBSERVER_H
#define CHESS_OBSERVER_H

/** An interface for implementing the Observer pattern. */
class Observer
{
  public:
    /** Virtual destructor. */
    virtual ~Observer() {};

    /** Tells the observer something has happened. */
    virtual void onNotify() = 0;
};

#endif
