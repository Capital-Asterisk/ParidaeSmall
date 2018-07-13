#include <iostream>
#include <cmath>
#include "titsmall.h"

/*class GuiString
{
    char* m_data;
    uint32_t* m_indices;
    uint32_t m_size;
    uint32_t m_sizeApparent;

  public:
    GuiString(const char* cstring)
    {
        // Loop through the cstring, terminate at null
        m_size = 0;
        while (cstring[m_size] != 0) {
            m_sizeApparent ++;
            // Skip 2 characters when a unicode character is detected
            m_size += (cstring[m_size] == 226) ? 3 : 1;
        }
        // m_size is now the length of the string
        // m_sizeApparent is the length of the string when printed, unicodes as a single char
        // *3 data size so that all characters in the string can be unicode
        m_data = new char[m_sizeApparent * 3];
        m_indices = new uint32_t[m_sizeApparent * 3];
        uint32_t j = 0;
        for (uint32_t i = 0; i < m_sizeApparent; i)
        {
            m_data[i] = cstring[i];
            m_indices[i] = j;
            j ++;
        }
    }
};*/

class Powversi
{
    static constexpr const char* smc_boardTemplate =
            " ╺────────────────────────────────────────────────────────────────────────────╸"
            "\n    A    B    C    D    E    F    G    H     [██: ++ ] [░░: -- ] [$]"
            "\n  ┌────┬────┬────┬────┬────┬────┬────┬────┐  "
            "\n 1│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  Turns"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  ░░ ██ ░░ ██ ░░ ██ ░░ ██ ░░ ██"
            "\n 2│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  II II II II II II II II II II"
            "\n 3│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  II II II II II II II II II II"
            "\n 4│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  II II II II II II II II II II"
            "\n 5│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  II II II II II II II II II II"
            "\n 6│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  II II II II II II II II II II"
            "\n 7│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ├────┼────┼────┼────┼────┼────┼────┼────┤  II II II II II II II II II II"
            "\n 8│ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │ 00 │  II II II II II II II II II II"
            "\n  ┕━━━━┷━━━━┷━━━━┷━━━━┷━━━━┷━━━━┷━━━━┷━━━━┙   "
            "\n ╺────────────────────────────────────────────────────────────────────────────╸";
    std::vector<uint8_t> m_possibleMoves;
    std::vector<uint8_t> m_turns;
    std::vector<uint16_t> m_moveHistory;
    uint64_t m_board[2]; // [black] [white]
    uint8_t m_quadCounts[8]; // [quad 1-4 black], [quavd 1-4 white]; cartesian order
    uint8_t m_turn;

    // For break computer
    std::vector<uint64_t[2]> m_boardStack;


public:
    static inline int xy(uint8_t x, uint8_t y) {
        if (x < 0 || x >= 8 || y < 0 || y >= 8) {
            return -1;
        }
        return (y * 8) + x;
    }
    static inline int quadrant(uint8_t x, uint8_t y) {
        return 2 * (y > 3) + ((x / 4) ^ (y < 4));
    }

    Powversi()
    {
        m_board[0] = (1 << 28) | (uint64_t(1) << 35);
        m_board[1] = (1 << 27) | (uint64_t(1) << 36);
        m_quadCounts[0] = 1;
        m_quadCounts[1] = 0;
        m_quadCounts[2] = 1;
        m_quadCounts[3] = 0;
        m_quadCounts[4] = 0;
        m_quadCounts[5] = 1;
        m_quadCounts[6] = 0;
        m_quadCounts[7] = 1;
        //m_board[0] = (uint64_t(1) << xy(3, 4)) | (uint64_t(1) << xy(4, 4)) | (uint64_t(1) << xy(5, 5));
        //m_board[1] = (uint64_t(1) << xy(1, 3)) | (uint64_t(1) << xy(2, 3)) | (uint64_t(1) << xy(3, 3)) | (uint64_t(1) << xy(4, 3)) | (uint64_t(1) << xy(4, 2));
        //m_printMe.reserve(80*25*3);
        m_possibleMoves.reserve(60);
        m_turn = 0;
    }

    void print(bool printPossibles)
    {
        // loop through string until it's null;
        int i = 0;
        uint64_t tile = 1;
        uint8_t history = 0;
        char c;
        while ((c = smc_boardTemplate[i]) != 0) {
            switch (c)
            {
            case '0': // Print a peice
                // 00 is replaced by a game piece or spaces
                if ((m_board[0] & tile) != 0)
                {
                    printf("░░");
                }
                else if ((m_board[1] & tile) != 0)
                {
                    printf("██");
                }
                else
                {
                    if (printPossibles)
                    {
                        uint8_t tileIndex = log2(tile);
                        int moveIndex = -1;

                        // test if this spot is a possible move
                        for (int i = 0; i < m_possibleMoves.size(); i ++)
                        {
                            if (m_possibleMoves[i] == tileIndex)
                            {
                                moveIndex = i;
                            }
                        }
                        if (moveIndex == -1)
                        {
                            printf("  ");
                        } else {

                            //printf("%02i", moveIndex);
                            printf("%i", moveIndex + 1);
                            if (moveIndex < 9)
                            {
                                printf(" ");
                            }
                        }

                    }
                    else
                    {
                        printf("  ");
                    }
                }
                tile = tile << 1;
                i ++; // skip next char
                break;
            case '-': // Print # of blacks
                printf("%02u", m_quadCounts[0] + m_quadCounts[1] + m_quadCounts[2] + m_quadCounts[3]);
                i ++; // skip next char
                break;
            case '+': // Print # of whites
                printf("%02u", m_quadCounts[4] + m_quadCounts[5] + m_quadCounts[6] + m_quadCounts[7]);
                i ++; // skip next char
                break;
            case 'I':
                if (m_moveHistory.size() > history)
                {
                    printf("%c%c", char(m_moveHistory[history] << 8 >> 8), char(m_moveHistory[history] >> 8));
                } else {
                    printf("  ");
                }
                history ++;
                i ++; // skip next char
                break;
            default:
                putchar(c);
                break;
            }

            i ++;
        }
    }

