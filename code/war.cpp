#include <stdio.h>
#include <string.h>
#include "war.h"
#include "random.h"
#include "mainwindow.h"
#define printf my_log
using namespace my_log_;
using namespace role;
using namespace random;
void apply_damage_to_enemy(enemy* target, int damage);
void check_unit_upgrade(role_current* unit)
{
    if (unit->growth_data1 >= 2) {
        unit->base_attack += 1;
        unit->base_max_blood += 1;
        unit->attack = unit->base_attack;
        unit->max_blood = unit->base_max_blood;
        unit->current_blood += 1;
        unit->growth_data1-=2;
        printf("  [%s] 提升了属性。基础攻击+1，基础生命+1",role::all_role_base[unit->name_number].name);
    }
}
bool create_summon_at_position(int position, int summon_id)
{
    if (role::left_team.num >= max_of_a_team) {
            printf("  队伍已满，无法召唤");
            return false;
        }
    const role_base& summon_base = role::all_role_base[summon_id];
    role_current original_team[max_of_a_team];
    int original_num = role::left_team.num;
    for (int i = 0; i < original_num; i++)
    {
        original_team[i] = role::left_team.each[i];
    }
    role::left_team.num++;
    for (int i = original_num - 1; i >= position; i--) {
            role::left_team.each[i + 1] = original_team[i];
        }
    for (int i = 0; i < position; i++)
    {
        role::left_team.each[i] = original_team[i];
    }
    role::left_team.each[position].name_number = summon_id;
        role::left_team.each[position].base_attack = summon_base.attack;
        role::left_team.each[position].base_max_blood = summon_base.blood;

        role::left_team.each[position].current_blood = summon_base.blood;
        role::left_team.each[position].max_blood = summon_base.blood;
        role::left_team.each[position].attack = summon_base.attack;

        role::left_team.each[position].shield = 0;
        role::left_team.each[position].attack_buff = 0;
        role::left_team.each[position].debuff_count = 0;
        role::left_team.each[position].first_blood = 0;
        role::left_team.each[position].is_ghost = 1;  // 标记为召唤物
        role::left_team.each[position].camp_influence = 1.0f;  // 召唤物固定阵营影响
        role::left_team.each[position].growth_data1 = 0;
        role::left_team.each[position].growth_data2 = 0;
        role::left_team.each[position].growth_flag = 0;
        return true;
 }
