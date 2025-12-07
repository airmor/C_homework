#include <stdio.h>
#include "war.h"
#include "random"

namespace role {
const struct role_base all_role_base[all_role_number]={
    //all_role_nunber在头文件中改
    //name,name_number_blood,defence,attack,camp[2]
    //  \   0       1       2
    //  0守序善良|守序中立|守序邪恶
    //  1中立善良|绝对中立|中立邪恶
    //  2混乱善良|混乱中立|混乱邪恶     //阵营的影响怎么算想一下最后变成一个一位小数如0.9、1.1然后造成伤害的时候乘这个系数大于1代表有加成，小于一代表有负面影响。
    {"战士",0,15,15,{2,6},{1,1}},//15血 15防 攻击2d6 阵营绝对中立
    {"法师",1,15,15,{2,6},{1,1}},
    {"牧师",2,15,15,{2,6},{1,1}},
    {"射手",3,15,15,{2,6},{1,1}}//乱写的数据
};
struct team left_team;
struct team right_team;
}

using namespace role;

void fight::a_fight::tmp()//测试用函数，无用可注释掉。
{

    role::left_team.num=4;
    role::left_team.each[0]={0,1,1.1};
    role::left_team.each[1]={1,1,1.1};
    role::left_team.each[2]={2,1,1.1};
    role::left_team.each[3]={3,1,1.1};
}

fight::change fight::a_fight::a_attack(int t)
//这个是你们要编写的主要函数,代表一次攻击t为0时是左边对右边发起一次攻击，t不是0的是右边对左边发起一次攻击；
//要改变全局变量struct team left_team和struct team right_team（已经定义好了，内容看头文件，代表角色实时状态）
//还要将返回值fight::change类型（看头文件）的变量代表发生了什么变化（到时候有掉血和加血的动画）
//可以调用random里的函数生成随机数。要注意血量不要超过限制（小于0或大于最高血）
{

}

void fight::a_fight::initialize_role()
//初始化角色吧struct team left_team和struct team right_team（已经定义好了，内容看头文件，代表角色实时状态）初始化，同时计算阵营影响也在这一步
{

}
