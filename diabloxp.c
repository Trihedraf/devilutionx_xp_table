#include <stdio.h>

int clvl; /* Character level */
int difficulty;
int hp_add;  /* Monster hp bonus per difficulty */
int hp_mult; /* Monster hp multipler per difficulty*/
int exp_add; /* Exp bonus per difficulty */

const int exp_only = 1;
const int exp_per_hp = 0;

char bg_color[] = "000000";
char text_color[] = "FFFFFF";
char even_color[] = "2D2D2D";
char odd_color[] = "3D3D3D";

void print_html_header(void);
void print_clvl(int clvl);
void print_dlvl(int clvl, int difficulty);
float monster(int mlvl, int base_exp, float hp, int xp_only);
int dungeon(short dlvl, int xp_only);

void main(void) {
    extern int clvl, difficulty;

    print_html_header();

    for (clvl = 1; clvl <= 50; clvl++) {
        print_clvl(clvl);
        for (difficulty = 1; difficulty <= 4; difficulty = 2 * difficulty)
            print_dlvl(clvl, difficulty);
    }

    printf("      </table>\n\n    </center>\n  </body>\n</html>");
}

void print_html_header(void) {
    printf("<html>\n"
           "  <head>\n"
           "    <title>Experience Point Table</title>\n"
           "  </head>\n"
           "  <body bgcolor=#%s text=#%s>\n"
           "    <center>\n\n",
           bg_color, text_color);

    printf("      <table border=1 bgcolor=#%s>\n"
           "        <tr>\n"
           "          <th rowspan=3>clvl</th>\n"
           "          <th rowspan=3>Diff</th>\n"
           "          <th colspan=19>Dungeon level</th>\n"
           "        </tr>\n"
           "        <tr>\n"
           "          <th colspan=4>Church</th>\n"
           "          <th bgcolor=#%s />\n"
           "          <th colspan=4>Catacombs</th>\n"
           "          <th bgcolor=#%s />\n"
           "          <th colspan=4>Caves</th>\n"
           "          <th bgcolor=#%s />\n"
           "          <th colspan=4>Hell</th>\n"
           "        </tr>\n"
           "        <tr>\n"
           "          <th> 1</th>\n"
           "          <th> 2</th>\n"
           "          <th> 3</th>\n"
           "          <th> 4</th>\n"
           "          <th bgcolor=#%s />\n"
           "          <th> 5</th>\n"
           "          <th> 6</th>\n"
           "          <th> 7</th>\n"
           "          <th> 8</th>\n"
           "          <th bgcolor=#%s />\n"
           "          <th> 9</th>\n"
           "          <th>10</th>\n"
           "          <th>11</th>\n"
           "          <th>12</th>\n"
           "          <th bgcolor=#%s />\n"
           "          <th>13</th>\n"
           "          <th>14</th>\n"
           "          <th>15</th>\n"
           "          <th>16</th>\n"
           "        </tr>\n",
           even_color, bg_color, bg_color, bg_color, bg_color, bg_color, bg_color);
}

void print_clvl(int clvl) {
    printf("\n        <tr align=right bgcolor=#"); /* Define a new row */

    /* Choose bgcolor accoring to clvl: */
    if (clvl % 2)
        printf("%s", odd_color);
    else
        printf("%s", even_color);
    printf(">\n          <th align=center ");

    /* How many rows for this clvl? */
    if (clvl < 20)
        printf(">");
    else if (clvl >= 30 && clvl < 40)
        printf("rowspan=3>");
    else
        printf("rowspan=2>");

    printf("%d", clvl);

    printf("</th>\n");
}

void print_dlvl(int clvl, int difficulty) {
    short dlvl;
    extern int exp_add, hp_add, hp_mult;

    /* When to NOT print: */
    if (difficulty == 1 && clvl >= 40 || /* Normal: >=40 */
        difficulty == 2 && clvl < 20 ||  /* Night:   <20 */
        difficulty == 4 && clvl < 30)    /* Hell:    <30 */
        return;

    if (difficulty == 2 && clvl < 40 || difficulty == 4) {
        printf("        <tr align=right bgcolor=#");

        if (clvl % 2)
            printf("%s", odd_color);
        else
            printf("%s", even_color);

        printf(">\n");
    }

    printf("          <th align=center>");

    switch (difficulty) {
    case 1: /* Normal */
        hp_add = 0;
        hp_mult = 1;
        exp_add = 0;
        printf("Norm");
        break;
    case 2: /* Nightmare */
        hp_add = 100;
        hp_mult = 3;
        exp_add = 2000;
        printf("Night");
        break;
    case 4: /* Hell */
        hp_add = 200;
        hp_mult = 4;
        exp_add = 4000;
        printf("Hell");
        break;
    }

    printf("</th>");

    for (dlvl = 1; dlvl <= 16; dlvl++) {
        printf("\n          <td>%5d<br />%3d</td>", dungeon(dlvl, exp_only), dungeon(dlvl, exp_per_hp));

        if (!(dlvl % 4) && dlvl % 16)                           /* Empty column every 4th dlvl, */
            printf("\n          <td bgcolor=#%s />", bg_color); /* but not dlvl 16 */
    }

    printf("\n        </tr>\n");
}

