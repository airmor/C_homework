#include "define_the_const.h"
#include "war.h"
#include "mainwindow.h"

#ifndef GACHA_H
#define GACHA_H
namespace cha {
    extern int cha[3];
    extern int coin;
    class gacha;
}
class cha::gacha
{
public:

    void cha_begin();//开始战斗
    int cha_buy(int num);//买,从左到右，从0开始计，第num个  //成功返回1，失败（钱不够等）返回0
    int cha_sell(int num);//卖队伍里从0开始第num个  //成功返回1，失败（钱不够等）返回0
    int cha_up();//升本 //成功返回1，失败（钱不够等）返回0
    int re();//重抽 //成功返回1，失败（钱不够等）返回0
};

#endif // GACHA_H