namespace skill_store1 {
    using namespace role;
    using namespace random;
    // 技能1：战斗开始时对随机敌人造成1点伤害
    void opening_damage(role_current* self) {
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
                printf("[技能] %s 战斗开始时对敌人造成1点伤害",
                       role::all_role_base[self->name_number].name);
            }
            if (current->blood<=0) enemy_change::die(current);
        }
    }
    void weaken_attack(role_current* self, enemy* target) {
        if (target == nullptr) return;

        if (random::a_to_b(1, 100) <= 25) {
            target->attack_debuff += 1;
            target->current_attack = target->base_attack - target->attack_debuff;
            printf("[技能] %s 削弱了敌人的攻击力（%d -> %d）", role::all_role_base[self->name_number].name,target->base_attack, target->current_attack);
        }
        if (target->attack_debuff>=5) enemy_change::die(target);
    }
    void self_damage(role_current* self, enemy* target) {
        if (target == nullptr) return;
        self->current_blood -= 1;
        printf("[技能] %s 在攻击时受到1点反噬伤害",role::all_role_base[self->name_number].name);
    }
    void heal_1(role_current* self) {
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
                printf("[技能] %s 治疗了右侧队友1点生命", role::all_role_base[self->name_number].name);
                return;
            }
        }
        if (self_index > 0) {
            role_current* left = &role::left_team.each[self_index - 1];
            if (left->current_blood > 0 && left->current_blood < left->max_blood) {
                left->current_blood += 1;
                printf("[技能] %s 治疗了左侧队友1点生命", role::all_role_base[self->name_number].name);
            }
        }

    }
    void  soul_gain(role_current* self)
   {
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++)
        {
            if ((role::left_team.each[i].current_blood==0)){
                self->attack+=1;
                role::left_team.each[i].first_blood=0;
                printf("[技能] %s 将冰冷的尸体转化为温暖的力量，攻击力+1",role::all_role_base[self->name_number].name);
           }
        }
    }
    void extra_debuff_damage(role_current* self, enemy* target) {
        if (target == nullptr) return;
        enemy* current1 = role::root;
        printf("[技能] %s 对有负面状态的敌人造成额外伤害",role::all_role_base[self->name_number].name);
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
    void random_second_target(role_current* self, enemy* first_target)
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

                printf("[技能] %s 随机攻击了另一个敌人，造成%d点伤害",role::all_role_base[self->name_number].name, damage);

                if (second_target->blood <= 0) {
                    role::enemy_change::die(second_target);
                }
            }
        }
    }
    void damage_reduction(role_current* self,int damage)
    {
        damage-=1;
        if (damage<1)damage=1; // 最少造成1点伤害
        self->current_blood-=damage;
        printf("[技能] %s 的护甲减免了1点伤害",role::all_role_base[self->name_number].name);
    }
    void death_gain(role_current* self)
    {
        if (role::left_team.num <= 1) return;
        int count=0;
        int alive[6];
        self->first_blood=1;
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
            printf("[技能] %s 死亡时，使 %s 获得+2攻击力", role::all_role_base[self->name_number].name,role::all_role_base[target->name_number].name);
        }
    }
    void give_shield(role_current* self)
    {
        if (role::left_team.num > 0) {
            role_current* front = &role::left_team.each[0];
            if (front->current_blood>0) {
                front->shield+=1;
                printf("[技能] %s 为前排提供了1点护盾",role::all_role_base[self->name_number].name);
            }
        }
    }
    // 空技能（占位用）
    void no_skill(role_current* self) {}
    void no_skill(role_current* self,int) {}
    void no_skill(role_current* self,enemy*) {}
}
    namespace skill_store2
    {
    void knight_protect(role_current* self) {
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }

        if (self_index == -1) return;

        printf("[圣殿骑士] 释放守护光环");

        self->shield = 1;
        printf("  -> 自身获得1点护盾");

        if (self_index < role::left_team.num - 1) {
            role_current* right = &role::left_team.each[self_index + 1];
            if (right->current_blood > 0) {
                right->shield = 1;
                printf("  -> 右侧[%s]获得1点护盾",role::all_role_base[right->name_number].name);
            }
        }
    }
    void pain_amplify(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int base_damage = self->attack;
        int extra_damage = 0;

        int debuff_layers = target->attack_debuff;

        if (debuff_layers > 0) {
            extra_damage = debuff_layers;
            printf("[拷问官] 折磨已受伤的敌人！基于%d层debuff追加%d点伤害",debuff_layers, extra_damage);
        }

        int total_damage = base_damage + extra_damage;

        apply_damage_to_enemy(target, total_damage);
    }

    void final_sentence(role_current* self) {
        printf("[拷问官] 临终诅咒：让敌人感受痛苦！");

        if (role::root == nullptr) {
            printf("  没有敌人可以诅咒");
            return;
        }
        enemy* last_enemy = role::root;
        enemy* previous = nullptr;

        while (last_enemy != nullptr && last_enemy->next != nullptr) {
            previous = last_enemy;
            last_enemy = last_enemy->next;
        }

        if (last_enemy == nullptr) {
            printf("  找不到最后一个敌人");
            return;
        }
        last_enemy->attack_debuff += 1;
        last_enemy->current_attack = last_enemy->base_attack-1;
        if (last_enemy->current_attack < 0) last_enemy->current_attack = 0;

        printf("  -> 最后的敌人[%s] 攻击力-%d（当前:%d）",
               last_enemy->name, 1, last_enemy->current_attack);
    }
    void blessing_priest_aoe(role_current* self) {
        printf("[祝福祭司] 释放祝福光环");

        int buffed_count = 0;

        for (int i = 0; i < role::left_team.num; i++) {
            role_current* teammate = &role::left_team.each[i];

            if (teammate->current_blood > 0) {
                teammate->attack += 1;
                buffed_count++;

                printf("  -> [%s] 攻击力+1（当前:%d）",
                       role::all_role_base[teammate->name_number].name,
                       teammate->attack);
            }
        }

        printf("  总计为%d名友军增加了攻击力", buffed_count);

        self->growth_data1 = 1;
    }
    // 祝福祭司技能2：死亡时 - 随机治疗一名友军2血
    void blessing_final_prayer(role_current* self) {
        printf("[祝福祭司] 临终祈祷");
        // 收集可治疗目标
        int candidates[max_of_a_team];
        int count = 0;

        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] != self &&
                role::left_team.each[i].current_blood > 0) {
                candidates[count++] = i;
            }
        }

        if (count == 0) {
            printf("  没有可以治疗的目标");
            return;
        }

        // 随机选择
        int idx = random::a_to_b(0, count - 1);
        role_current* target = &role::left_team.each[candidates[idx]];

        // 计算治疗量
        int heal = 2;
        int need = target->max_blood - target->current_blood;
        if (heal > need) heal = need;

        // 执行治疗
        int old_blood = target->current_blood;
        target->current_blood += heal;

        printf("  -> 治疗[%s] %d血（%d→%d）",
               role::all_role_base[target->name_number].name,
               heal, old_blood, target->current_blood);
    }
    void wild_ranger_double_tap(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int base_damage = self->attack;

        printf("[荒野射手] 射击！造成%d点伤害", base_damage);
        apply_damage_to_enemy(target, base_damage);

        if (random::a_to_b(1, 100) <= 50) {
            printf("[荒野射手] 双重射击！再造成%d点伤害", base_damage);
            apply_damage_to_enemy(target, base_damage);
        }
        if (random::a_to_b(1, 100) <= 25) {
            printf("[荒野射手] 三重射击！再造成%d点伤害", base_damage);
            apply_damage_to_enemy(target, base_damage);
        }
    }


    void wild_ranger_evade(role_current* self, int damage) {
        if (random::a_to_b(1, 100) <= 15) {
            printf("[荒野射手] 灵活闪避！躲避了%d点伤害", damage);
        } else {
            self->current_blood -= damage;
            if (self->current_blood < 0) self->current_blood = 0;
        }
    }
    void tactician_positioning(role_current* self) {
        // 找到自己在队伍中的位置
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }
        if (self_index == -1 || role::left_team.num <= 1) return;

        // 随机选择一个其他友军
        int target_index;
        do {
            target_index = random::a_to_b(0, role::left_team.num - 1);
        } while (target_index == self_index);

        role_current temp = role::left_team.each[self_index];
        role::left_team.each[self_index] = role::left_team.each[target_index];
        role::left_team.each[target_index] = temp;

        printf("[战术家] 与 [%s] 交换了位置",
               role::all_role_base[role::left_team.each[self_index].name_number].name);
    }

    // 战术家技能2：攻击时 - 若目标满血，伤害+3
    void tactician_weakpoint(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;

        // 检查目标是否满血
        if (target->blood >= all_enemy_base[target->name].blood) {  // 假设 enemy 有 base_blood 字段
            damage += 3;
            printf("[战术家] 发现弱点！对满血目标额外造成2点伤害");
        }

        apply_damage_to_enemy(target, damage);
    }
    void soul_reaper_mark(role_current* self) {
        if (role::root == nullptr) return;

        // 统计敌人数量
        enemy* current = role::root;
        int enemy_count = 0;
        while (current != nullptr) {
            enemy_count++;
            current = current->next;
        }
        if (enemy_count == 0) return;

        // 随机选择一个敌人
        int target_idx = random::a_to_b(0, enemy_count - 1);
        current = role::root;
        for (int i = 0; i < target_idx && current != nullptr; i++) {
            current = current->next;
        }

        if (current != nullptr) {
            current->vulnerable +=3;
            printf("[灵魂收割者] 给 [%s] 施加了易伤印记！", current->name);
            printf("  所有我方单位攻击该目标时额外造成3点伤害");
            self->growth_data1 = target_idx;  // 存储目标索引
        }
    }
    void soul_reaper_execute(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;

        apply_damage_to_enemy(target, damage);

        if (target->blood <= 0 && target->vulnerable > 0) {
            printf("[灵魂收割者] 收割了易伤目标的灵魂！");
            self->base_attack += 1;
            self->attack = self->base_attack;

            printf("  基础攻击力永久+1（当前：%d）", self->base_attack);
        }
    }
    void elite_trainer_inspire(role_current* self)
    {
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++)
        {
            if (&role::left_team.each[i] == self)
            {
                self_index = i;
                break;
            }
        }
        if (self_index == -1) return;

        for (int i = 0; i < self_index; i++) {
            role_current* front_unit = &role::left_team.each[i];
            if (front_unit->current_blood > 0) {
                const role_base& base = role::all_role_base[front_unit->name_number];

                if (base.camp[0] == 0)
                {
                    front_unit->growth_data1 += 1;
                    printf("  指导守序阵营的[%s]，经验+1（当前：%d/2）",
                           base.name, front_unit->growth_data1);
                    check_unit_upgrade(front_unit);
                    return;  // 只找一个
                }
            }
        }
        printf("  前方没有守序阵营的友军指导");
    }
    void elite_trainer_protect(role_current* self, int damage)
    {
        self->current_blood -= damage;
        if (self->current_blood < 0) self->current_blood = 0;

        printf("[精英训练官] 受到%d点伤害", damage);
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }
        if (self_index == -1) return;
        for (int i = self_index + 1; i < role::left_team.num; i++)
        {
            role_current* back_unit = &role::left_team.each[i];
            if (back_unit->current_blood > 0)
            {
                const role_base& base = role::all_role_base[back_unit->name_number];
                if (base.camp[1] == 0) {  // 善良
                    back_unit->growth_data1 += 1;  // 经验+1
                    printf("  激励善良阵营的[%s]，经验+1（当前：%d/2）",
                           base.name, back_unit->growth_data1);
                    check_unit_upgrade(back_unit);
                    return;  // 只找一个
                }
            }
        }
        printf("  后方没有善良阵营的友军关怀");
    }
    // 深渊吞噬者技能2：攻击时 - 若击杀，永久+1攻
    void abyss_eater_devour(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;
        int original_blood = target->blood;
        apply_damage_to_enemy(target, damage);

        // 检查是否击杀
        if (original_blood > 0 && target->blood <= 0) {
            self->base_attack += 1;  // 永久+1攻
            self->attack = self->base_attack;
            self->growth_data1 += 1;  // 击杀计数

            printf("[深渊吞噬者] 吞噬灵魂！攻击力永久+1（累计击杀：%d）",
                   self->growth_data1);
        }
    }

    // 深渊吞噬者技能3：友军死亡时 - 永久+1血（限2次）
    void abyss_eater_consume(role_current* self) {
        if (self->growth_data2 < 2) {  // 最多2次
            self->base_max_blood += 1;  // 永久+1最大生命
            self->max_blood = self->base_max_blood;
            self->current_blood += 1;   // 回1血
            self->growth_data2 += 1;

            printf("[深渊吞噬者] 吸收友军残魂！最大生命永久+1（%d/2）",
                   self->growth_data2);
            printf("  当前生命：%d/%d", self->current_blood, self->max_blood);
        }
    }
    void beast_king_frenzy(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;
        my_log("[荒野兽王]造成%d点伤害", damage);
        apply_damage_to_enemy(target, damage);
        if (random::a_to_b(1, 100) <= 50) {
            my_log("[荒野兽王] 连击再造成%d点伤害", damage);
            apply_damage_to_enemy(target, damage);
        }
    }
    void beast_king_soul(role_current* self)
    {
        my_log("[荒野兽王]化作亡魂归来！");
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }
        if (self_index == -1) return;
        my_log("  召唤了一个3攻2血的亡魂");
        self->current_blood=2;
        self->max_blood=2;
        self->attack=3;
        self->attack_buff=0;
        self->shield=0;
        self->debuff_count=0;
        self->is_ghost=1;
        if (self_index < role::left_team.num - 1) {
            role_current* back_unit = &role::left_team.each[self_index + 1];
            if (back_unit->current_blood > 0) {
                back_unit->current_blood += 2;
                if (back_unit->current_blood > back_unit->max_blood) {
                    back_unit->current_blood = back_unit->max_blood;
                }
                my_log("  亡魂立即强化了后方[%s]，生命+2",
                       role::all_role_base[back_unit->name_number].name);
            }
        }
    }
    void ghost_heal_back(role_current* self)
    {
        if (self->is_ghost == 0) return;  // 不是亡魂

        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }

        if (self_index == -1) return;

        my_log("[亡魂] 释放灵魂能量");

        if (self_index < role::left_team.num - 1) {
            role_current* back_unit = &role::left_team.each[self_index + 1];
            if (back_unit->current_blood > 0 && back_unit->growth_flag == 0) {
                int old_blood = back_unit->current_blood;
                back_unit->current_blood += 2;
                if (back_unit->current_blood > back_unit->max_blood) {
                    back_unit->current_blood = back_unit->max_blood;
                }
                if (back_unit->current_blood > old_blood) {
                    my_log("  强化后方[%s]，生命+2（%d→%d）",
                           role::all_role_base[back_unit->name_number].name,
                           old_blood, back_unit->current_blood);
                }
            }
        }
    }
    void flame_mage_mark(role_current* self) {
        if (role::root == nullptr) return;
        enemy* current = role::root;
        int enemy_count = 0;
        enemy* enemies[20];

        while (current != nullptr && enemy_count < 20) {
            enemies[enemy_count++] = current;
            current = current->next;
        }

        if (enemy_count == 0) return;

        int target_idx = random::a_to_b(0, enemy_count - 1);
        enemy* target = enemies[target_idx];
        target->vulnerable += 2;

        my_log("[烈焰法师] 给[%s]施加了2层易伤！", target->name);
        my_log("  所有攻击对该目标额外造成2点伤害");
    }

    // 烈焰法师技能2：攻击时 - 若目标有易伤，蔓延到另一个敌人
    void flame_mage_spread_vuln(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;
        apply_damage_to_enemy(target, damage);
        if (target->vulnerable > 0) {
            if (role::root == nullptr) return;
            enemy* current = role::root;
            int available_count = 0;
            enemy* available[20];

            while (current != nullptr && available_count < 20) {
                if (current != target) {
                    available[available_count++] = current;
                }
                current = current->next;
            }

            if (available_count > 0) {
                int spread_idx = random::a_to_b(0, available_count - 1);
                enemy* spread_target = available[spread_idx];

                // 给目标加1-2层易伤
                int spread_amount = random::a_to_b(1, 2);
                spread_target->vulnerable += spread_amount;

                my_log("[烈焰法师] 易伤蔓延！[%s]获得%d层易伤（总计易伤目标：%d）",
                       spread_target->name, spread_amount,spread_target->vulnerable);
            }
        }
    }

    // 烈焰法师技能3：死亡时 - 所有有易伤的敌人受到2点伤害
    void flame_mage_final_burn(role_current* self) {
        my_log("[烈焰法师] 临终灼烧！");
        int burned_count = 0;
        enemy* current = role::root;

        while (current != nullptr) {
            if (current->vulnerable > 0) {
                current->blood -= 2;  // 造成2点伤害
                burned_count++;

                my_log("  灼烧[%s]（%d层易伤），造成2点伤害（剩余血量：%d）",
                       current->name, current->vulnerable, current->blood);

                if (current->blood <= 0) {
                    enemy* next = current->next;
                    role::enemy_change::die(current);
                    current = next;
                    continue;
                }
            }
            current = current->next;
        }
        my_log("  总计灼烧了%d个敌人", burned_count);
    }
    void summon_instructor_start(role_current* self)
    {
        const int SKELETON_SUMMON_ID = 22;
        if(create_summon_at_position(0, SKELETON_SUMMON_ID)){
            printf("[召唤教官] 召唤了骷髅战士");
        }
    }
    void summon_instructor_buff(role_current* self) {
        printf("[召唤教官] 强化所有召唤物！");

        int buffed_count = 0;

        for (int i = 0; i < role::left_team.num; i++) {
            role_current* unit = &role::left_team.each[i];

            if (unit->is_ghost && unit != self)
            {
                unit->attack += 2;
                unit->max_blood += 2;
                unit->current_blood += 2;
                printf("  -> 强化[%s] 攻+2 血+2（当前:攻%d 血%d/%d）",
                   role::all_role_base[unit->name_number].name,
                   unit->attack,
                   unit->current_blood,
                   unit->max_blood);
            }
        }
    }
    void pain_conduction_death(role_current* self)
    {
    printf("[痛苦传导者] 临终激励所有友军！");

    int buffed_count = 0;
    for (int i = 0; i < role::left_team.num; i++) {
        role_current* unit = &role::left_team.each[i];
        if (unit != self && unit->current_blood > 0) {
            unit->attack += 1;
            printf("  -> [%s] 攻击力+1（当前:%d）",
                   role::all_role_base[unit->name_number].name,
                   unit->attack);
        }
        }
    }
    void pain_conduction(role_current* self, int damage) {
        self->current_blood-=damage;
        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }
        if (self_index != -1 && self_index < role::left_team.num - 1) {
            role_current* back_unit = &role::left_team.each[self_index + 1];
            if (back_unit->current_blood > 0) {
                back_unit->current_blood -= 1;
                printf("  痛苦传导！后方[%s]受到1点伤害",
                       role::all_role_base[back_unit->name_number].name);

                if (back_unit->current_blood <= 0) {
                    back_unit->first_blood = 1;
                    if (role::all_role_base[back_unit->name_number].skill_on_death != NULL) {
                        role::all_role_base[back_unit->name_number].skill_on_death(back_unit);
                    }
                }
            }
        }
    }
    void pain_scream(role_current* self, int damage) {
        self->current_blood -= damage;

        if (role::root != nullptr) {
            enemy* current = role::root;
            int enemy_count = 0;
            enemy* enemies[10];

            while (current != nullptr && enemy_count < 10) {
                enemies[enemy_count++] = current;
                current = current->next;
            }

            if (enemy_count > 0) {
                int target_idx = random::a_to_b(0, enemy_count - 1);
                enemy* target = enemies[target_idx];

                target->blood -= 3;
                printf("  痛苦尖啸！随机对[%s]造成3点伤害", target->name);

                if (target->blood <= 0) {
                    role::enemy_change::die(target);
                }
            }
        }
    }
    void no_skill(role_current* self) {}
    void no_skill(role_current* self,int) {}
    void no_skill(role_current* self,enemy*) {}
}
namespace skill_store3 {
    void  revenging3(role_current* self)
{
    printf("[技能] %s 死亡时对所有敌人造成2点伤害",
           role::all_role_base[self->name_number].name);

    enemy* current = role::root;
    while (current != nullptr) {
        if (current->blood > 0) {
            current->blood -= 2;
            if (current->blood <= 0) {
                role::enemy_change::die(current);
            }
        }
        current = current->next;
    }
}

void heal_1(role_current* self) {
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
                right->current_blood += self->attack;
                printf("[技能] %s 治疗了右侧队友 %s 点生命", role::all_role_base[self->name_number].name,self->attack);
                if (right->current_blood>right->max_blood)  right->current_blood=right->max_blood;
            }
        }
        if (self_index > 0) {
            role_current* left = &role::left_team.each[self_index - 1];
            if (left->current_blood > 0 && left->current_blood < left->max_blood) {
                left->current_blood += self->attack;
                printf("[技能] %s 治疗了左侧队友 %s 点生命", role::all_role_base[self->name_number].name,self->attack);
                if (left->current_blood>left->max_blood)  left->current_blood=left->max_blood;
            }
        }

    }
