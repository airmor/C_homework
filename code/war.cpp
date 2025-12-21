#include <stdio.h>
#include "war.h"
#include "random.h"
using namespace role;
using namespace random;
namespace skill_store1 {
    using namespace role;
    using namespace random;
    // 技能1：战斗开始时对随机敌人造成1点伤害
    void opening_damage(role_current* self, enemy*, int) {
        if (role::root == nullptr) return;
        
        enemy* current = role::root;
        int enemy_count = 0;
        while (current != nullptr) {
            enemy_count++;
            current = current->next;
        }
        
        if (enemy_count > 0) {
            int target_index = random::a_to_b(0,enemy_count-1);
            current = role::root;
            for (int i = 0; i < target_index && current != nullptr; i++) {
                current = current->next;
            }
            
            if (current != nullptr && current->blood > 0) {
                current->blood -= 1; // 造成1点伤害
                printf("[技能] %s 战斗开始时对敌人造成1点伤害\n", 
                       role::all_role_base[self->name_number].name);
            }
            if (current->blood<=0) enemy_change::die(current);
        }
    }
    void weaken_attack(role_current* self, enemy* target,int) {
        if (target == nullptr) return;
        
        if (random::a_to_b(1, 100) <= 25) { 
            target->attack_debuff += 1;
            target->current_attack = target->base_attack - target->attack_debuff;
            printf("[技能] %s 削弱了敌人的攻击力（%d -> %d）\n", role::all_role_base[self->name_number].name,target->base_attack, target->current_attack);
        }
        if (target->attack_debuff>=5) enemy_change::die(target);
    }
    void self_damage(role_current* self, enemy* target,int) {
        if (target == nullptr) return;
        self->current_blood -= 1; 
        printf("[技能] %s 在攻击时受到1点反噬伤害\n",role::all_role_base[self->name_number].name);
    }
    void heal_1(role_current* self, enemy*, int) {
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }
        