float monster(int mlvl, int basexp, float hp, int xp_only) {
    float exp;
    extern int clvl, difficulty, hp_add, hp_mult;

    if (difficulty > 1) /* Set the mlvl according to difficulty */
        mlvl = mlvl + difficulty * 7.5;
    exp = (difficulty * basexp + exp_add) * /* Calculate experience points */
          (1 + .1 * (mlvl - clvl));

    if (exp > 200 * clvl) /* Not over the 200*clvl cap */
        exp = 200 * clvl;
    else if (exp < 0) /* When exp is negative, set it to 0 */
        exp = 0;

    if (xp_only) /* Return exp only */
        return exp;
    else /* Return exp per hitpoints */
        return exp / (hp_mult * hp + hp_add);
}

/* Calculate average experience points from a dungeon level
 * Value of 'r' determines what to return:
 * 1 (exp_only): experience only
 * 0 (exp_per_hp): experience per hitpoints
 */
int dungeon(short dlvl, int r) {
    switch (dlvl) {
    case 1:                              /* Dungeon level 1: */
        return (monster(1, 54, 5.5, r) + /* Zombie */
                monster(1, 46, 2.5, r) + /* Fallen One Spear */
                monster(1, 52, 3.5, r) + /* Fallen One Sword */
                monster(1, 64, 3.0, r) + /* Skeleton */
                monster(4, 90, 4.5, r) + /* Skeleton Captain */
                monster(2, 80, 4.5, r))  /* Scavenger */
               / 6;
    case 2:                                     /* Dungeon level 2: */
        return (monster(1, 54, 5.5, r) * 18 +   /* Zombie */
                monster(2, 58, 9.0, r) * 18 +   /* Ghoul */
                monster(4, 136, 20.0, r) * 18 + /* Rotting Carcass */
                monster(1, 46, 2.5, r) * 20 +   /* Fallen One Spear */
                monster(3, 80, 6.0, r) * 20 +   /* Carver Spear */
                monster(5, 155, 18.0, r) * 21 + /* Devil Kin Spear */
                monster(1, 52, 3.5, r) * 20 +   /* Fallen One Sword */
                monster(3, 90, 7.0, r) * 20 +   /* Carver Sword */
                monster(5, 180, 20.0, r) * 20 + /* Devil Kin Sword */
                monster(1, 64, 3.0, r) * 21 +   /* Skeleton */
                monster(2, 68, 5.5, r) * 20 +   /* Corpse Axe */
                monster(4, 154, 10.0, r) * 20 + /* Burning Dead */
                monster(3, 110, 3.0, r) * 20 +  /* Skeleton Archer */
                monster(5, 210, 12.0, r) * 20 + /* Corpse Bow */
                monster(4, 90, 4.5, r) * 20 +   /* Skeleton Captain */
                monster(6, 200, 16.0, r) * 20 + /* Corpse Captain */
                monster(2, 80, 4.5, r) * 28 +   /* Scavenger */
                monster(4, 188, 18.0, r) * 28 + /* Plague Eater */
                monster(3, 102, 4.5, r) * 17 +  /* Fiend */
                monster(5, 278, 16.0, r) * 35)  /* Hidden */
               / 424;
    case 3:                                     /* Dungeon level 3: */
        return (monster(2, 58, 9.0, r) * 13 +   /* Ghoul */
                monster(4, 136, 20.0, r) * 13 + /* Rotting Carcass */
                monster(6, 240, 32.5, r) * 13 + /* Black Death */
                monster(3, 80, 6.0, r) * 15 +   /* Carver Spear */
                monster(5, 155, 18.0, r) * 15 + /* Devil Kin Spear */
                monster(7, 255, 28.0, r) * 15 + /* Dark One Spear */
                monster(3, 90, 7.0, r) * 14 +   /* Carver Sword */
                monster(5, 180, 20.0, r) * 14 + /* Devil Kin Sword */
                monster(7, 280, 30.0, r) * 14 + /* Dark One Sword */
                monster(2, 68, 5.5, r) * 24 +   /* Corpse Axe */
                monster(4, 154, 10.0, r) * 24 + /* Burning Dead */
                monster(6, 264, 16.0, r) * 24 + /* Horror */
                monster(3, 110, 3.0, r) * 24 +  /* Skeleton Archer */
                monster(5, 210, 12.0, r) * 24 + /* Corpse Bow */
                monster(7, 364, 17.0, r) * 24 + /* Burning Dead Archer */
                monster(4, 90, 4.5, r) * 24 +   /* Skeleton Captain */
                monster(6, 200, 16.0, r) * 24 + /* Corpse Captain */
                monster(8, 393, 23.0, r) * 24 + /* Burning Dead Captain */
                monster(2, 80, 4.5, r) * 20 +   /* Scavenger */
                monster(4, 188, 18.0, r) * 20 + /* Plague Eater */
                monster(6, 375, 30.0, r) * 20 + /* Shadow Beast */
                monster(3, 102, 4.5, r) * 23 +  /* Fiend */
                monster(7, 340, 20.0, r) * 23 + /* Blink */
                monster(5, 278, 16.0, r) * 11)  /* Hidden */
               / 459;
    case 4:                                      /* Dungeon level 4: */
        return (monster(4, 136, 20.0, r) * 24 +  /* Rotting Carcass */
                monster(6, 240, 32.5, r) * 24 +  /* Black Death */
                monster(5, 155, 18.0, r) * 26 +  /* Devil Kin Spear */
                monster(7, 255, 28.0, r) * 26 +  /* Dark One Spear */
                monster(5, 180, 20.0, r) * 26 +  /* Devil Kin Sword */
                monster(7, 280, 30.0, r) * 26 +  /* Dark One Sword */
                monster(4, 154, 10.0, r) * 26 +  /* Burning Dead */
                monster(6, 264, 16.0, r) * 26 +  /* Horror */
                monster(5, 210, 12.0, r) * 26 +  /* Corpse Bow */
                monster(7, 364, 17.0, r) * 26 +  /* Burning Dead Archer */
                monster(9, 594, 30.0, r) * 26 +  /* Horror Archer */
                monster(6, 200, 16.0, r) * 26 +  /* Corpse Captain */
                monster(8, 393, 23.0, r) * 26 +  /* Burning Dead Captain */
                monster(10, 604, 42.5, r) * 26 + /* Horror Captain */
                monster(4, 188, 18.0, r) * 31 +  /* Plague Eater */
                monster(6, 375, 30.0, r) * 31 +  /* Shadow Beast */
                monster(8, 552, 34.0, r) * 31 +  /* Bone Gasher */
                monster(7, 340, 20.0, r) * 34 +  /* Blink */
                monster(9, 509, 32.0, r) * 34 +  /* Gloom */
                monster(5, 278, 16.0, r) * 23 +  /* Hidden */
                monster(8, 460, 37.5, r) * 22 +  /* Flesh Clan */
                monster(8, 448, 27.5, r) * 22)   /* Flesh Clan Archer */
               / 588;
    case 5:                                      /* Dungeon level 5: */
        return (monster(6, 240, 32.5, r) * 23 +  /* Black Death */
                monster(7, 255, 28.0, r) * 26 +  /* Dark One Spear */
                monster(7, 280, 30.0, r) * 25 +  /* Dark One Sword */
                monster(6, 264, 16.0, r) * 26 +  /* Horror */
                monster(7, 364, 17.0, r) * 26 +  /* Burning Dead Archer */
                monster(9, 594, 30.0, r) * 26 +  /* Horror Archer */
                monster(8, 393, 23.0, r) * 25 +  /* Burning Dead Captain */
                monster(10, 604, 42.5, r) * 25 + /* Horror Captain */
                monster(6, 375, 30.0, r) * 32 +  /* Shadow Beast */
                monster(8, 552, 34.0, r) * 32 +  /* Bone Gasher */
                monster(7, 340, 20.0, r) * 36 +  /* Blink */
                monster(9, 509, 32.0, r) * 37 +  /* Gloom */
                monster(5, 278, 16.0, r) * 21 +  /* Hidden */
                monster(9, 630, 37.5, r) * 21 +  /* Stalker */
                monster(8, 460, 37.5, r) * 20 +  /* Flesh Clan */
                monster(10, 685, 47.5, r) * 20 + /* Stone Clan */
                monster(8, 448, 27.5, r) * 20 +  /* Flesh Clan Archer */
                monster(10, 645, 35.0, r) * 20 + /* Stone Clan Archer */
                monster(10, 635, 70.0, r) * 20 + /* Overlord */
                monster(9, 662, 52.5, r) * 16)   /* Winged Demon */
               / 497;
    case 6:                                      /* Dungeon level 6: */
        return (monster(9, 594, 30.0, r) * 31 +  /* Horror Archer */
                monster(10, 604, 42.5, r) * 31 + /* Horror Captain */
                monster(8, 552, 34.0, r) * 44 +  /* Bone Gasher */
                monster(9, 509, 32.0, r) * 47 +  /* Gloom */
                monster(13, 448, 27.5, r) * 47 + /* Familiar */
                monster(9, 630, 37.5, r) * 23 +  /* Stalker */
                monster(11, 935, 42.5, r) * 23 + /* Unseen */
                monster(8, 460, 37.5, r) * 23 +  /* Flesh Clan */
                monster(10, 685, 47.5, r) * 22 + /* Stone Clan */
                monster(12, 906, 57.5, r) * 22 + /* Fire Clan */
                monster(4, 448, 27.5, r) * 22 +  /* Flesh Clan Archer */
                monster(10, 645, 35.0, r) * 22 + /* Stone Clan Archer */
                monster(12, 822, 45.0, r) * 22 + /* Fire Clan Archer */
                monster(10, 635, 70.0, r) * 22 + /* Overlord */
                monster(9, 662, 52.5, r) * 17 +  /* Winged Demon */
                monster(11, 846, 53.0, r) * 29)  /* Acid Beast */
               / 447;
    case 7:                                        /* Dungeon level 7: */
        return (monster(13, 448, 27.5, r) * 75 +   /* Familiar */
                monster(9, 630, 37.5, r) * 20 +    /* Stalker */
                monster(11, 935, 42.5, r) * 20 +   /* Unseen */
                monster(10, 685, 47.5, r) * 20 +   /* Stone Clan */
                monster(12, 906, 57.5, r) * 20 +   /* Fire Clan */
                monster(14, 1190, 62.5, r) * 20 +  /* Night Clan */
                monster(10, 645, 35.0, r) * 20 +   /* Stone Clan Archer */
                monster(12, 822, 45.0, r) * 20 +   /* Fire Clan Archer */
                monster(14, 1092, 57.5, r) * 20 +  /* Night Clan Archer */
                monster(10, 635, 70.0, r) * 19 +   /* Overlord */
                monster(14, 1165, 112.5, r) * 20 + /* Mud Man */
                monster(9, 662, 52.5, r) * 15 +    /* Winged Demon */
                monster(13, 1205, 75.0, r) * 15 +  /* Gargoyle */
                monster(13, 1172, 60.0, r) * 15 +  /* Horned Demon */
                monster(11, 846, 53.0, r) * 34)    /* Acid Beast */
               / 353;
    case 8:                                        /* Dungeon level 8: */
        return (monster(13, 448, 27.5, r) * 49 +   /* Familiar */
                monster(11, 935, 42.5, r) * 19 +   /* Unseen */
                monster(13, 1500, 50.0, r) * 19 +  /* Illusion Weaver */
                monster(12, 906, 57.5, r) * 18 +   /* Fire Clan */
                monster(14, 1190, 62.5, r) * 18 +  /* Night Clan */
                monster(12, 822, 45.0, r) * 18 +   /* Fire Clan Archer */
                monster(14, 1092, 57.5, r) * 18 +  /* Night Clan Archer */
                monster(14, 1165, 112.5, r) * 18 + /* Mud Man */
                monster(16, 1380, 147.5, r) * 18 + /* Toad Demon */
                monster(13, 1295, 75.0, r) * 14 +  /* Gargoyle */
                monster(13, 1076, 60.0, r) * 14 +  /* Magma Demon */
                monster(14, 1309, 65.0, r) * 14 +  /* Blood Stone */
                monster(13, 1172, 60.0, r) * 14 +  /* Horned Demon */
                monster(15, 1404, 70.0, r) * 14 +  /* Mud Runner */
                monster(11, 846, 53.0, r) * 33 +   /* Acid Beast */
                monster(15, 1248, 72.5, r) * 33)   /* Poison Spitter */
               / 331;
    case 9:                                        /* Dungeon level 9: */
        return (monster(13, 1500, 50.0, r) * 16 +  /* Illusion Weaver */
                monster(14, 1190, 62.5, r) * 16 +  /* Night Clan */
                monster(14, 1092, 57.5, r) * 16 +  /* Night Clan Arc */
                monster(14, 1165, 112.5, r) * 16 + /* Mud Man */
                monster(16, 1380, 147.5, r) * 16 + /* Toad Demon */
                monster(13, 1205, 75.0, r) * 13 +  /* Gargoyle */
                monster(19, 1873, 100.0, r) * 13 + /* Blood Claw */
                monster(13, 1076, 60.0, r) * 13 +  /* Magma Demon */
                monster(14, 1309, 65.0, r) * 13 +  /* Blood Stone */
                monster(16, 1680, 70.0, r) * 13 +  /* Hell Stone */
                monster(18, 2124, 77.5, r) * 13 +  /* Lava Lord */
                monster(13, 1172, 60.0, r) * 13 +  /* Horned Demon */
                monster(15, 1404, 70.0, r) * 13 +  /* Mud Runner */
                monster(17, 1720, 80.0, r) * 13 +  /* Frost Charger */
                monster(15, 1248, 72.5, r) * 56 +  /* Poison Spitter */
                monster(18, 2160, 82.5, r) * 13)   /* Red Storm */
               / 266;
    case 10:                                       /* Dungeon level 10: */
        return (monster(13, 1500, 50.0, r) * 19 +  /* Illusion Weaver */
                monster(16, 1380, 147.5, r) * 19 + /* Toad Demon */
                monster(20, 2058, 180.0, r) * 19 + /* Flayed One */
                monster(19, 1873, 100.0, r) * 13 + /* Blood Claw */
                monster(23, 2278, 120.0, r) * 13 + /* Death Wing */
                monster(14, 1309, 65.0, r) * 13 +  /* Blood Stone */
                monster(16, 1680, 70.0, r) * 13 +  /* Hell Stone */
                monster(18, 2124, 77.5, r) * 13 +  /* Lava Lord */
                monster(15, 1404, 70.0, r) * 13 +  /* Mud Runner */
                monster(17, 1720, 80.0, r) * 13 +  /* Frost Charger */
                monster(19, 1809, 90.0, r) * 13 +  /* Obsidian Lord */
                monster(15, 1248, 72.5, r) * 31 +  /* Poison Spitter */
                monster(21, 2060, 95.0, r) * 31 +  /* Pit Beast */
                monster(18, 2160, 82.5, r) * 13 +  /* Red Storm */
                monster(20, 2391, 90.0, r) * 13 +  /* Storm Rider */
                monster(20, 2300, 130.0, r) * 8)   /* Slayer */
               / 257;
    case 11:                                       /* Dungeon level 11: */
        return (monster(20, 2058, 180.0, r) * 31 + /* Flayed One */
                monster(19, 1873, 100.0, r) * 14 + /* Blood Claw */
                monster(23, 2278, 120.0, r) * 14 + /* Death Wing */
                monster(16, 1680, 70.0, r) * 14 +  /* Hell Stone */
                monster(18, 2124, 77.5, r) * 14 +  /* Lava Lord */
                monster(17, 1720, 80.0, r) * 14 +  /* Frost Charger */
                monster(19, 1809, 90.0, r) * 14 +  /* Obsidian Lord */
                monster(21, 2060, 95.0, r) * 31 +  /* Pit Beast */
                monster(18, 2160, 82.5, r) * 14 +  /* Red Storm */
                monster(20, 2391, 90.0, r) * 14 +  /* Storm Rider */
                monster(22, 2775, 105.0, r) * 14 + /* Storm Lord */
                monster(20, 2300, 130.0, r) * 9 +  /* Slayer */
                monster(22, 2714, 150.0, r) * 9 +  /* Guardian */
                monster(21, 2725, 125.0, r) * 19)  /* Cave Viper */
               / 225;
    case 12:                                       /* Dungeon level 12: */
        return (monster(20, 2058, 180.0, r) * 23 + /* Flayed One */
                monster(23, 2278, 120.0, r) * 13 + /* Death Wing */
                monster(19, 1809, 90.0, r) * 13 +  /* Obsidian Lord */
                monster(21, 2060, 95.0, r) * 30 +  /* Pit Beast */
                monster(25, 2940, 125.0, r) * 30 + /* Lava Maw */
                monster(20, 2391, 90.0, r) * 13 +  /* Storm Rider */
                monster(22, 2775, 105.0, r) * 13 + /* Storm Lord */
                monster(24, 3177, 120.0, r) * 13 + /* Maelstorm */
                monster(20, 2300, 130.0, r) * 9 +  /* Slayer */
                monster(22, 2714, 150.0, r) * 10 + /* Guardian */
                monster(24, 3252, 170.0, r) * 10 + /* Vortex Lord */
                monster(21, 2725, 125.0, r) * 18 + /* Cave Viper */
                monster(23, 3139, 145.0, r) * 18 + /* Fire Drake */
                monster(24, 3696, 135.0, r) * 23 + /* Succubus */
                monster(24, 3360, 150.0, r) * 10)  /* Black Knight */
               / 246;
    case 13:                                       /* Dungeon level 13: */
        return (monster(25, 2940, 125.0, r) * 27 + /* Lava Maw */
                monster(22, 2775, 105.0, r) * 11 + /* Storm Lord */
                monster(24, 3177, 120.0, r) * 11 + /* Maelstorm */
                monster(22, 2714, 150.0, r) * 10 + /* Guardian */
                monster(24, 3252, 170.0, r) * 10 + /* Vortex Lord */
                monster(26, 3643, 190.0, r) * 10 + /* Balrog */
                monster(21, 2725, 125.0, r) * 21 + /* Cave Viper */
                monster(23, 3139, 145.0, r) * 21 + /* Fire Drake */
                monster(25, 3484, 160.0, r) * 21 + /* Gold viper */
                monster(24, 3696, 135.0, r) * 23 + /* Succubus */
                monster(26, 4084, 155.0, r) * 23 + /* Snow Witch */
                monster(24, 3360, 150.0, r) * 10 + /* Black Knight */
                monster(26, 3650, 165.0, r) * 10 + /* Doom Guard */
                monster(30, 5130, 200.0, r) * 10 + /* Blood Knight */
                monster(25, 3876, 70.0, r) * 10)   /* Counselor */
               / 228;
    case 14:                                       /* Dungeon level 14: */
        return (monster(25, 2940, 125.0, r) * 26 + /* Lava Maw */
                monster(24, 3177, 120.0, r) * 10 + /* Maelstorm */
                monster(24, 3252, 170.0, r) * 10 + /* Vortex Lord */
                monster(26, 3643, 190.0, r) * 9 +  /* Balrog */
                monster(23, 3139, 145.0, r) * 21 + /* Fire Drake */
                monster(25, 3484, 160.0, r) * 21 + /* Gold Viper */
                monster(24, 3696, 135.0, r) * 22 + /* Succubus */
                monster(26, 4084, 155.0, r) * 22 + /* Snow Witch */
                monster(28, 4480, 175.0, r) * 22 + /* Hell Spawn */
                monster(24, 3360, 150.0, r) * 10 + /* Black Knight */
                monster(26, 3650, 165.0, r) * 10 + /* Doom Guard */
                monster(28, 4252, 180.0, r) * 10 + /* Steel Lord */
                monster(30, 5130, 200.0, r) * 10 + /* Blood Knight */
                monster(25, 3876, 70.0, r) * 10 +  /* Counselor */
                monster(27, 4478, 85.0, r) * 10)   /* Magistrate */
               / 223;
    case 15:                                       /* Dungeon level 15: */
        return (monster(26, 3643, 190.0, r) * 17 + /* Balrog */
                monster(27, 3791, 180.0, r) * 33 + /* Azure Drake */
                monster(26, 4084, 155.0, r) * 33 + /* Snow Witch */
                monster(28, 4480, 175.0, r) * 34 + /* Hell Spawn */
                monster(30, 4644, 182.5, r) * 33 + /* Soul Burner */
                monster(26, 3650, 165.0, r) * 17 + /* Doom Guard */
                monster(28, 4252, 180.0, r) * 17 + /* Steel Lord */
                monster(27, 4478, 85.0, r) * 17 +  /* Magistrate */
                monster(29, 4929, 100.0, r) * 17)  /* Cabalist */
               / 218;
    case 16:                                  /* Dungeon level 16: */
        return (monster(30, 5130, 200.0, r) + /* Blood Knight */
                monster(30, 4968, 145.0, r))  /* Advocate */
               / 2;
    }
}