void reviving(role_current* self) {

    if (self->first_blood ==1) {
        int revive_hp = (self->max_blood+1) / 2;
        if (revive_hp < 1) revive_hp = 1;

        self->current_blood = revive_hp;
        self->first_blood = 2;

        printf("[技能] %s 死亡后复活，恢复一半生命",
               role::all_role_base[self->name_number].name);
    }
}

void assassin_dodge(role_current* self, int damage) {
    if (random::a_to_b(1, 100) <= 15) {
        printf("[闪避] %s 灵巧地闪避了攻击！",
               role::all_role_base[self->name_number].name);
        damage = 0;
    }

    if (damage > 0) {
        self->current_blood -= damage;
    }
}


void opening_damage3(role_current* self) {
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
                current->blood -= 3; // 造成3点伤害
                printf("[技能] %s 战斗开始时对敌人造成3点伤害",
                       role::all_role_base[self->name_number].name);
            }
            if (current->blood<=0) enemy_change::die(current);
        }
    }

void assassin_critical_strike(role_current* self, enemy* target) {
    if (target == nullptr) return;

    int damage = self->attack;

    if (random::a_to_b(1, 100) <= 40) {
        damage *= 2;
        printf("[致命一击] %s 打出暴击！",
               role::all_role_base[self->name_number].name);
    }

    target->blood -= damage;


    if (target->blood <= 0) {
        role::enemy_change ec;
        ec.die(target);
    }
}
 void dragon_warrior_attack(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int main_damage = self->attack;
        int splash_damage = main_damage / 2;  // 一半伤害

        printf("[龙裔战士] 龙息攻击！造成%d点主要伤害", main_damage);

        // 对主目标造成伤害
        target->blood -= main_damage;

        // 寻找目标左右相邻的敌人
        enemy* left_target = nullptr;
        enemy* right_target = nullptr;
        enemy* current = role::root;

        // 找到目标在链表中的位置
        while (current != nullptr) {
            if (current == target) {
                break;
            }
            current = current->next;
        }

        if (current != nullptr) {
            // 找到目标的前一个敌人（左边）
            enemy* prev = role::root;
            while (prev != nullptr && prev->next != current) {
                prev = prev->next;
            }
            left_target = prev;

            // 找到目标的后一个敌人（右边）
            right_target = current->next;
        }

        // 对左边敌人造成溅射伤害
        if (left_target != nullptr && left_target->blood > 0) {
            left_target->blood -= splash_damage;
            printf("  -> 溅射到左边[%s]，造成%d点伤害",
                   left_target->name, splash_damage);

            if (left_target->blood <= 0) {
                printf("  -> 溅射击杀了[%s]", left_target->name);
                role::enemy_change::die(left_target);
            }
        }

        // 对右边敌人造成溅射伤害
        if (right_target != nullptr && right_target->blood > 0) {
            right_target->blood -= splash_damage;
            printf("  -> 溅射到右边[%s]，造成%d点伤害",
                   right_target->name, splash_damage);

            if (right_target->blood <= 0) {
                printf("  -> 溅射击杀了[%s]", right_target->name);
                role::enemy_change::die(right_target);
            }
        }

        // 检查主目标是否死亡
        if (target->blood <= 0) {
            printf("  -> 主要目标[%s]被击杀", target->name);
            role::enemy_change::die(target);
        }
    }
    void dragon_warrior_hurt(role_current* self, int damage) {
        // 先承受伤害
        self->current_blood -= damage;

        if (damage > 0 && self->current_blood > 0) {
            // 受伤后永久提升攻击力
            self->base_attack += 1;
            self->attack+=1;

            printf("[龙裔战士] 龙血沸腾！基础攻击永久+1（当前：%d）",
                   self->base_attack);
        }
    }
    void holy_knight_start(role_current* self) {
        printf("[圣光骑士] 释放神圣护盾！");

        int shielded_count = 0;

        for (int i = 0; i < role::left_team.num; i++) {
            role_current* unit = &role::left_team.each[i];
            if (unit->current_blood > 0) {
                const role_base& base = role::all_role_base[unit->name_number];
                if (base.camp[1]==0) unit->shield += 1;

                printf("  -> [%s] 获得1点护盾",
                       role::all_role_base[unit->name_number].name);
            }
        }
    }

    // 圣光骑士攻击时：攻击后治疗血量最低的友军2点
    void holy_knight_attack(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;
        printf("[圣光骑士] 神圣打击！造成%d点伤害", damage);

        target->blood -= damage;

        if (target->blood <= 0) {
            role::enemy_change::die(target);
        }

        role_current* lowest_hp_unit = nullptr;
        int lowest_percentage = 101;  // 生命百分比

        for (int i = 0; i < role::left_team.num; i++) {
            role_current* unit = &role::left_team.each[i];

            if (unit != self && unit->current_blood > 0) {
                int percentage = (unit->current_blood * 100) / unit->max_blood;

                if (lowest_hp_unit == nullptr ||
                    percentage < lowest_percentage ||
                    (percentage == lowest_percentage &&
                     unit->current_blood < lowest_hp_unit->current_blood)) {
                    lowest_hp_unit = unit;
                    lowest_percentage = percentage;
                }
            }
        }

        // 治疗血量最低的友军
        if (lowest_hp_unit != nullptr) {
            int old_blood = lowest_hp_unit->current_blood;
            lowest_hp_unit->current_blood += 2;

            // 不超过最大生命值
            if (lowest_hp_unit->current_blood > lowest_hp_unit->max_blood) {
                lowest_hp_unit->current_blood = lowest_hp_unit->max_blood;
            }

            if (lowest_hp_unit->current_blood > old_blood) {
                printf("  -> 治疗了血量最低的[%s] 2点生命（%d→%d）",
                       role::all_role_base[lowest_hp_unit->name_number].name,
                       old_blood, lowest_hp_unit->current_blood);
            }
        }
    }
    void holy_knight_hurt(role_current* self, int damage) {
        int original_damage = damage;

        damage -= 2;
        if (damage < 1) damage = 1;

        if (original_damage > damage) {
            printf("[圣光骑士] 神圣防护减免了%d点伤害（%d→%d）",
                   original_damage - damage, original_damage, damage);
        }
        self->current_blood -= damage;
        if (self->current_blood < 0) self->current_blood = 0;
    }
    // 暗影巫师战斗开始时：随机诅咒两个敌人（attack_debuff+1）
    void shadow_mage_start(role_current* self)
    {
        printf("[暗影巫师] 释放双重暗影诅咒！");

        if (role::root == nullptr)
        {
            printf("  没有敌人可以诅咒");
            return;
        }

        // 统计所有敌人
        enemy* current = role::root;
        int enemy_count = 0;
        enemy* enemies[10];

        while (current != nullptr && enemy_count < 10)
        {
            enemies[enemy_count++] = current;
            current = current->next;
        }
        //printf("1");
        //QCoreApplication::processEvents(); // 强制处理所有未完成的事件（包括重绘
        if (enemy_count < 2)
        {
            printf("  敌人数量不足2个");
            // 如果只有一个敌人，只诅咒一个
            if (enemy_count == 1)
            /*{
                enemies[0]->attack_debuff += 1;
                enemies[0]->current_attack = enemies[0]->base_attack - enemies[0]->attack_debuff;
                if (enemies[0]->current_attack < 0) enemies[0]->current_attack = 0;
                printf("  -> 诅咒了[%s]，攻击力-1（当前:%d）",
                       enemies[0]->name, enemies[0]->current_attack);
            }*/
            return;
        }
        int cursed_count = 0;
        bool selected[10] = {false};

        while (cursed_count < 2) {
            int target_idx = random::a_to_b(0, enemy_count - 1);

            if (!selected[target_idx]) {
                enemy* target = enemies[target_idx];

                target->attack_debuff += 1;
                target->current_attack = target->base_attack - target->attack_debuff;
                if (target->current_attack < 0) target->current_attack = 0;

                cursed_count++;
                selected[target_idx] = true;

                printf("  -> 诅咒了[%s]，攻击力-1（当前:%d）",
                       target->name, target->current_attack);
            }
        }

        printf("  成功诅咒了%d个敌人", cursed_count);
    }

    void shadow_mage_attack(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;

        if (target->attack_debuff > 0) {
            damage += 2;
            printf("[暗影巫师] 诅咒强化！对受诅咒目标额外造成2点伤害");
        }

        printf("  造成%d点伤害", damage);

        target->blood -= damage;

        if (target->blood <= 0) {
            role::enemy_change::die(target);
        }
    }

    void shadow_mage_hurt(role_current* self, int damage) {
        printf("[暗影巫师] 痛苦转化为力量！");

        self->current_blood -= damage;

        if (role::root != nullptr) {
            enemy* current = role::root;
            int enemy_count = 0;
            enemy* enemies[10];

            while (current != nullptr && enemy_count < 10) {
                enemies[enemy_count++] = current;
                current = current->next;
            }

            if (enemy_count > 0) {
                int target_idx = random::a_to_b(0, enemy_count - 1);
                enemy* target = enemies[target_idx];

                //target->vulnerable += 3;

                printf("  -> [%s]获得3层易伤（所有攻击对其额外造成3点伤害）",
                       target->name);
                //printf("     当前易伤层数：%d", target->vulnerable);
            }
        }
    }
    void shadow_mage_death(role_current* self) {
        printf("[暗影巫师] 诅咒消散...");

        enemy* current = role::root;

        while (current != nullptr) {
            if (current->attack_debuff > 0) {
                int old_debuff = current->attack_debuff;
                current->attack_debuff -=1;
                current->current_attack = current->base_attack;

                printf("  -> 移除了[%s]的诅咒（攻击力恢复）",
                       current->name);
            }
            current = current->next;
        }
    }
    void no_skill(role_current* self) {}
    void no_skill(role_current* self,int) {}
    void no_skill(role_current* self,enemy*) {}
}
namespace skill_store5
{
    void demon_contractor_start(role_current* self) {
        printf("[恶魔契约者] 签订恶魔契约！");

        // 检查生命是否足够
        if (self->current_blood <= 2) {
            printf("  生命值不足，无法签订契约");
            return;
        }

        // 消耗2点生命
        self->current_blood -= 2;

        // 永久增加1点基础攻击
        self->base_attack += 2;
        self->attack += 2;

        printf("  消耗2点生命，攻击力永久+2");
        printf("  当前状态：攻:%d 血:%d/%d",
               self->attack, self->current_blood, self->max_blood);
    }