        if (self_index == -1) return;
        if (self_index < role::left_team.num - 1) {
            role_current* right = &role::left_team.each[self_index + 1];
            if (right->current_blood > 0 && right->current_blood < right->max_blood) {
                right->current_blood += 1;
                printf("[技能] %s 治疗了右侧队友1点生命\n", role::all_role_base[self->name_number].name);
				return;            
			}
        }
        if (self_index > 0) {
            role_current* left = &role::left_team.each[self_index - 1];
            if (left->current_blood > 0 && left->current_blood < left->max_blood) {
                left->current_blood += 1;
                printf("[技能] %s 治疗了左侧队友1点生命\n", role::all_role_base[self->name_number].name);
            }
        }
        
    }
    void  soul_gain(role_current* self, enemy*, int)
   {
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) 
		{
            if ((role::left_team.each[i].current_blood==0)&&(role::left_team.each[i].first_blood==1)){
             	self->attack+=1;
                role::left_team.each[i].first_blood=0;
             	printf("[技能] %s 将冰冷的尸体转化为温暖的力量，攻击力+1\n",role::all_role_base[self->name_number].name);
           }
		}
	}
    void extra_debuff_damage(role_current* self, enemy* target,int) {
        if (target == nullptr) return;
        enemy* current1 = role::root;
        printf("[技能] %s 对有负面状态的敌人造成额外伤害\n",role::all_role_base[self->name_number].name);
        while (current1 != nullptr)
		{
		 	if (current1->attack_debuff!=0) 
			{
			 	current1->blood-=current1->attack_debuff;
		    }
		    if (current1->blood <= 0) {
                    role::enemy_change::die(current1);
                }
            current1 = current1->next;
        }
    }
    void random_second_target(role_current* self, enemy* first_target,int)
	{
        if (random::a_to_b(1, 100) <= 25) 
		{
            if (role::root == nullptr) return;
            
            const int MAX_ENEMIES = 10;
            enemy* enemies[MAX_ENEMIES];
            int count = 0;
            
            enemy* current = role::root;
            while (current != nullptr && count < MAX_ENEMIES) {
                if (current != first_target) 
				{ 
                    enemies[count++] = current;
                }
                current = current->next;
            }
            
            if (count > 0) {
                int idx = random::a_to_b(0, count - 1);
                enemy* second_target = enemies[idx];
                
                int damage = self->attack;
                second_target->blood -= damage;
                
                printf("[技能] %s 随机攻击了另一个敌人，造成%d点伤害\n",role::all_role_base[self->name_number].name, damage);
                
                if (second_target->blood <= 0) {
                    role::enemy_change::die(second_target);
                }
            }
        }
    }
    void damage_reduction(role_current* self, enemy* ,int damage)
	{
        damage-=1;
        if (damage<1)damage=1; // 最少造成1点伤害
        printf("[技能] %s 的护甲减免了1点伤害\n",role::all_role_base[self->name_number].name);
    }
    void death_gain(role_current* self, enemy*, int)
	{
        if (role::left_team.num <= 1) return; 
        int count=0;
        int alive[6]; 
        
        for (int i = 0; i < role::left_team.num; i++) 
		{
            if (&role::left_team.each[i] != self &&role::left_team.each[i].current_blood > 0)
			 {
                alive[count++]=i;
            }
        }
        if (count>0) {
            int idx = random::a_to_b(0,count-1);
            int target_idx = alive[idx];
            role_current* target = &role::left_team.each[target_idx];
            target->attack+=2;
            printf("[技能] %s 死亡时，使 %s 获得+2攻击力\n", role::all_role_base[self->name_number].name,role::all_role_base[target->name_number].name);
        }
    }
    void give_shield(role_current* self, enemy*, int)
	{
        if (role::left_team.num > 0) {
            role_current* front = &role::left_team.each[0];
            if (front->current_blood>0) {
                front->shield+=1; 
                printf("[技能] %s 为前排提供了1点护盾\n",role::all_role_base[self->name_number].name);
            }
        }
    }
    // 空技能（占位用）
    void no_skill(role_current* self, enemy*, int) {}
    void no_skill_with_target(role_current* self, enemy* target,int) {}
}
namespace role {
const struct role_base all_role_base[all_role_number]={
    // name,name_number,level,blood,attack,camp[2]
    {
    	"  ",0,1,1,2,{0,1},
		skill_store1::opening_damage,      // 战斗开始时：造成1点伤害
        skill_store1::no_skill_with_target, // 攻击时：无技能
        skill_store1::no_skill,            // 受到伤害时：无技能
        skill_store1::no_skill,            // 死亡时：无技能
        skill_store1::no_skill  
	} ,
	{
        "  ",1,1,3,1,{0,2},
        skill_store1::no_skill,
        skill_store1::weaken_attack,         
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::no_skill
    },
    {
        "邪术学徒",2,1,2,3,{2,2}, 
        skill_store1::no_skill,
        skill_store1::self_damage,      
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::no_skill
    },
    {
        "学徒牧师",3,1,1,1,{1,0}, 
        skill_store1::no_skill,
        skill_store1::no_skill_with_target,
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::heal_1 
    },
    {
        "暗影信徒",4,1,2,1, {1,2},
        skill_store1::no_skill,
        skill_store1::no_skill_with_target,
        skill_store1::no_skill,
        skill_store1::soul_gain,      
        skill_store1::no_skill
    },
    {
        "弩手",5,1,1,2,{2,1}, 
        skill_store1::no_skill,
        skill_store1::extra_debuff_damage,   
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::no_skill
    },
    {
        "游侠",6,1,3,2,{2,0}, 
        skill_store1::no_skill,
        skill_store1::random_second_target,       
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::no_skill
    },
    {
        "盾卫",7,1,4,1,{1,1}, 
        skill_store1::no_skill,
        skill_store1::no_skill_with_target,
        skill_store1::damage_reduction, 
        skill_store1::no_skill,
        skill_store1::no_skill
    },
    {
        "预言学徒",8,1,1,2,{2,1}, 
        skill_store1::no_skill,
        skill_store1::no_skill_with_target,
        skill_store1::no_skill,
        skill_store1::death_gain,        
        skill_store1::no_skill
    },
    {
        "治愈学徒",9,1,2,1,{0,0}, 
        skill_store1::give_shield,
        skill_store1::no_skill_with_target,
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::no_skill
    },
    {
        "侦察兵",10,1,4,2,{1,1},
        skill_store1::no_skill,
        skill_store1::no_skill_with_target,
        skill_store1::no_skill,
        skill_store1::no_skill,
        skill_store1::no_skill
    }
};
const struct enemy_base all_enemy_base[4]={
    {"侏儒战士",0,15,7,7,0},  // 15血，7点攻击
    {"侏儒法师",1,15,7,7,0},
    {"侏儒牧师",2,15,7,7,0},
    {"侏儒射手",3,15,7,7,0}
};
struct team left_team;
struct enemy* root;
}
using namespace role;

