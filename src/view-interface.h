#ifndef CHESS_VIEWINTERFACE_H
#define CHESS_VIEWINTERFACE_H

/**
 * Provides an interface for views; that is, things that display a chess game.
 */
class ViewInterface
{
  public:
    /** Virtual destructor. */
    virtual ~ViewInterface() {};

    /**
     * Asks the view to update itself. Can be synchronous or asynchronous.
     */
    virtual void refresh() = 0;
};

#endif