    // 恶魔契约者攻击时：造成伤害的50%转化为自身生命
    void demon_contractor_attack(role_current* self, enemy* target) {
        if (target == nullptr) return;

        int damage = self->attack;
        int original_blood = target->blood;
        target->blood -= damage;
        int actual_damage = original_blood - target->blood;
        if (actual_damage < 0) actual_damage = 0;
        printf("[恶魔契约者] 造成%d点伤害", actual_damage);

        // 吸血：50%伤害转化为生命
        int heal_amount = actual_damage / 2;  // 向下取整
        if (heal_amount > 0) {
            int old_blood = self->current_blood;
            self->current_blood += heal_amount;

            if (self->current_blood > self->max_blood) {
                self->current_blood = self->max_blood;
            }

            if (self->current_blood > old_blood) {
                printf("，吸血%d点（%d→%d）", heal_amount, old_blood, self->current_blood);
            }
        }

        printf("");

        if (target->blood <= 0) {
            role::enemy_change::die(target);
        }
    }

    // 恶魔契约者受伤时：立即对攻击者造成3点伤害
    void demon_contractor_hurt(role_current* self, int damage) {
        printf("[恶魔契约者] 恶魔反噬！");

        self->current_blood -= damage;

        if (role::root != nullptr && role::root->blood > 0) {
            enemy* attacker = role::root;

            attacker->blood -= 3;
            printf("  -> 对攻击者[%s]造成3点反噬伤害（剩余血量：%d）",
                   attacker->name, attacker->blood);

            if (attacker->blood <= 0) {
                printf("  -> 反噬击杀了[%s]", attacker->name);
                role::enemy_change::die(attacker);
            }
        }
    }