//这个是敌人的技能，返回造成的伤害，全局变量的改变在这里完成。
/*int role::enemy_skill::zhanshi(int level)
{

}

int role::enemy_skill::fashi(int level)
{

}

int role::enemy_skill::sheshou(int level)
{

}

int role::enemy_skill::mushi(int level)
{

}
*/
void role::enemy_change::die(enemy* p) // 敌人用一个链表来管理,指针指向的敌人死亡
{
    if (root == nullptr || p == nullptr) return;

    if (root == p) {
        enemy* temp = root;
        root = root->next;
        delete temp; 
        return;
    }

    enemy* current = root;
    while (current != nullptr && current->next != p) {
        current = current->next;
    }
    
    if (current != nullptr) {
        current->next = p->next;
        delete p;
    }
}

void role::enemy_change::add(int num) //生成一个链表的敌人,个数为num
{

}

fight::change fight::a_fight::a_attack(int t)
{
    fight::change all;
    memset(&all, 0, sizeof(all));
    role_current* left_attacker = nullptr;
    enemy* right_attacker = nullptr;
    role_current* left_defender = nullptr;
    enemy* right_defender = nullptr;
    
    if (t == 0) { 
        if (left_team.num <= 0 || left_team.each[0].current_blood <= 0) return all;
        if (root == nullptr || root->blood <= 0) return all;
        
        role_current* left_attacker = &left_team.each[0];
        enemy* right_defender = root;
        //确定攻击对象
        
        if (all_role_base[left_attacker->name_number].skill_on_attack != NULL) {
            all_role_base[left_attacker->name_number].skill_on_attack(left_attacker, right_defender,1);
        }
        //有无“攻击时”技能 
        int damage = left_attacker->attack;
        damage = (int)(damage * left_attacker->camp_influence);
        if (damage < 1) damage = 1;
        
        right_defender->blood -= damage;
        
        all.right[0][0] = 1;
        all.right[0][1] = -damage;
        
        if (right_defender->blood <= 0) {
            enemy_change::die(right_defender);
        }
    } else { 
        if (root == nullptr || root->blood <= 0) return all;
        if (left_team.num <= 0 || left_team.each[0].current_blood <= 0) return all;
        
        enemy* right_attacker = root;
        role_current* left_defender = &left_team.each[0];
        int damage = right_attacker->current_attack;
        
        if (damage < 1) damage = 1;
        int original_blood = left_defender->current_blood;
        left_defender->current_blood -= damage;
        
        if (left_defender->current_blood < 0) {
            left_defender->current_blood = 0;
            left_defender->first_blood=1;
            damage = original_blood;
        }
        
        all.left[0][0] = 1;
        all.left[0][1] = -damage;
    }
    return all;
}
void battle_start_skills() {
    for (int i = 0; i < left_team.num; i++) {
        const role_base& base = all_role_base[left_team.each[i].name_number];
        if (base.skill_on_start != NULL) {
            base.skill_on_start(&left_team.each[i],NULL,1);
        }
    }
}
void fight::a_fight::calculate_camp_influence()
{
    static const int CAMP_COORDS[9][2] = {
        {-1, -1}, // 0:守序善良
        {-1,  0}, // 1:守序中立
        {-1, +1}, // 2:守序邪恶
        { 0, -1}, // 3:中立善良
        { 0,  0}, // 4:绝对中立
        { 0, +1}, // 5:中立邪恶
        {+1, -1}, // 6:混乱善良
        {+1,  0}, // 7:混乱中立
        {+1, +1}  // 8:混乱邪恶
    };
   const float SAME_AXIS_BONUS = 1.2f;     
	const float OPPOSITE_AXIS_PENALTY = 0.8f; 
	const float NEUTRAL = 1.04f; 

	const float MIN_INFLUENCE = 0.4f; 
	const float MAX_INFLUENCE = 2.5f; 
    if (left_team.num <= 1) {
        for (int i = 0; i < left_team.num; i++) {
            left_team.each[i].camp_influence = 1.0f;
        }
        return;
    }
    
    for (int i = 0; i < left_team.num; i++) 
	{
        const role_base& base = all_role_base[left_team.each[i].name_number];
        int camp_index = base.camp[0] * 3 + base.camp[1]; 
        const int* my_coords = CAMP_COORDS[camp_index];
        
        float total_influence = 0.0f;
        int teammate_count = 0;
        
        for (int j = 0; j < left_team.num; j++) 
		{
			
            if (i == j) continue; 
            
            float influence = 1.0f;
            
            const role_base& other_base = all_role_base[left_team.each[j].name_number];
            int other_camp_index = other_base.camp[0] * 3 + other_base.camp[1];
            const int* other_coords = CAMP_COORDS[other_camp_index];
            
            if (my_coords[0] == other_coords[0] && my_coords[0] != 0) {
                influence *= SAME_AXIS_BONUS; 
            } else if (my_coords[0] == -other_coords[0] && my_coords[0] != 0) {
                influence *=OPPOSITE_AXIS_PENALTY;  
            }
            if (my_coords[1] == other_coords[1] && my_coords[1] != 0) {
                influence *= SAME_AXIS_BONUS;  
            } else if (my_coords[1] == -other_coords[1] && my_coords[1] != 0) {
                influence *= OPPOSITE_AXIS_PENALTY;  
            }
            if (my_coords[0] == 0) influence *= NEUTRAL; 
            if (my_coords[1] == 0) influence *= NEUTRAL;  
            
            total_influence += influence;
            teammate_count++;
        }
        left_team.each[i].camp_influence = total_influence / teammate_count;
        
        float raw_average=left_team.each[i].camp_influence ;
        float dilution_factor;//稀释一下 
        if (raw_average >= 1.0f) 
		{
        	dilution_factor = 1.2335f / (0.5f + 0.7335f * teammate_count);
 		} else 
		{
    		dilution_factor = 1.333f / (0.333f + 0.667f * teammate_count);
		}

		left_team.each[i].camp_influence = 1.0f + (raw_average - 1.0f) * dilution_factor;
		
        if (left_team.each[i].camp_influence < MIN_INFLUENCE) {
            left_team.each[i].camp_influence = MIN_INFLUENCE;
        } else if (left_team.each[i].camp_influence > MAX_INFLUENCE) {
            left_team.each[i].camp_influence = MAX_INFLUENCE;
        }
    }
}

void fight::a_fight::initialize_role()
//初始化角色吧struct team left_team（已经定义好了，内容看头文件，代表角色实时状态）初始化，同时计算阵营影响也在这一步
{
	calculate_camp_influence();	
	for (int i = 0; i < left_team.num; i++)
	{
		float influence = left_team.each[i].camp_influence;
        left_team.each[i].max_blood = left_team.each[i].base_max_blood;
        left_team.each[i].attack = left_team.each[i].base_attack;
        
        left_team.each[i].max_blood = (int)(left_team.each[i].base_max_blood*influence);
        if (left_team.each[i].max_blood < 1) left_team.each[i].max_blood = 1;
        
        left_team.each[i].attack = (int)(left_team.each[i].base_attack*influence);
        if (left_team.each[i].attack < 1) left_team.each[i].attack = 1;
    }
    for (int i = 0; i < left_team.num; i++)
    {
        left_team.each[i].current_blood = left_team.each[i].max_blood;
    }
}
