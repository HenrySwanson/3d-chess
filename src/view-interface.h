#ifndef CHESS_VIEWINTERFACE_H
#define CHESS_VIEWINTERFACE_H

/**
 * An interface for the view layer in MVP. Required for the Presenter class to
 * update the viewer class.
 */
class ViewInterface
{
  public:
    /** Virtual destructor. */
    virtual ~ViewInterface() {};

    /** Tells the viewer to re-collect data from the Presenter. */
    virtual void refresh() = 0;
};

#endif