    void demon_contractor_death(role_current* self) {
        printf("[恶魔契约者] 释放最后的恶魔之力！");

        const int LITTLE_DEMON_ID = 42;

        int self_index = -1;
        for (int i = 0; i < role::left_team.num; i++) {
            if (&role::left_team.each[i] == self) {
                self_index = i;
                break;
            }
        }

        if (self_index != -1) {
            role_current* little_demon = &role::left_team.each[self_index];

            little_demon->name_number = LITTLE_DEMON_ID;
            little_demon->base_attack = 3;
            little_demon->base_max_blood = 4;
            little_demon->current_blood = 4;
            little_demon->max_blood = 4;
            little_demon->attack = 3;
            little_demon->is_ghost = 1;  // 标记为召唤物
            little_demon->shield = 0;

            printf("  -> 召唤了[小恶魔]在位置%d（攻:3 血:4/4）", self_index);
        }
    }

    void demon_contractor_turn_end(role_current* self) {
        printf("[恶魔契约者] 契约代价...");

        if (self->current_blood > 1) {
            self->current_blood -= 1;

            self->base_attack += 1;
            self->attack += 1;

            printf("  -> 损失1点生命，攻击力永久+1");
            printf("     当前状态：攻:%d 血:%d/%d",
                   self->attack, self->current_blood, self->max_blood);
        } else if (self->current_blood == 1) {
            self->base_attack += 1;
            self->attack += 1;

            printf("  -> 生命值过低，免于扣血，攻击力永久+1");
            printf("     当前攻击力：%d", self->attack);
        } else {
            printf("  -> 已死亡，契约结束");
        }
    }
}



namespace enemy_skills {
using namespace role;
/*
    // 空技能（占位）
    void no_skill(enemy* self) {}
    void no_skill(enemy* self, role_current* target) {}
    void no_skill(enemy* self, int damage) {}
    */
// 哥布林小子 技能：受伤时激怒（攻击力+1）
void goblin_hurt(enemy* self, int damage) {
    if (damage > 0) {
        self->current_attack += 1;
        printf("[哥布林小子] 受伤激怒！攻击力+1");
    }
}

// 哥布林法师技能：攻击时概率施加易伤
void goblin_attack(enemy* self, role_current* target) {
    if (random::a_to_b(1, 100) <= 30) { // 30%概率
        target->debuff_count += 1;
        printf("[哥布林法师] 对[%s]施加了魔法易伤",
               role::all_role_base[target->name_number].name);
    }
}

void gnome_archer_attack(enemy* self, role_current* target) {
    if (random::a_to_b(1, 100) <= 25) {
        printf("[哥布林射手] 快速射击！额外攻击一次");

        // 额外攻击造成的伤害需要手动计算
        if (target != nullptr) {
            int extra_damage = self->current_attack;
            int original_blood = target->current_blood;

            // 检查护盾
            if (target->shield > 0) {
                int shield_used = (extra_damage < target->shield) ? extra_damage : target->shield;
                target->shield -= shield_used;
                extra_damage -= shield_used;
                printf("  护盾抵挡了%d点额外伤害", shield_used);
            }

            if (extra_damage > 0) {
                target->current_blood -= extra_damage;
                if (target->current_blood < 0) target->current_blood = 0;

                printf("  额外造成%d点伤害（总伤害：%d）",
                       extra_damage, original_blood - target->current_blood);
            }
        }
    }
}


void goblin_death_reproduce (enemy* self) {
    printf("[哥布林皇后] 死亡时释放繁殖本能！");

    // 创建迷你哥布林
    enemy* mini_goblin = new enemy;

    // 设置迷你哥布林属性
    //strcpy(mini_goblin->name, "迷你哥布林");
    mini_goblin->name = 4;  // 假设迷你哥布林使用编号99
    mini_goblin->level = 0;
    mini_goblin->blood = 2;  // 2血
    mini_goblin->base_attack = 1;  // 1攻
    mini_goblin->current_attack = 1;
    mini_goblin->attack_debuff = 0;
    mini_goblin->vulnerable = 0;

    // 找到哥布林在链表中的位置
    if (role::root == self) {
        // 哥布林是第一个
        mini_goblin->next = role::root->next;
        role::root->next=mini_goblin;

    } else {
        enemy* current = role::root;
        while (current != nullptr && current->next != self) {
            current = current->next;
        }

        if (current != nullptr) {
            // 哥布林在链表中
            current->next = mini_goblin;
            mini_goblin->next = self->next;

        }
    }
}

void goblin_king_death(enemy* self) {
    printf("[哥布林之王] 陨落！哥布林们陷入混乱...");

    // 所有其他哥布林攻击力翻倍，但血量-1
    enemy* current = role::root;
    int affected_count = 0;

    while (current != nullptr) {
        if (current->name >= 4 && current != self && current->blood > 0) {
            // 攻击力翻倍
            current->current_attack *= 2;

            // 血量减1（1点不减）
            if(current->blood>=2)
                current->blood -=1 ;

            printf("  [%s] 狂暴化！攻%d 血%d",
                   current->name, current->current_attack, current->blood);
        }
        current = current->next;
    }

}



void goblin_shieldbearer_hurt(enemy* self, int damage) {
    printf("[哥布林盾卫] 用盾牌格挡！");

    // 减伤50%
    int actual_damage = damage / 2;
    if (actual_damage < 1 && damage > 0) {
        actual_damage = 1;  // 至少造成1点伤害
    }

    if (actual_damage < damage) {
        printf("  格挡了%d点伤害（%d → %d）",
               damage - actual_damage, damage, actual_damage);
    }

    self->blood -= actual_damage;
}



void goblin_berserker_hurt(enemy* self, int damage) {
    printf("[哥布林狂战士] 受伤后更加狂暴！");

    // 每受到伤害，永久增加1点攻击力
    if (damage > 0) {
        self->current_attack += 1;
        printf("  攻击力永久+1（当前：%d）", self->current_attack);
    }
}

// 空技能（占位）
void no_skill(enemy* self) {}
void no_skill(enemy* self, role_current* target) {}
void no_skill(enemy* self, int damage) {}
}


