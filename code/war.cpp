#include <stdio.h>
#include "war.h"

namespace role {
const struct role_base all_role_base[all_role_number]={
    //all_role_nunber在头文件中改
    //name,name_number_blood,defence,attack,camp[2]
    {"战士",0,15,15,8,{1,1}},
    {"法师",1,15,15,8,{1,1}},
    {"牧师",2,15,15,8,{1,1}},
    {"射手",3,15,15,8,{1,1}}
};
struct team left_team;
struct team right_team;
}

void fight::a_fight::tmp()
{

    role::left_team.num=4;
    role::left_team.each[0]={0,1,1.1};
    role::left_team.each[1]={1,1,1.1};
    role::left_team.each[2]={2,1,1.1};
    role::left_team.each[3]={3,1,1.1};
}
