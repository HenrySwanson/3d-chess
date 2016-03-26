3d-chess
========

Description
-----------

A C++ implementation of 3D chess. Not the Star Trek kind; not the standard-chess-with-pretty-graphics kind; this is a full 8 x 8 x 8 chessboard (chesscube?), with a rotatable 3D view. With 13 types of pieces (including the traditional 6), a whopping 64 pawns per side, and an additional dimension to navigate, this will be... interesting?

It's not complete yet. For example, my "AI" just picks the first move it can find, and I still need to make actual models for each piece. Right now, they're recognizable as the right pieces, but I just don't think they look good.

Installation
------------

This project uses OpenGL and wxWidgets. I'm not entirely sure how to correctly distribute these dependencies. So until I figure that out, you're on your own.

Screenshots
-----------
![Initial Configuration (Side)](http://imgur.com/unRzH2W.png)
![Initial Configuration (Top)](http://imgur.com/4EbdJFU.png)
![All Pieces](http://imgur.com/KTKL6Kv.png)

Contributions
-------------

The piece names and initial board setup come from hixoxih.com. Unfortunately, at the time of writing, this site is no longer accessible.
