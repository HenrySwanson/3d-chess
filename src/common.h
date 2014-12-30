#ifndef CHESS_COMMON_H
#define CHESS_COMMON_H

/**
 * Converts from an 8 x 8 x 8 cube to a 1728 array. The x-coordinates vary
 * first, then y, then z.
 */
inline int mailbox(int x, int y, int z)
{
    return (z + 2) * 144 + (y + 2) * 12 + (x + 2);
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

#endif
