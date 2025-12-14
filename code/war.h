#include "define_the_const.h"

#ifndef WAR_H
#define WAR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace role {
struct enemy;
class enemy_change;
struct role_base;
struct role_current;
struct team;
extern const struct role::role_base all_role_base[all_role_number];
extern struct role::team left_team;
extern struct enemy* root;
}

namespace fight{
using namespace role;
struct fight_current;
class a_fight;
struct change;
}
QT_END_NAMESPACE

struct role::role_base//角色基本参数（常数）
{
    char name[20];
    int name_number;
    int level;
    int blood;
    int attack[2];
    int camp[2];
};

struct role::role_current//角色实时状态
{
    int name_number;//角色职业
    int blood;//实时血量
    float camp_influence;//阵营的影响
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
    struct role::enemy *next;
};

struct fight::fight_current{

};

class fight::a_fight{
public:
    static void tmp();
    static struct fight::change a_attack(int t);
    static void initialize_role();
};

struct fight::change{
    int left[6][2];
    //第一个数字代表是第几个角色数据有变化，比如左边队伍从右往左第2个角色扣1滴血 left[0][0]=2//第二个角色 left[0][0]=-1//扣1滴血，回血就是正的 多个变化就left[1],left[2]多的全部归0
    int right[6][2];
    //第一个数字代表是第几个角色数据有变化，比如右边队伍从左往右第2个角色扣1滴血
};

class role::enemy_change{
public:
    void die(int num);
    void add();
};


#endif // WAR_H


