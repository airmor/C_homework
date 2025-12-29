#include "define_the_const.h"

#ifndef WAR_H
#define WAR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace role {
struct enemy;
class enemy_change;
struct enemy_base;
struct role_base;
struct role_current;
struct team;
class skill;
class enemy_skill;
extern const struct role::role_base all_role_base[all_role_number];
extern struct role::team left_team;
extern struct enemy* root;
extern const struct role::enemy_base all_enemy_base[all_enemy_number];
}

namespace fight{
using namespace role;
struct fight_current;
class a_fight;
struct change;
void turn_end_skills();
void battle_start_skills();
}
QT_END_NAMESPACE

struct role::role_base//角色基本参数（常数）
{
    char name[20];
    int name_number;
    int level;
    int cost;
    int blood;
    int attack;
    int camp[2];
    void (*skill_on_start)(role_current* self);                    // 战斗开始时
    void (*skill_on_attack)(role_current* self, enemy* target);    // 攻击时
    void (*skill_on_hurt)(role_current* self, int damage);         // 受到伤害时
    void (*skill_on_death)(role_current* self);                    // 死亡时
    void (*skill_on_turn_end)(role_current* self);                 // 回合结束时
};

struct role::role_current//角色实时状态
{
    int name_number;//角色职业
    int current_blood;//实时血量
    int max_blood;
    int attack;

    int base_max_blood;   // 基础最大血量
    int base_attack;      // 基础攻击力
    float camp_influence;//阵营的影响

    int shield;           // 护盾值
    int attack_buff;      // 临时攻击增益
    int debuff_count;     // 负面状态计数
    int first_blood; //1为已死
    int growth_data1;  // 多用途（经验/层数/计数）
    int growth_data2;  // 第二个计数器
    int growth_flag;   // 状态标记
    int is_ghost;
};
struct summon_unit {
    int base_attack;       // 召唤物的基础攻击
    int base_max_blood;    // 召唤物的基础生命
    int current_blood;     // 召唤物的当前生命
    int attack;            // 召唤物的当前攻击
    int max_blood;         // 召唤物的最大生命
    int original_unit_idx; // 对应的原始单位索引
    int is_summon;         // 标记为召唤物
};

struct role::team
{
    int num;//队伍数量
    struct role::role_current each[max_of_a_team];//每个队员实时状态
};

struct role::enemy{
    int name;
    int level;
    int blood;
    int base_attack;
    int current_attack;
    int attack_debuff;
    int vulnerable;
    struct role::enemy *next;
};

struct fight::fight_current{

};

class fight::a_fight{
public:
    static void tmp();
    static struct fight::change a_attack(int t);
    static void initialize_role();
    static void calculate_camp_influence();
};

struct fight::change{
    int left[6][2];
    //left[i][j]第一个数字代表是第i个角色数据有没有变化有为非1，没有为0，j表示变化,回血就是正的,扣血为负数
    int right[6][2];
};

class role::enemy_change{
public:
    static void die(enemy* p);
    static void add(int num);
};

struct role::enemy_base{
    char name[20];
    int name_number;
    int blood;
    int attack;
    int current_attack;   // 当前攻击力（受debuff影响）
    int attack_debuff;    // 攻击力减益层数
    void (*skill_on_attack)(enemy* self, role_current* target);    // 攻击时
    void (*skill_on_hurt)(enemy* self, int damage);                // 受到伤害时
    void (*skill_on_start)(enemy* self);                          // 战斗开始时
    void (*skill_on_death)(enemy* self);                          // 死亡时
    void (*skill_on_turn_end)(enemy* self);                       // 回合结束时
};

class role::skill{
    void skill0();
    //………………
};

class role::enemy_skill{
    int zhanshi(int level);
    int fashi(int level);
    int sheshou(int level);
    int mushi(int level);
};


#endif // WAR_H