namespace role {
const struct role_base all_role_base[all_role_number]={
    // name,name_number,level,cost,blood,attack,camp[2]
    {
        "  ",0,1,2,2,2,{0,1},
        skill_store1::opening_damage,      // 战斗开始时：造成1点伤害
        NULL, // 攻击时：无技能
        NULL,            // 受到伤害时：无技能
        NULL,            // 死亡时：无技能
        NULL
    } ,
    {
        "  ",1,1,2,3,1,{0,2},
        NULL,
        skill_store1::weaken_attack,
        NULL,
        NULL,
        NULL
    },
    {
        "邪术学徒",2,1,2,2,3,{2,2},
        NULL,
        skill_store1::self_damage,
        NULL,
        NULL,
        NULL
    },
    {
        "学徒牧师",3,1,2,1,1,{1,0},
        NULL,
        NULL,
        NULL,
        NULL,
        skill_store1::heal_1
    },
    {
        "暗影信徒",4,1,2,2,1, {1,2},
        NULL,
        NULL,
        NULL,
        NULL,
        skill_store1::soul_gain
    },
    {
        "弩手",5,1,2,1,2,{2,1},
        NULL,
        skill_store1::extra_debuff_damage,
        NULL,
        NULL,
        NULL
    },
    {
        "游侠",6,1,2,2,2,{2,0},
        NULL,
        skill_store1::random_second_target,
        NULL,
        NULL,
        NULL
    },
    {
        "盾卫",7,1,2,4,1,{1,1},
        NULL,
        NULL,
        skill_store1::damage_reduction,
        NULL,
        NULL
    },
    {
        "预言学徒",8,1,2,1,2,{2,1},
        NULL,
        NULL,
        NULL,
        skill_store1::death_gain,
        NULL
    },
    {
        "治愈学徒",9,1,2,2,1,{0,0},
        skill_store1::give_shield,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "侦察兵",10,1,2,2,2,{1,1},
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "圣殿骑士", 11, 2,3,5, 3, {0, 0},  // LG
        skill_store2::knight_protect,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "荒野射手", 12, 2, 3, 3, 3, {2, 0},  // CG, cost=3
        NULL,
        skill_store2::wild_ranger_double_tap,  // 攻击时：50%概率攻击两次
        skill_store2::wild_ranger_evade,       // 受伤时：25%概率闪避
        NULL,
        NULL
    },
    {
        "拷问官", 13, 2, 3, 4, 3, {0, 2},  // cost=3
        NULL,
        skill_store2::pain_amplify,
        NULL,
        skill_store2::final_sentence,
        NULL
    },
    {
        "祝福祭司", 14, 2, 2, 2, 2, {0, 0},  // LG, cost=2
        skill_store2::blessing_priest_aoe,     // 战斗开始：前方所有友军+1攻击
        NULL,
        NULL,
        skill_store2::blessing_final_prayer,   // 死亡时：治疗随即一名友军1血
        NULL
    },
    {
        "战术家", 15, 2, 2, 4, 3, {1, 1},  // TN, cost=1
        skill_store2::tactician_positioning,   // 战斗开始：与一个友军交换位置
        skill_store2::tactician_weakpoint,     // 攻击时：若目标满血，伤害+2
        NULL,
        NULL,
        NULL
    },
    {
        "灵魂收割者", 16, 2, 3, 3, 2, {1, 2},  // NE, cost=3
        skill_store2::soul_reaper_mark,        // 战斗开始：标记一个敌人
        skill_store2::soul_reaper_execute,     // 攻击时：对标记目标伤害+3
        NULL,
        NULL,
        NULL
    },
    {
        "精英训练官", 17, 2, 3, 3, 2, {0, 0},  // LG, cost=3 (改为守序善良)
        skill_store2::elite_trainer_inspire,   // 战斗开始：若前方单位为守序阵营，经验+1
        NULL,
        skill_store2::elite_trainer_protect,   // 受伤时：若后方单位为善良阵营，经验+1
        NULL,
        check_unit_upgrade     // 回合结束：经验≥2时，永久+1/+1
    },
    {
        "深渊吞噬者", 18, 2, 3, 2, 4, {1, 2},  // NE, cost=3
        NULL,
        skill_store2::abyss_eater_devour,     // 攻击时：若击杀，永久+1攻
        NULL,
        skill_store2::abyss_eater_consume,    // 友军死亡时：永久+1血（限2次）
        NULL
    },
    {
        "荒野兽王", 19, 2, 3, 2, 3, {2, 0},  // CG, cost=2
        NULL,
        skill_store2::beast_king_frenzy,      // 攻击时：50%双击
        NULL,
        skill_store2::beast_king_soul,        // 死亡时：产生一个4-1的亡魂
        skill_store2::ghost_heal_back
    },
    {
        "烈焰法师", 20, 2, 3, 2, 4, {2, 1},  // CN, cost=3
        skill_store2::flame_mage_mark,         // 战斗开始：给一个敌人挂3层易伤
        skill_store2::flame_mage_spread_vuln,  // 攻击时：若目标有易伤，给随机另一个敌人也挂易伤
        NULL,
        skill_store2::flame_mage_final_burn,   // 死亡时：所有有易伤的敌人受到2点伤害
        NULL
    },
    {
        "召唤教官", 21, 2, 3, 2, 2, {1, 1},  // TN中立，费用3，4血2攻（生存型）
        skill_store2::summon_instructor_start,
        NULL,
        NULL,
        NULL,
        skill_store2::summon_instructor_buff
    },
    {
        "骷髅战士", 22, 0, 0, 2, 1, {1, 1},  // TN中立，费用3，4血2攻（生存型）
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },
    {
        "痛苦传导者", 23, 2, 3, 4, 2, {1, 2},  // 中立邪恶
        NULL,
        NULL,                 // 攻击时：无
        skill_store2::pain_conduction,          // 受伤时：使后方友军受1伤害
        skill_store2::pain_conduction_death,    // 死亡时：所有友军攻击+1
        NULL                  // 回合结束：无
    },
    {
        "痛苦尖啸者", 24, 2, 2, 5, 1, {1, 1},  // 混乱中立，费用2，3血1攻
         NULL,                 // 战斗开始：无
        NULL,
        skill_store2::pain_scream,         // 受伤时：随机敌人3伤害
        NULL,                 // 死亡时：无
        NULL                  // 回合结束：无
    },
    {
        "狂野炸弹兵", 25, 3, 3, 2, 4, {1, 1},                       // 混乱邪恶
        NULL,                                     // 战斗开始
        NULL,                                   // 攻击时
        NULL,                                        // 受伤时
        skill_store3::revenging3,                                  // 死亡时
        NULL,
    },
    {
        "战斗天使",26,3,3,5,1,{0,0},
        NULL,                                     // 战斗开始
        NULL,                                  // 攻击时
        NULL,                                        // 受伤时
        skill_store3::reviving,                                  // 死亡时
        skill_store3::heal_1,
   },
   {
        "刺客",27,3,3,3,3,{2,1},
        skill_store3::opening_damage3,                                     // 战斗开始
        skill_store3::assassin_critical_strike,                                  // 攻击时
        skill_store3::assassin_dodge,                                        // 受伤时
        NULL,                                  // 死亡时
        NULL,
    },
    {
        "龙裔战士", 28, 3, 4, 5, 3, {2, 0},  // 混乱邪恶，费用4，5血3攻
        NULL,               // 战斗开始：无
        skill_store3::dragon_warrior_attack,  // 攻击时：溅射伤害
        skill_store3::dragon_warrior_hurt,    // 受伤时：永久+1攻击
        NULL,
        NULL                // 回合结束：无
    },
    {
        "圣光骑士", 29, 3, 4, 5, 2, {0, 0},  // 守序善良，费用4，6血2攻
        skill_store3::holy_knight_start,     // 战斗开始：善良阵营+1护盾
        skill_store3::holy_knight_attack,    // 攻击时：治疗善良阵营最低血友军
        skill_store3::holy_knight_hurt,      // 受伤时：减伤2点
        NULL,
        NULL               // 回合结束：无
    },
    {
        "暗影巫师", 30, 3, 3, 3, 3, {2, 2},  // 混乱邪恶，费用3，3血2攻
        skill_store3::shadow_mage_start,      // 战斗开始：随机两个敌人诅咒
        skill_store3::shadow_mage_attack,     // 攻击时：对诅咒目标伤害+2
        skill_store3::shadow_mage_hurt,       // 受伤时：诅咒攻击者
        skill_store3::shadow_mage_death,      // 死亡时：引爆所有诅咒
        NULL
    },
    {
        "恶魔契约者", 31, 5, 5, 2, 8, {2, 2},
        skill_store5::demon_contractor_start,
        skill_store5::demon_contractor_attack,
        skill_store5::demon_contractor_hurt,
        skill_store5::demon_contractor_death,
        skill_store5::demon_contractor_turn_end
    },
    {
        "小恶魔", 32, 0, 0, 4, 3, {2, 2},  // 召唤物，3血3攻
        NULL,             // 战斗开始：无
        NULL,             // 攻击时：无
        NULL,             // 受伤时：无
        NULL,             // 死亡时：无
        NULL              // 回合结束：无
    }
};


