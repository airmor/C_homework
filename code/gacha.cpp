#include "gacha.h"
#include "random.h"
#include "war.h"

#include <cstring>
#include <vector>
#include <array>
#include <algorithm>

namespace cha
{
int cha[3];
int coin;
int shop_level;

namespace
{
// 每级商店的卡索引桶，索引对应 card_pool 的位置；用 1-5 级，0 号桶留空。
std::array<std::vector<int>, 6> buckets;
std::vector<CardInfo> card_pool;
}

// 下面四个函数的用处见头文件。
namespace
{
bool pay(int cost)
{
    if (coin < cost)
        return false;
    coin -= cost;
    return true;
}
}

void gacha::cha_begin()
{
    coin = 1;
    shop_level = 1;
    for (int i = 0; i < 3; ++i)
        cha[i] = -1;
    role::left_team.num = 0;
    for (int i = 0; i < max_of_a_team; ++i)
    {
        role::left_team.each[i].name_number = 0;
        role::left_team.each[i].blood = 0;
        role::left_team.each[i].camp_influence = 1.0f;
    }
    generate_shop();
    printf("%d %d %d",cha[0],cha[1],cha[2]);
}

void gacha::load_card_pool( int count)
{
    card_pool.clear();
    for (auto &v : buckets)
        v.clear();
    if (role::all_role_base == nullptr || count <= 0)
        return;

    card_pool.reserve(count);
    for (int i = 0; i < count; ++i)
    {
        CardInfo info;
        info.level=role::all_role_base[i].level;
        info.name_number=role::all_role_base[i].name_number;
        // 夹紧等级到 1-5
        if (info.level < 1)
            info.level = 1;
        else if (info.level > 5)
            info.level = 5;
        printf("level: %d %d\n",info.level,info.name_number);
        int idx = static_cast<int>(card_pool.size());
        card_pool.push_back(info);
        buckets[info.level].push_back(idx);
    }
}

int gacha::cha_buy(int num)
{
    if (num < 0 || num >= max_of_card_pool)
        return 0;
    if (cha[num] < 0)
        return 0;
    if (role::left_team.num >= max_of_a_team)
        return 0;
    int price = shop_level;
    if (!pay(price))
        return 0;

    int name = cha[num];
    int pos = role::left_team.num;
    role::left_team.each[pos].name_number = name;
    role::left_team.each[pos].blood = role::all_role_base[name].blood;
    role::left_team.each[pos].camp_influence = 1.0f;
    role::left_team.num++;

    cha[num] = -1;
    return 1;
}

int gacha::cha_sell(int num)
{
    if (num < 0 || num >= role::left_team.num)
        return 0;
    coin += 1;
    for (int i = num; i < role::left_team.num - 1; ++i)
    {
        role::left_team.each[i] = role::left_team.each[i + 1];
    }
    if (role::left_team.num > 0)
    {
        role::left_team.each[role::left_team.num - 1].name_number = 0;
        role::left_team.each[role::left_team.num - 1].blood = 0;
        role::left_team.each[role::left_team.num - 1].camp_influence = 1.0f;
        role::left_team.num--;
    }
    return 1;
}

int gacha::cha_up()
{
    if (shop_level >= 5)
        return 0;
    if (!pay(1))
        return 0;
    ++shop_level;
    return 1;
}

int gacha::re()
{
    if (!pay(1))
        return 0;
    generate_shop();
    return 1;
}

void gacha::generate_shop()
{
    for (int i = 0; i < max_of_card_pool; ++i)
    {
        cha[i] = pick_card();
    }
}

int gacha::pick_card()
{
    // 按当前商店等级优先选高等级池，不足则降级兜底。
    int lvl = std::min(shop_level, 5);
    for (int cur = lvl; cur >= 1; --cur)
    {
        auto &bucket = buckets[cur];
        if (!bucket.empty())
        {
            int pick = bucket[random::a_to_b(0, static_cast<int>(bucket.size()) - 1)];
            return card_pool[pick].name_number;
        }
    }
    printf("err\n");
    // 如果外部未加载卡池，则返回 -1 表示空槽。
    return -1;
}

}
