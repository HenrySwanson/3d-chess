3d-chess
========

Description
-----------

A C++ implementation of 3D chess. Not the Star Trek kind; not the standard-chess-with-pretty-graphics kind; this is a full 8 x 8 x 8 chessboard (chesscube?), with a rotatable 3D view. With 13 types of pieces (including the traditional 6), a whopping 64 pawns per side, and an additional dimension to navigate, this will be... interesting?

It's not complete yet. For example, my "AI" just picks the first move it can find, and I still need to make actual models for each piece. Right now, it's just a bunch of colored pyramids. I've asked a friend of mine who actually knows how to use Blender to help out, so that'll happen eventually.

This should be thread-safe; I've limited the places where threads interact to the Game and HumanPlayer classes, and the DisplayCanvas::refresh() method. Still though, threads are new to me, so let me know if I've done anything stupid.

Installation
------------

This project uses OpenGL and wxWidgets. I'm not entirely sure how to correctly distribute these dependencies. So until I figure that out, you're on your own.

Screenshots
-----------
![Initial Configuration (Side)](http://imgur.com/HaxuP9x)
![Initial Configuration (Top)](http://imgur.com/sAXw5w9)
![Move Indicators](http://imgur.com/nu4a5so)

Contributions
-------------

The piece names and initial board setup come from hixoxih.com. Unfortunately, at the time of writing, this site is no longer accessible.
