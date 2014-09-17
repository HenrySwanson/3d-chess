#ifndef CHESS_COMMON_H
#define CHESS_COMMON_H

struct Cell
{
    int x, y, z;
};

inline bool operator==(Cell a, Cell b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

/**
 * Converts from an 8 x 8 x 8 cube to a 1728 array. The x-coordinates vary
 * first, then y, then z.
 */
inline int mailbox(int x, int y, int z)
{
    return (z + 2) * 144 + (y + 2) * 12 + (x + 2);
}

/** Like mailbox(int, int, int), but accepts a Cell instead. */
inline int mailbox(Cell c)
{
    return mailbox(c.x, c.y, c.z);
}

/** Converts from a 1728 array to the x-coordinate of an 8 x 8 x 8 cube. */
inline int unmailboxX(int i)
{
    return (i % 12) - 2;
}

/** Converts from a 1728 array to the y-coordinate of an 8 x 8 x 8 cube. */
inline int unmailboxY(int i)
{
    return (i / 12) % 12 - 2;
}

/** Converts from a 1728 array to the z-coordinate of an 8 x 8 x 8 cube. */
inline int unmailboxZ(int i)
{
    return (i / 144) % 12 - 2;
}

/** Converts from a 1728 array to a Cell. */
inline Cell unmailbox(int i)
{
    Cell c;
    c.x = unmailboxX(i);
    c.y = unmailboxY(i);
    c.z = unmailboxZ(i);
    return c;
}

#endif
