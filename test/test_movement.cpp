#include "../src/board.h"

#include <iostream>

using std::cout;
using std::endl;

int pack(int x, int y, int z)
{
    return Board::mailbox(64 * z + 8 * y + x);
}

int print_unpacked(int m)
{
    int i = Board::unmailbox(m);
    cout << (i % 8) << ", " << ((i / 8) % 8) << ", " << ((i / 64) % 8) << endl;
}

int main(void)
{

    cout << "----Begin test----" << endl;

    Board b;
    Square s;
    s.type = KNIGHT;
    s.color = true;

    int loc = pack(2,3,4);
    b.squares[loc] = s;
    std::list<Move> li = b.generateMoves(loc);

    for(std::list<Move>::iterator it = li.begin(); it != li.end(); it++)
    {
        print_unpacked(it->to);
    }

    cout << "----End test----" << endl;

    return 0;
}
