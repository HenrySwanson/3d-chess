#ifndef CHESS_VIEWINTERFACE_H
#define CHESS_VIEWINTERFACE_H

class ViewInterface
{
  public:
    virtual ~ViewInterface() {};
    virtual void refresh() = 0;
};

#endif
