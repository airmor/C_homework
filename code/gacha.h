#include "define_the_const.h"
#include "war.h"
#include "mainwindow.h"

#ifndef GACHA_H
#define GACHA_H
namespace cha
{
    extern int cha[3];
    extern int coin;
    extern int shop_level;
    struct CardInfo
    {
        int name_number; // 对应 all_role_base 的职业编号
        int level;       // 该卡所属商店等级（1-5）
    };
    class gacha;
}
class cha::gacha
{
public:
    static void cha_begin();      // 开始战斗
    static int cha_buy(int num);  // 买,从左到右，从0开始计，第num个  //成功返回1，失败（钱不够等）返回0
    static int cha_sell(int num); // 卖队伍里从0开始第num个  //成功返回1，失败（钱不够等）返回0
    static int cha_up();          // 升本 //成功返回1，失败（钱不够等）返回0
    static int re();              // 重抽 //成功返回1，失败（钱不够等）返回0
    static void refill_coins();   // 补满金币到当前商店等级的上限

    // 从外部加载完整卡池（含等级），会在内部按等级分桶。
    // items: 长度为 count 的数组，level 约束在 1-5，越界会被夹紧到范围内。
    static void load_card_pool(int count);
    static int is_wrang(int a);
    static int get_upgrade_cost(); // 获取升级到下一等级的费用

private:
    //const int wrang[20]={};
    static void generate_shop();   // 生成当前三张卡
    static int pick_card();        // 按商店等级选一张卡（密度梯度抽卡）
    static int get_coin_limit();   // 获取当前商店等级的金币上限

};

#endif // GACHA_H
