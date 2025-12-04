#include "define_the_const.h"

#ifndef WAR_H
#define WAR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace role {
struct role_base;
struct role_current;
struct team;
extern const struct role::role_base all_role_base[all_role_number];
extern struct role::team left;
extern struct role::team right;
}

namespace fight{
using namespace role;
struct fight_current;
class a_fight;
}
QT_END_NAMESPACE

struct role::role_base
{
    char name[20];
    int name_number;
    int blood;
    int defence;
    int attack;
    int camp[2];
};

struct role::role_current
{
    int blood;
    float camp_influence;
};

struct role::team
{
    int num;
    struct role::role_current each[max_of_a_team];
};

struct fight::fight_current{

};


#endif // WAR_H
