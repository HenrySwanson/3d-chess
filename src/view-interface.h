#ifndef CHESS_VIEWINTERFACE_H
#define CHESS_VIEWINTERFACE_H

class ViewInterface
{
  public:
    /** Virtual destructor. */
    virtual ~ViewInterface() {};

    virtual void refresh() = 0;
};

#endif