    bool line_check(int x, int y, int dirX, int dirY, bool capture) {
        // index of a spot right next to (x, y)
        // Test if that new tile is of the other colour

        uint8_t turn = m_turn % 2;
        uint64_t tile;
        uint64_t captures = 0;
        int steps = 1;
        int ind = 0;
        bool run = true;
        while (run)
        {
            // Step along specified direction
            ind = xy(x + dirX * steps, y + dirY * steps);

            // False if edge of board reached
            if (ind == -1)
            {
                return false;
            }
            tile = (uint64_t(1) << ind);
            // Continue if the current tile is of opposite colour
            if (m_board[!turn] & tile)
            {
                captures |= tile;
                steps ++;
            } else {
                run = false;
            }
        }
        // false if nothing in between, or blank
        if (steps == 1)
        {
           return false;
        }
        // Check if the last tile is of
        if (m_board[turn] & tile)
        {
            if (capture) {
                for (int i = 1; i < steps; i ++) {
                    int quad = quadrant(x + dirX * i, y + dirY * i);
                    m_quadCounts[quad + 4 * turn] ++;
                    m_quadCounts[quad + 4 * !turn] --;
                }
                m_board[!turn] ^= captures; // Remove tiles of other colour
                m_board[turn] |= captures; // Add new tiles
            }
            return true;
        }
        return false;
    }

    void calc_possible_moves()
    {
        m_possibleMoves.clear();
        uint8_t turn = m_turn % 2; // 0 for black, 1 for white
        // Loop through all the tiles
        for (int y = 0; y < 8; y ++)
        {
            for (int x = 0; x < 8; x ++)
            {
                int ind = xy(x, y);
                uint64_t tile = (uint64_t(1) << ind);
                // If the current tile is blank
                if (!((m_board[0] | m_board[1]) & tile))
                {
                    if (line_check(x, y, 1, 1, false)
                        || line_check(x, y, 1, -1, false)
                        || line_check(x, y, -1, 1, false)
                        || line_check(x, y, -1, -1, false)
                        || line_check(x, y, 0, 1, false)
                        || line_check(x, y, 0, -1, false)
                        || line_check(x, y, 1, 0, false)
                        || line_check(x, y, -1, 0, false))
                    {
                        m_possibleMoves.push_back(ind);
                    }
                }

            }
        }
    }

    void put_piece(bool white, int x, int y)
    {
        // Add the new peice
        m_board[white] |= (uint64_t(1) << xy(x, y));
        m_quadCounts[quadrant(x, y) + 4 * white] ++;
        // Capture some other ones1
        line_check(x, y, 1, 1, true);
        line_check(x, y, 1, -1, true);
        line_check(x, y, -1, 1, true);
        line_check(x, y, -1, -1, true);
        line_check(x, y, 0, 1, true);
        line_check(x, y, 0, -1, true);
        line_check(x, y, 1, 0, true);
        line_check(x, y, -1, 0, true);
    }

    void two_player()
    {
        int i;

        while (true) {
            printf("POWVERSI\n");
            this->calc_possible_moves();
            this->print(true);
            if (m_possibleMoves.size() != 0) {
                //printf("QuadStats: %u %u %u %u\n", m_quadCounts[0], m_quadCounts[1], m_quadCounts[2], m_quadCounts[3]);
                printf("\n\n\n %s's Turn: ", (m_turn % 2) ? "[██] White" : "[░░] Black");
                scanf("%i", &i);
                int x = m_possibleMoves[i - 1] % 8;
                int y = m_possibleMoves[i - 1] / 8;
                m_moveHistory.push_back(uint8_t('A' + x) | ((uint8_t('1' + y) << 8)));
                put_piece((m_turn % 2), x, y);
            } else {
                m_moveHistory.push_back('-' | ('-' << 8));
            }
            m_turn ++;
        }
    }

    void break_computer()
    {

    }
};

int main()
{
    //std::wstring aaa = L"widebird \u20AC \u25A0  ░ ██ ░░ █";
    //wprintf(L"Press 0 for game, press 1 for global thermonuclear war. [%u, %S]\n", aaa.length(), aaa.c_str());
    //std::wcout << aaa << std::endl;
    //"\u25CC \u2B24 \u25EF\naaaaaaa"
    //uint8_t a[9] = {226, 150, 136,   226, 101, 0,   226, 101, 0};
    //estring f    = "aaa";
    //std::cout.write((char*)a, 10);
    //std::cout << sizeof("aaa") << "\naaa";

    Powversi p;
    p.two_player();



    return 0;
}


