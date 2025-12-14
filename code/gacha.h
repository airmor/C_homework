#include "define_the_const.h"
#include "war.h"
#include "mainwindow.h"

#ifndef GACHA_H
#define GACHA_H
namespace cha {
    extern int cha[];
    class gacha;
}
class cha::gacha
{
public:
    void cha_begin();
    void cha_buy();
    void cha_up();
    void cha_l();
    void cha_m();
    void cha_r();
};

#endif // GACHA_H