    //namespace role
const struct enemy_base all_enemy_base[all_enemy_number] = {
    // name,name_number,blood,attack,current_attack,attack_debuff, skill_on_attack,skill_on_hurt,skill_on_start,skill_on_death,skill_on_turn_end
    {
        "哥布林小子", 0, 3, 2, 2, 0,
        NULL,              // skill_on_attack
        enemy_skills::goblin_hurt,    // skill_on_hurt
        NULL,              // skill_on_start
        NULL,              // skill_on_death
        NULL               // skill_on_turn_end
    },
    {
        "哥布林法师", 1, 4, 4, 4, 0,
        enemy_skills::goblin_attack,     // skill_on_attack
        NULL,              // skill_on_hurt
        NULL,              // skill_on_start
        NULL,      // skill_on_death
        NULL               // skill_on_turn_end
    },

    {
        "哥布林射手", 2, 3, 5, 5, 0,
        enemy_skills::gnome_archer_attack,   // skill_on_attack
        NULL,              // skill_on_hurt
        NULL,              // skill_on_start
        NULL,              // skill_on_death
        NULL               // skill_on_turn_end
    },


    {
        "哥布林皇后", 3, 5, 5, 5, 0,
        enemy_skills::goblin_attack,     // 攻击时：普通攻击
        NULL,          // 受伤时：无
        NULL,          // 战斗开始：无
        enemy_skills::goblin_death_reproduce, // 死亡时：繁殖迷你哥布林
        NULL           // 回合结束：无
    },

    {
        "迷你哥布林", 4, 2, 1, 1, 0,  // 2血1攻
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    },

      {
        "哥布林之王", 5, 6, 6, 6, 0,
        NULL,
        NULL,
        NULL,
        enemy_skills::goblin_king_death,   //哥布林们陷入混乱
        NULL
    },
    {
        "哥布林盾卫", 6, 6, 1, 1, 0,
        NULL,
        enemy_skills::goblin_shieldbearer_hurt, // 受伤时：减伤50%
        NULL,
        NULL,
        NULL
    },
    {
        "哥布林狂战士", 7, 4, 4, 4, 0,
        NULL,
        enemy_skills::goblin_berserker_hurt,   // 受伤时：永久加攻击
        NULL,
        NULL,
        NULL
    },

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
// 对敌人造成伤害（通用）
void apply_damage_to_enemy(enemy* target, int damage) {
    if (target == nullptr || damage <= 0) return;

    // 检查易伤buff
    if (target->vulnerable > 0) {
        damage += target->vulnerable;
        //printf("%d",target->vulnerable);
        target->vulnerable--;
        printf("  [易伤] 额外造成%d点伤害",target->vulnerable);
    }

    int original_blood = target->blood;
    target->blood -= damage;

    printf("  总共造成%d点伤害", damage);
    if (original_blood > 0 && target->blood <= 0) {
        printf("（击杀！）");
    }
    printf("");

    if (target->blood <= 0) {
        role::enemy_change::die(target);
    }
}
void role::enemy_change::die(enemy* p)
{
    if (root == nullptr || p == nullptr) return;

    // 触发死亡技能
    int enemy_index = p->name;
    if (enemy_index >= 0 && enemy_index < 4) {
        const enemy_base& base = all_enemy_base[enemy_index];
        if (base.skill_on_death != NULL) {
            base.skill_on_death(p);
        }
    }

    // 链表删除逻辑
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
/*
struct role::enemy{
    int name;
    int level;
    int blood;
    int base_attack;
    int current_attack;
    int attack_debuff;
    int vulnerable;
    struct role::enemy *next;
};*/

int a_add(int n,enemy* p){
    enemy* new_enemy=(enemy*)malloc(sizeof(enemy));
    if(new_enemy==NULL){
        my_log("wrong");
        return 0;
    }
    new_enemy->name=n;
    new_enemy->blood=all_enemy_base[n].blood;
    new_enemy->base_attack=all_enemy_base[n].attack;
    new_enemy->attack_debuff=0;
    new_enemy->current_attack=all_enemy_base[n].attack;
    new_enemy->next=NULL;
    new_enemy->vulnerable=0;
    p->next=new_enemy;
    //p=new_enemy;
    return 1;
}
int b_add(int n){
    if (root != nullptr) {
        // 清理现有链表
        enemy
            * current = root;
        while (current != nullptr) {
            enemy
                * next = current->next;
            delete current;
            current
                = next;
        }
    }
    root=(enemy*)malloc(sizeof(enemy));
    if(root==NULL){
        my_log("wrong");
        return 0;
    }
    root->name=n;
    root->blood=all_enemy_base[n].blood;
    root->base_attack=all_enemy_base[n].attack;
    root->attack_debuff=0;
    root->current_attack=all_enemy_base[n].attack;
    root->vulnerable=0;
    root->next = nullptr;
    return 1;
}

#define MAX(a,b) ((a)<(b) ? (b):(a))
void role::enemy_change::add(int num,int turn) //生成一个链表的敌人,个数为num
{
    //my_log("turn %d",MAX(800-20*turn,100));
    int l,m,r;
    l=r=num/3;
    m=num-l-r;
    int g=-1;
    if(num>=6){
//        g=random::a_to_b(1,m-1);
    }
    int k=-1;
    if(num>=9){
        k=random::a_to_b(1,m);
    }
    enemy *p=NULL;
    if(l>0){
        int ran=random::a_to_b(1,MAX(800-20*turn,100));
        if(ran<=30){
            if(!b_add(0)){
                return;
            }
        }else if(ran<=60){
            if(!b_add(6)){
                return;
            }
        }else if(ran<=80){
            if(!b_add(7)){
                return;
            }
        }else{
            if(!b_add(4)){
                return;
            }
        }
        p=root;
        for(int i=0;i<l-1;i++){
            //30-0;30-6;20-7
            int ran=random::a_to_b(1,MAX(800-20*turn,100));
            if(ran<=30){
                if(!a_add(0,p)){
                    return;
                }
            }else if(ran<=60){
                if(!a_add(6,p)){
                    return;
                }
            }else if(ran<=80){
                if(!a_add(7,p)){
                    return;
                }
            }else{
                if(!a_add(4,p)){
                    return;
                }
            }
            p=p->next;
        }
    }else{
        int ran=random::a_to_b(1,MAX(900-25*turn,100));
        if(ran<=30){
            if(!b_add(0)){
                return;
            }
        }else if(ran<=60){
            if(!b_add(7)){
                return;
            }
        }else if(ran<=70){
            if(!b_add(4)){
                return;
            }
        }else if(ran<=80){
            if(!b_add(2)){
                return;
            }
        }else if(ran<=90){
            if(!b_add(6)){
                return;
            }
        }else{
            if(!b_add(4)){
                return;
            }
        }
        p=root;
        m--;
    }
    for(int i=0;i<m;i++){
        if(i==g){
            if(!a_add(3,p)){
                return;
            }
            p=p->next;
            continue;
        }else if(i==k){
            if(!a_add(5,p)){
                return;
            }
            p=p->next;
            continue;
        }
        int ran=random::a_to_b(1,MAX(900-25*turn,100));
        if(ran<=30){
            if(!a_add(0,p)){
                return;
            }
        }else if(ran<=60){
            if(!a_add(7,p)){
                return;
            }
        }else if(ran<=70){
            if(!a_add(4,p)){
                return;
            }
        }else if(ran<=80){
            if(!a_add(2,p)){
                return;
            }
        }else if(ran<=90){
            if(!a_add(6,p)){
                return;
            }
        }else{
            if(!a_add(4,p)){
                return;
            }
        }
        p=p->next;
    }
    for(int i=0;i<r;i++){
        //20-0;30-1;30-2;10-7
        int ran=random::a_to_b(1,MAX(900-25*turn,100));
        if(ran<=20){
            if(!a_add(0,p)){
                return;
            }
        }else if(ran<=50){
            if(!a_add(4,p)){
                return;
            }
        }else if(ran<=80){
            if(!a_add(2,p)){
                return;
            }
        }else if(ran<=90){
            if(!a_add(7,p)){
                return;
            }
        }else{
            if(!a_add(4,p)){
                return;
            }
        }
        p=p->next;
    }

}
int turn(){
    //int result=-1;
    for(int i=0;i<role::left_team.num;i++){
        if(role::left_team.each[i].current_blood>0){
            //printf("a:%d %d",i);
            return i;
        }
    }
    return -1;
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
        int first=(turn());
        if (left_team.num <= 0 || first==-1) return all;
        if (root == nullptr || root->blood <= 0) return all;

        role_current* left_attacker = &left_team.each[first];
        enemy* right_defender = root;
        //确定攻击对象

        if (all_role_base[left_attacker->name_number].skill_on_attack != NULL) {
            all_role_base[left_attacker->name_number].skill_on_attack(left_attacker, right_defender);
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
    } else { // 敌方攻击
        if (root == nullptr || root->blood <= 0) return all;
        int first=turn();
        if (left_team.num <= 0 || first==-1) return all;
        //printf("%d",first);
        enemy* right_attacker = root;
        role_current* left_defender = &(left_team.each[first]);
        int fla=0;
        // 触发敌人的攻击前技能（如果有）
        int enemy_index = right_attacker->name;
        if (enemy_index >= 0 && enemy_index < 4) {
            const enemy_base& base = all_enemy_base[enemy_index];
            if (base.skill_on_attack != NULL) {
                base.skill_on_attack(right_attacker, left_defender);
                //printf("%d",right_attacker->current_attack);
                fla=1;
            }
        }

        int damage = right_attacker->current_attack;
        if (damage < 1) damage = 1;

        // 触发友军的受伤技能（如果有）
        const role_base& defender_base = all_role_base[left_defender->name_number];
        if (defender_base.skill_on_hurt != NULL) {
            defender_base.skill_on_hurt(left_defender, damage);
        } else if (fla==0){
            // 如果没有受伤技能，直接计算伤害
            int original_blood = left_defender->current_blood;
            // 处理护盾
            if (left_defender->shield > 0) {
                int shield_used = (damage < left_defender->shield) ? damage : left_defender->shield;
                left_defender->shield -= shield_used;
                damage -= shield_used;
                printf("  [%s] 的护盾抵挡了%d点伤害",
                       role::all_role_base[left_defender->name_number].name,
                       shield_used);
            }
            else{
            left_defender->current_blood -= damage;
                if (left_defender->current_blood <=0)
                {
                left_defender->current_blood=0;
                    left_defender->first_blood=1;
                }
            //printf("h");
            }

        }

        all.left[0][0] = 1;
        all.left[0][1] = -damage;

        // 检查是否死亡
        if (left_defender->current_blood <= 0 ) {
            // 触发死亡技能
            if (defender_base.skill_on_death != NULL) {
                defender_base.skill_on_death(left_defender);
            }
        }
    }
    return all;
}


void fight::battle_start_skills() {
    // 友军技能
    for (int i = 0; i < left_team.num; i++) {
        const role_base& base = all_role_base[left_team.each[i].name_number];
        if (base.skill_on_start != NULL) {
            base.skill_on_start(&left_team.each[i]);
        }
    }

    // 敌军技能
    enemy* current = root;
    while (current != nullptr) {
        int enemy_index = current->name;
        if (enemy_index >= 0 && enemy_index < 4) {
            const enemy_base& base = all_enemy_base[enemy_index];
            if (base.skill_on_start != NULL) {
                base.skill_on_start(current);
            }
        }
        current = current->next;
    }
}

void fight::turn_end_skills() {
    // 友军技能
    for (int i = 0; i < left_team.num; i++) {
        const role_base& base = all_role_base[left_team.each[i].name_number];
        if (base.skill_on_turn_end != NULL) {
            base.skill_on_turn_end(&left_team.each[i]);
        }
    }

    // 敌军技能
    enemy* current = root;
    while (current != nullptr) {
        int enemy_index = current->name;
        if (enemy_index >= 0 && enemy_index < 4) {
            const enemy_base& base = all_enemy_base[enemy_index];
            if (base.skill_on_turn_end != NULL) {
                base.skill_on_turn_end(current);
            }
        }
        current = current->next;
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
        left_team.each[i].shield=0;
        left_team.each[i].attack_buff=0;
        left_team.each[i].debuff_count=0;
        left_team.each[i].first_blood=0;
        left_team.each[i].is_ghost=0;
    }
    for (int i = 0; i < left_team.num; i++)
    {
        left_team.each[i].current_blood = left_team.each[i].max_blood;
    }
}
