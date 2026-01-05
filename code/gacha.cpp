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
            {
                my_log_::my_log("金币不足");
                return false;
            }
            coin -= cost;
            return true;
        }
    }

    void gacha::cha_begin()
    {
        coin = 3;
        shop_level = 1;
        for (int i = 0; i < 3; ++i)
            cha[i] = -1;
        role::left_team.num = 0;
        for (int i = 0; i < max_of_a_team; ++i)
        {
            role::left_team.each[i].name_number = 0;
            role::left_team.each[i].base_max_blood = 0;
            role::left_team.each[i].base_attack = 0;
            role::left_team.each[i].camp_influence = 1.0f;
        }
        generate_shop();
        //my_log_::my_log("%d %d %d", cha[0], cha[1], cha[2]);
    }

    void gacha::load_card_pool(int count)
    {
        card_pool.clear();
        for (auto &v : buckets)
            v.clear();
        if (count <= 0)
            return;

        card_pool.reserve(count);
        for (int i = 0; i < count; ++i)
        {
            CardInfo info;
            info.level = role::all_role_base[i].level;
            info.name_number = role::all_role_base[i].name_number;
            // 夹紧等级到 1-5
            if (info.level < 1 || gacha::is_wrang(i)==1)
                continue;
            else if (info.level > 5)
                info.level = 5;
            // printf("level: %d %d\n",info.level,info.name_number);
            int idx = static_cast<int>(card_pool.size());
            card_pool.push_back(info);
            buckets[info.level].push_back(idx);
        }
    }

    int gacha::is_wrang(int a)
    {
        const int wrang[]={0,1,25,13,20,24,28,31,21,16,30};
        for(int i=0;i<11;i++){
            if(a==wrang[i]) return 1;
        }
        return 0;
    }

    int gacha::cha_buy(int num)
    {
        if (num < 0 || num >= max_of_card_pool)
            return 0;
        if (cha[num] < 0)
            return 0;
        if (role::left_team.num >= max_of_a_team)
            return 0;
        int name = cha[num];
        int price = role::all_role_base[name].cost;
        if (!pay(price))
            return 0;

        int pos = role::left_team.num;
        role::left_team.each[pos].name_number = name;
        role::left_team.each[pos].base_max_blood = role::all_role_base[name].blood;
        role::left_team.each[pos].base_attack = role::all_role_base[name].attack;
        role::left_team.each[pos].camp_influence = 1.0f;
        role::left_team.num++;

        cha[num] = -1;
        return 1;
    }

    int gacha::cha_sell(int num)
    {
        if (num < 0 || num >= role::left_team.num)
            return 0;
        coin += (role::all_role_base[role::left_team.each[num].name_number].cost/2);
        for (int i = num; i < role::left_team.num - 1; ++i)
        {
            role::left_team.each[i] = role::left_team.each[i + 1];
        }
        if (role::left_team.num > 0)
        {
            role::left_team.each[role::left_team.num - 1].name_number = 0;
            role::left_team.each[role::left_team.num - 1].base_max_blood = 0;
            role::left_team.each[role::left_team.num - 1].base_attack = 0;
            role::left_team.each[role::left_team.num - 1].camp_influence = 1.0f;
            role::left_team.num--;
        }
        return 1;
    }

    int gacha::cha_up()
    {
        if (shop_level >= 3)
        {
            my_log_::my_log("已经满级！");
            return 0;
        }
        int cost = get_upgrade_cost();
        if (!pay(cost))
            return 0;
        ++shop_level;
        return 1;
    }

    int gacha::re()
    {
        if (!pay(1))
            return 0;
        generate_shop();
        //refill_coins();
        //my_log_::my_log("出现%d %d %d", cha[0], cha[1], cha[2]);
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
        // 密度梯度抽卡：根据商店等级确定各星级卡片的抽取概率
        // 概率表 [商店等级][星级-1] = 概率(0-100)
        static const int probability_table[6][5] = {
            {0, 0, 0, 0, 0},     // 0级(占位)
            {100, 0, 0, 0, 0},   // 1级: 100% 1★
            {70, 30, 0, 0, 0},   // 2级: 70% 1★, 30% 2★
            {40, 35, 25, 0, 0},  // 3级: 40% 1★, 35% 2★, 25% 3★
            {20, 30, 35, 15, 0}, // 4级: 20% 1★, 30% 2★, 35% 3★, 15% 4★
            {10, 20, 35, 25, 10} // 5级: 10% 1★, 20% 2★, 35% 3★, 25% 4★, 10% 5★
        };

        int lvl = std::min(shop_level, 5);
        int roll = random::a_to_b(1, 100);
        int cumulative = 0;

        // 按概率选择星级
        for (int star = 1; star <= 5; ++star)
        {
            cumulative += probability_table[lvl][star - 1];
            if (roll <= cumulative)
            {
                // 从该星级的卡池中随机选一张
                auto &bucket = buckets[star];
                if (!bucket.empty())
                {
                    int pick = bucket[random::a_to_b(0, static_cast<int>(bucket.size()) - 1)];
                    return card_pool[pick].name_number;
                }
            }
        }

        // 降级兜底：如果选中的星级池为空，从低星级找可用的
        for (int cur = lvl; cur >= 1; --cur)
        {
            auto &bucket = buckets[cur];
            if (!bucket.empty())
            {
                int pick = bucket[random::a_to_b(0, static_cast<int>(bucket.size()) - 1)];
                return card_pool[pick].name_number;
            }
        }

        // 如果外部未加载卡池，则返回 -1 表示空槽。
        return -1;
    }

    int gacha::get_coin_limit()
    {
        // 金币上限：1本3，2本4，3本6，4本7，5本9
        static const int coin_limits[6] = {0, 300, 4000, 6000, 7000, 9000};
        return coin_limits[std::min(shop_level, 5)];
    }

    int gacha::get_upgrade_cost()
    {
        // 升本费用：1→2需2金币，2→3需3金币，3→4需5金币，4→5需7金币
        static const int upgrade_costs[5] = {0, 5, 10, 5, 7};
        if (shop_level >= 1 && shop_level <= 2)
            return upgrade_costs[shop_level];
        return 0;
    }

    void gacha::refill_coins()
    {
        int limit = get_coin_limit();
        if (coin < limit)
        {
            coin = limit;
           my_log_::my_log("金币已补满至%d", coin);
        }
    }

}
