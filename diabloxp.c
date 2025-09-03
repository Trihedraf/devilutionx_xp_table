/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
* Last change: 17/Mar/1999									*
*												*
* diabloxp.c:	Calculate the average exp per hitpoints from each dungeon level for each	*
*		character level from 1 to 50, and print it out as an HTML-table			*
*		Also works as a passive CGI-program (uncomment line 76)				*
*												*
* This program is pretty simple C-code, which could be great for someone learning C		*
* 												*
* Your display should be set for at least 100 characters per row in order to view		*
* this properly. If you use a graphical text-editor (ironic heh?), make sure you're using	*
* monospace fonts										*
*	if (this_looks_fine)									*
*		nevernmind;									*
*												*
* Sorry about that, but I like large display when coding					*
*												*
* When run, it will print out all the data to the screen (if not used as CGI)			*
* Since you might want it as a file, you must use redirection:					*
*	diabloxp.exe > diabloxp.htm	(DOS)							*
*	diabloxp > diabloxp.html	(UNIX)							*
*												*
* I hope you understand my code, but I'm currently a C-newbie too :)				*
*												*
* Regards,											*
* Willy Nyland, aka Soul Toucher								*
*												*
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>					/* Only the standard libary, that means	*/
								/* no fancy stuff, hehe		*/
									
#define YES		1				/* Yeap, oui, ja			*/
#define NO		0				/* Nope, non, nei			*/
#define EXP_ONLY	1				/* \ Used to tell functions what to do;	*/
#define EXP_HP		0				/* / return only exp, or exp per hp	*/


/* Global variables:
 */
int clvl,						/* Character level			*/
    skill,						/* Skill				*/
    hp_mult,						/* Monster hp multipler   <--\		*/
    exp_add;						/* Exp bonus   <--(nightmare and hell)	*/


/* Functions:
 */
void file_headers(void);				/* Print out file-headers		*/
void clvl_data(int clvl);				/* Print clvl data			*/
void dlvl_data(int clvl,int skill);			/* Print dlvl data			*/
float mstr(int mlvl,int base_exp,float hp,int xp_only);	/* Calculate exp from monster		*/
int dungeon(short dlvl,int xp_only);			/* Calculate avg. exp from a dungeon	*/


/* The program:
   No command-line arguments
   Return nothing (pretty simple, heh?)
 */
void main(void)
{
	extern int clvl,				/* Get those global variables		*/
		   skill;

	file_headers();					/* Print file headers			*/
	for (clvl=1; clvl<=50; clvl++) {		/* Loop from clvl 1 to 50		*/
		clvl_data(clvl);			/* Print clvl data			*/
		for (skill=1; skill<=4; skill=2*skill) 	/* Loop skill 1, 2 and 4		*/
			dlvl_data(clvl,skill);		/* Print dlvl data 			*/
	}
	printf("\n</table>\n</body>\n</html>");		/* Print table- and HTML-terminator	*/
}							/* Close the program			*/


/* Print the headers of the file
 */
void file_headers(void)
{
							/* Print HTML header				*/
	printf(/* "Content-type: text/html\n\n" */
	       "<html>\n"
	       "<head>\n"
	       "<center>\n"
	       "<title>Experience Point Table</title>\n"
	       "</head>\n"
	       "<body\n"
	       "bgcolor\t= #FFFFFF\n"
	       "text\t= #000000\n"
	       "link\t= #0000FF\n"
	       "vlink\t= #000080\n"
	       "alink\t= #FF0000\n>\n\n");

							/* Print Table header				*/
	printf("<table border=1 bgcolor=#9090FF>\n\n"
	       "<th rowspan = 3>clvl\n"
	       "<th rowspan = 3>Skill\n"
	       "<th colspan =19>Dungeon level\n"
	       "<tr>\n"
	       "<th colspan = 4>Church\t <th bgcolor=#FFFFFF>\n"
	       "<th colspan = 4>Catacombs<th bgcolor=#FFFFFF>\n"
	       "<th colspan = 4>Caves\t <th bgcolor=#FFFFFF>\n"
	       "<th colspan = 4>Hell\n"
	       "<tr>\n"
	       "<th> 1<th> 2<th> 3<th> 4<th bgcolor=#FFFFFF>\n"
	       "<th> 5<th> 6<th> 7<th> 8<th bgcolor=#FFFFFF>\n"
	       "<th> 9<th>10<th>11<th>12<th bgcolor=#FFFFFF>\n"
	       "<th>13<th>14<th>15<th>16\n"
	       "<tr>\n");
}


/* Print the clvl data
 */
void clvl_data(int clvl)
{
	printf("\n<tr align=right bgcolor=#");		/* Define a new row 			*/
	if (clvl%2)					/* Choose bgcolor accoring to clvl:	*/
		printf("B0B0FF");				/* Odd color			*/
	else
		printf("9090FF");				/* Even	color			*/
	printf("><th ");
							/* How many rows for this clvl? 	*/
	if (clvl<20)						/* 01-19:			*/
		printf("align=center>");			/* One row			*/

	else if (clvl>=30 && clvl<40)				/* 30-39:			*/
		printf("rowspan=3>");				/* Three rows			*/

	else							/* 20-29, 40-50:	 	*/
		printf("rowspan=2>");				/* Two rows			*/

	printf("%d\n", clvl);				/* Print the character level		*/
}


/* Print the dungeon data
 */
void dlvl_data(int clvl, int skill)
{
	short dlvl;
	extern exp_add, hp_mult;
							/* When to NOT print:			*/
	if (skill==1 && clvl>=40||				/* Normal: 40+			*/
	    skill==2 && clvl<20 ||				/* Night:  20-			*/
	    skill==4 && clvl<30)				/* Hell:   30-			*/
		return;
							/* Define a new row when:		*/
	if (skill==2 && clvl<40 ||				/* Night, up to clvl 40		*/
	    skill==4) {						/* Hell, always			*/
		printf("<tr align=right bgcolor=#");
		if (clvl%2)				/* Choose bgcolor for this row:		*/
			printf("B0B0FF");			/* Odd color			*/
		else
			printf("9090FF");			/* Even	color			*/
		printf(">\n");
	}
	printf("<th align=left>");
							/* What skill is this?			*/
	switch (skill) {
		case 1:					/* 1 = Normal				*/
			hp_mult = 1;				/* Monster hp: 1x normal	*/
			exp_add = 0;				/* No exp-bonus			*/
			printf("Norm\n");			/* Print out skill		*/
			break;
		case 2:					/* 2 = Nightmare			*/
			hp_mult = 3;				/* Monster hp: 3x normal	*/
			exp_add = 2000;				/* Exp: normal + 2000		*/
			printf("Night\n");			/* Print out skill		*/
			break;
		case 4:					/* 4 = Hell				*/
			hp_mult = 4;				/* Monster hp: 4x normal	*/
			exp_add = 4000;				/* Exp: normal + 4000		*/
			printf("Hell\n");			/* Print out skill		*/
			break;
	}
	for (dlvl=1; dlvl<=16; dlvl++) {		/* Write table data			*/
		printf("<td>\t\t\t%5d<br>\t%3d",dungeon(dlvl,EXP_ONLY), dungeon(dlvl,EXP_HP));
		if (!(dlvl%4) && dlvl%16)			/* Empty column every 4th dlvl,	*/
			printf("\n<td bgcolor=#FFFFFF>");		/* but not dlvl 16	*/
		if (!(dlvl%2))
			printf("\n");				/* New line every 2nd dlvl	*/
	}
}


/* Calculate experience point from a monster
 */
float mstr(int mlvl, int basexp, float hp, int xp_only)
{
	float exp;
	extern int clvl, skill, hp_mult;

	if (skill > 1)					/* Set the mlvl according to skill	*/
		mlvl = mlvl + skill * 7.5;
	exp = (skill * basexp + exp_add) *		/* Calculate experience points		*/
	      (1 + .1 * (mlvl - clvl));
	if (exp > 200*clvl)				/* Not over the 200*clvl cap		*/
		exp = 200 * clvl;
	else if (exp < 0)				/* When exp is negative, set it to 0	*/
		exp = 0;
	if (xp_only)					/* Return exp only			*/
		return exp;
	else						/* Return exp per hitpoints		*/
		return exp / (hp_mult * hp + skill - 1.0);
}


/* Calculate average experience points from a dungeon level
 * Value of 'r' determines what to return:
 *	1 (EXP_ONLY):	experience only
 *	0 (EXP_HP):	experience per hitpoints
 */
int dungeon(short dlvl, int r)
{
	switch (dlvl) {					/* Find out what level to calculate	*/
	
	case 1:						/* Dungeon level 1:			*/
		return (mstr( 1,  54,  5.5,r)+			/* Zombie			*/
			mstr( 1,  46,  2.5,r)+			/* Fallen One Spear		*/
			mstr( 1,  52,  3.5,r)+			/* Fallen One Sword		*/
			mstr( 1,  64,  3.0,r)+			/* Skeleton			*/
			mstr( 4,  90,  4.5,r)+			/* Skeleton Captain		*/
			mstr( 2,  80,  4.5,r))			/* Scavenger			*/
			/ 6;
	case 2:						/* Dungeon level 2:			*/
		return (mstr( 1,  54,  5.5,r) *18+		/* Zombie			*/
			mstr( 2,  58,  9.0,r) *18+		/* Ghoul			*/
			mstr( 4, 136, 20.0,r) *18+		/* Rotting Carcass		*/
			mstr( 1,  46,  2.5,r) *20+		/* Fallen One Spear		*/
			mstr( 3,  80,  6.0,r) *20+		/* Carver Spear			*/
			mstr( 5, 155, 18.0,r) *21+		/* Devil Kin Spear		*/
			mstr( 1,  52,  3.5,r) *20+		/* Fallen One Sword		*/
			mstr( 3,  90,  7.0,r) *20+		/* Carver Sword			*/
			mstr( 5, 180, 20.0,r) *20+		/* Devil Kin Sword		*/
			mstr( 1,  64,  3.0,r) *21+		/* Skeleton			*/
			mstr( 2,  68,  5.5,r) *20+		/* Corpse Axe			*/
			mstr( 4, 154, 10.0,r) *20+		/* Burning Dead			*/
			mstr( 3, 110,  3.0,r) *20+		/* Skeleton Archer		*/
			mstr( 5, 210, 12.0,r) *20+		/* Corpse Bow			*/
			mstr( 4,  90,  4.5,r) *20+		/* Skeleton Captain		*/
			mstr( 6, 200, 16.0,r) *20+		/* Corpse Captain		*/
			mstr( 2,  80,  4.5,r) *28+		/* Scavenger			*/
			mstr( 4, 188, 18.0,r) *28+		/* Plague Eater			*/
			mstr( 3, 102,  4.5,r) *17+		/* Fiend			*/
			mstr( 5, 278, 16.0,r) *35)		/* Hidden			*/
			/ 424;
	case 3:						/* Dungeon level 3:			*/
		return (mstr( 2,  58,  9.0,r) *13+		/* Ghoul			*/
			mstr( 4, 136, 20.0,r) *13+		/* Rotting Carcass		*/
			mstr( 6, 240, 32.5,r) *13+		/* Black Death			*/
			mstr( 3,  80,  6.0,r) *15+		/* Carver Spear			*/
			mstr( 5, 155, 18.0,r) *15+		/* Devil Kin Spear		*/
			mstr( 7, 255, 28.0,r) *15+		/* Dark One Spear		*/
			mstr( 3,  90,  7.0,r) *14+		/* Carver Sword			*/
			mstr( 5, 180, 20.0,r) *14+		/* Devil Kin Sword		*/
			mstr( 7, 280, 30.0,r) *14+		/* Dark One Sword		*/
			mstr( 2,  68,  5.5,r) *24+		/* Corpse Axe			*/
			mstr( 4, 154, 10.0,r) *24+		/* Burning Dead			*/
			mstr( 6, 264, 16.0,r) *24+		/* Horror			*/
			mstr( 3, 110,  3.0,r) *24+		/* Skeleton Archer		*/
			mstr( 5, 210, 12.0,r) *24+		/* Corpse Bow			*/
			mstr( 7, 364, 17.0,r) *24+		/* Burning Dead Archer		*/
			mstr( 4,  90,  4.5,r) *24+		/* Skeleton Captain		*/
			mstr( 6, 200, 16.0,r) *24+		/* Corpse Captain		*/
			mstr( 8, 393, 23.0,r) *24+		/* Burning Dead Captain		*/
			mstr( 2,  80,  4.5,r) *20+		/* Scavenger			*/
			mstr( 4, 188, 18.0,r) *20+		/* Plague Eater			*/
			mstr( 6, 375, 30.0,r) *20+		/* Shadow Beast			*/
			mstr( 3, 102,  4.5,r) *23+		/* Fiend			*/
			mstr( 7, 340, 20.0,r) *23+		/* Blink			*/
			mstr( 5, 278, 16.0,r) *11)		/* Hidden			*/
			/ 459;
	case 4:						/* Dungeon level 4:			*/
		return (mstr( 4, 136, 20.0,r) *24+		/* Rotting Carcass		*/
			mstr( 6, 240, 32.5,r) *24+		/* Black Death			*/
			mstr( 5, 155, 18.0,r) *26+		/* Devil Kin Spear		*/
			mstr( 7, 255, 28.0,r) *26+		/* Dark One Spear		*/
			mstr( 5, 180, 20.0,r) *26+		/* Devil Kin Sword		*/
			mstr( 7, 280, 30.0,r) *26+		/* Dark One Sword		*/
			mstr( 4, 154, 10.0,r) *26+		/* Burning Dead			*/
			mstr( 6, 264, 16.0,r) *26+		/* Horror			*/
			mstr( 5, 210, 12.0,r) *26+		/* Corpse Bow			*/
			mstr( 7, 364, 17.0,r) *26+		/* Burning Dead Archer		*/
			mstr( 9, 594, 30.0,r) *26+		/* Horror Archer		*/
			mstr( 6, 200, 16.0,r) *26+		/* Corpse Captain		*/
			mstr( 8, 393, 23.0,r) *26+		/* Burning Dead Captain		*/
			mstr(10, 604, 42.5,r) *26+		/* Horror Captain		*/
			mstr( 4, 188, 18.0,r) *31+		/* Plague Eater			*/
			mstr( 6, 375, 30.0,r) *31+		/* Shadow Beast			*/
			mstr( 8, 552, 34.0,r) *31+		/* Bone Gasher			*/
			mstr( 7, 340, 20.0,r) *34+		/* Blink			*/
			mstr( 9, 509, 32.0,r) *34+		/* Gloom			*/
			mstr( 5, 278, 16.0,r) *23+		/* Hidden			*/
			mstr( 8, 460, 37.5,r) *22+		/* Flesh Clan			*/
			mstr( 8, 448, 27.5,r) *22)		/* Flesh Clan Archer		*/
			/ 588;
	case 5:						/* Dungeon level 5:			*/
		return (mstr( 6, 240, 32.5,r) *23+		/* Black Death			*/
			mstr( 7, 255, 28.0,r) *26+		/* Dark One Spear		*/
			mstr( 7, 280, 30.0,r) *25+		/* Dark One Sword		*/
			mstr( 6, 264, 16.0,r) *26+		/* Horror			*/
			mstr( 7, 364, 17.0,r) *26+		/* Burning Dead Archer		*/
			mstr( 9, 594, 30.0,r) *26+		/* Horror Archer		*/
			mstr( 8, 393, 23.0,r) *25+		/* Burning Dead Captain		*/
			mstr(10, 604, 42.5,r) *25+		/* Horror Captain		*/
			mstr( 6, 375, 30.0,r) *32+		/* Shadow Beast			*/
			mstr( 8, 552, 34.0,r) *32+		/* Bone Gasher			*/
			mstr( 7, 340, 20.0,r) *36+		/* Blink			*/
			mstr( 9, 509, 32.0,r) *37+		/* Gloom			*/
			mstr( 5, 278, 16.0,r) *21+		/* Hidden			*/
			mstr( 9, 630, 37.5,r) *21+		/* Stalker			*/
			mstr( 8, 460, 37.5,r) *20+		/* Flesh Clan			*/
			mstr(10, 685, 47.5,r) *20+		/* Stone Clan			*/
			mstr( 8, 448, 27.5,r) *20+		/* Flesh Clan Archer		*/
			mstr(10, 645, 35.0,r) *20+		/* Stone Clan Archer		*/
			mstr(10, 635, 70.0,r) *20+		/* Overlord			*/
			mstr( 9, 662, 52.5,r) *16)		/* Winged Demon			*/
			/ 497;
	case 6:						/* Dungeon level 6:			*/
		return (mstr( 9, 594, 30.0,r) *31+		/* Horror Archer		*/
			mstr(10, 604, 42.5,r) *31+		/* Horror Captain		*/
			mstr( 8, 552, 34.0,r) *44+		/* Bone Gasher			*/
			mstr( 9, 509, 32.0,r) *47+		/* Gloom			*/
			mstr(13, 448, 27.5,r) *47+		/* Familiar			*/
			mstr( 9, 630, 37.5,r) *23+		/* Stalker			*/
			mstr(11, 935, 42.5,r) *23+		/* Unseen			*/
			mstr( 8, 460, 37.5,r) *23+		/* Flesh Clan			*/
			mstr(10, 685, 47.5,r) *22+		/* Stone Clan			*/
			mstr(12, 906, 57.5,r) *22+		/* Fire Clan			*/
			mstr( 4, 448, 27.5,r) *22+		/* Flesh Clan Archer		*/
			mstr(10, 645, 35.0,r) *22+		/* Stone Clan Archer		*/
			mstr(12, 822, 45.0,r) *22+		/* Fire Clan Archer		*/
			mstr(10, 635, 70.0,r) *22+		/* Overlord			*/
			mstr( 9, 662, 52.5,r) *17+		/* Winged Demon			*/
			mstr(11, 846, 53.0,r) *29)		/* Acid Beast			*/
			/ 447;
	case 7:						/* Dungeon level 7:			*/
		return (mstr(13, 448, 27.5,r) *75+		/* Familiar			*/
			mstr( 9, 630, 37.5,r) *20+		/* Stalker			*/
			mstr(11, 935, 42.5,r) *20+		/* Unseen			*/
			mstr(10, 685, 47.5,r) *20+		/* Stone Clan			*/
			mstr(12, 906, 57.5,r) *20+		/* Fire Clan			*/
			mstr(14,1190, 62.5,r) *20+		/* Night Clan			*/
			mstr(10, 645, 35.0,r) *20+		/* Stone Clan Archer		*/
			mstr(12, 822, 45.0,r) *20+		/* Fire Clan Archer		*/
			mstr(14,1092, 57.5,r) *20+		/* Night Clan Archer		*/
			mstr(10, 635, 70.0,r) *19+		/* Overlord			*/
			mstr(14,1165,112.5,r) *20+		/* Mud Man			*/
			mstr( 9, 662, 52.5,r) *15+		/* Winged Demon			*/
			mstr(13,1205, 75.0,r) *15+		/* Gargoyle			*/
			mstr(13,1172, 60.0,r) *15+		/* Horned Demon			*/
			mstr(11, 846, 53.0,r) *34)		/* Acid Beast			*/
			/ 353;
	case 8:						/* Dungeon level 8:			*/
		return (mstr(13, 448, 27.5,r) *49+		/* Familiar			*/
			mstr(11, 935, 42.5,r) *19+		/* Unseen			*/
			mstr(13,1500, 50.0,r) *19+		/* Illusion Weaver		*/
			mstr(12, 906, 57.5,r) *18+		/* Fire Clan			*/
			mstr(14,1190, 62.5,r) *18+		/* Night Clan			*/
			mstr(12, 822, 45.0,r) *18+		/* Fire Clan Archer		*/
			mstr(14,1092, 57.5,r) *18+		/* Night Clan Archer		*/
			mstr(14,1165,112.5,r) *18+		/* Mud Man			*/
			mstr(16,1380,147.5,r) *18+		/* Toad Demon			*/
			mstr(13,1295, 75.0,r) *14+		/* Gargoyle			*/
			mstr(13,1076, 60.0,r) *14+		/* Magma Demon			*/
			mstr(14,1309, 65.0,r) *14+		/* Blood Stone			*/
			mstr(13,1172, 60.0,r) *14+		/* Horned Demon			*/
			mstr(15,1404, 70.0,r) *14+		/* Mud Runner			*/
			mstr(11, 846, 53.0,r) *33+		/* Acid Beast			*/
			mstr(15,1248, 72.5,r) *33)		/* Poison Spitter		*/
			/ 331;
	case 9:						/* Dungeon level 9:			*/
		return (mstr(13,1500, 50.0,r) *16+		/* Illusion Weaver		*/
			mstr(14,1190, 62.5,r) *16+		/* Night Clan			*/
			mstr(14,1092, 57.5,r) *16+		/* Night Clan Arc		*/
			mstr(14,1165,112.5,r) *16+		/* Mud Man			*/
			mstr(16,1380,147.5,r) *16+		/* Toad Demon			*/
			mstr(13,1205, 75.0,r) *13+		/* Gargoyle			*/
			mstr(19,1873,100.0,r) *13+		/* Blood Claw			*/
			mstr(13,1076, 60.0,r) *13+		/* Magma Demon			*/
			mstr(14,1309, 65.0,r) *13+		/* Blood Stone			*/
			mstr(16,1680, 70.0,r) *13+		/* Hell Stone			*/
			mstr(18,2124, 77.5,r) *13+		/* Lava Lord			*/
			mstr(13,1172, 60.0,r) *13+		/* Horned Demon			*/
			mstr(15,1404, 70.0,r) *13+		/* Mud Runner			*/
			mstr(17,1720, 80.0,r) *13+		/* Frost Charger		*/
			mstr(15,1248, 72.5,r) *56+		/* Poison Spitter		*/
			mstr(18,2160, 82.5,r) *13)		/* Red Storm			*/
			/ 266;
	case 10:					/* Dungeon level 10:			*/
		return (mstr(13,1500, 50.0,r) *19+		/* Illusion Weaver		*/
			mstr(16,1380,147.5,r) *19+		/* Toad Demon			*/
			mstr(20,2058,180.0,r) *19+		/* Flayed One			*/
			mstr(19,1873,100.0,r) *13+		/* Blood Claw			*/
			mstr(23,2278,120.0,r) *13+		/* Death Wing			*/
			mstr(14,1309, 65.0,r) *13+		/* Blood Stone			*/
			mstr(16,1680, 70.0,r) *13+		/* Hell Stone			*/
			mstr(18,2124, 77.5,r) *13+		/* Lava Lord			*/
			mstr(15,1404, 70.0,r) *13+		/* Mud Runner			*/
			mstr(17,1720, 80.0,r) *13+		/* Frost Charger		*/
			mstr(19,1809, 90.0,r) *13+		/* Obsidian Lord		*/
			mstr(15,1248, 72.5,r) *31+		/* Poison Spitter		*/
			mstr(21,2060, 95.0,r) *31+		/* Pit Beast			*/
			mstr(18,2160, 82.5,r) *13+		/* Red Storm			*/
			mstr(20,2391, 90.0,r) *13+		/* Storm Rider			*/
			mstr(20,2300,130.0,r) * 8)		/* Slayer			*/
			/ 257;
	case 11:					/* Dungeon level 11:			*/
		return (mstr(20,2058,180.0,r) *31+		/* Flayed One			*/
			mstr(19,1873,100.0,r) *14+		/* Blood Claw			*/
			mstr(23,2278,120.0,r) *14+		/* Death Wing			*/
			mstr(16,1680, 70.0,r) *14+		/* Hell Stone			*/
			mstr(18,2124, 77.5,r) *14+		/* Lava Lord			*/
			mstr(17,1720, 80.0,r) *14+		/* Frost Charger		*/
			mstr(19,1809, 90.0,r) *14+		/* Obsidian Lord		*/
			mstr(21,2060, 95.0,r) *31+		/* Pit Beast			*/
			mstr(18,2160, 82.5,r) *14+		/* Red Storm			*/
			mstr(20,2391, 90.0,r) *14+		/* Storm Rider			*/
			mstr(22,2775,105.0,r) *14+		/* Storm Lord			*/
			mstr(20,2300,130.0,r) * 9+		/* Slayer			*/
			mstr(22,2714,150.0,r) * 9+		/* Guardian			*/
			mstr(21,2725,125.0,r) *19)		/* Cave Viper			*/
			/ 225;
	case 12:					/* Dungeon level 12:			*/
		return (mstr(20,2058,180.0,r) *23+		/* Flayed One			*/
			mstr(23,2278,120.0,r) *13+		/* Death Wing			*/
			mstr(19,1809, 90.0,r) *13+		/* Obsidian Lord		*/
			mstr(21,2060, 95.0,r) *30+		/* Pit Beast			*/
			mstr(25,2940,125.0,r) *30+		/* Lava Maw			*/
			mstr(20,2391, 90.0,r) *13+		/* Storm Rider			*/
			mstr(22,2775,105.0,r) *13+		/* Storm Lord			*/
			mstr(24,3177,120.0,r) *13+		/* Maelstorm			*/
			mstr(20,2300,130.0,r) * 9+		/* Slayer			*/
			mstr(22,2714,150.0,r) *10+		/* Guardian			*/
			mstr(24,3252,170.0,r) *10+		/* Vortex Lord			*/
			mstr(21,2725,125.0,r) *18+		/* Cave Viper			*/
			mstr(23,3139,145.0,r) *18+		/* Fire Drake			*/
			mstr(24,3696,135.0,r) *23+		/* Succubus			*/
			mstr(24,3360,150.0,r) *10)		/* Black Knight			*/
			/ 246;
	case 13:					/* Dungeon level 13:			*/
		return (mstr(25,2940,125.0,r) *27+		/* Lava Maw			*/
			mstr(22,2775,105.0,r) *11+		/* Storm Lord			*/
			mstr(24,3177,120.0,r) *11+		/* Maelstorm			*/
			mstr(22,2714,150.0,r) *10+		/* Guardian			*/
			mstr(24,3252,170.0,r) *10+		/* Vortex Lord			*/
			mstr(26,3643,190.0,r) *10+		/* Balrog			*/
			mstr(21,2725,125.0,r) *21+		/* Cave Viper			*/
			mstr(23,3139,145.0,r) *21+		/* Fire Drake			*/
			mstr(25,3484,160.0,r) *21+		/* Gold viper			*/
			mstr(24,3696,135.0,r) *23+		/* Succubus			*/
			mstr(26,4084,155.0,r) *23+		/* Snow Witch			*/
			mstr(24,3360,150.0,r) *10+		/* Black Knight			*/
			mstr(26,3650,165.0,r) *10+		/* Doom Guard			*/
			mstr(30,5130,200.0,r) *10+		/* Blood Knight			*/
			mstr(25,3876, 70.0,r) *10)		/* Counselor			*/
			/ 228;
	case 14:					/* Dungeon level 14:			*/
		return (mstr(25,2940,125.0,r) *26+		/* Lava Maw			*/
			mstr(24,3177,120.0,r) *10+		/* Maelstorm			*/
			mstr(24,3252,170.0,r) *10+		/* Vortex Lord			*/
			mstr(26,3643,190.0,r) * 9+		/* Balrog			*/
			mstr(23,3139,145.0,r) *21+		/* Fire Drake			*/
			mstr(25,3484,160.0,r) *21+		/* Gold Viper			*/
			mstr(24,3696,135.0,r) *22+		/* Succubus			*/
			mstr(26,4084,155.0,r) *22+		/* Snow Witch			*/
			mstr(28,4480,175.0,r) *22+		/* Hell Spawn			*/
			mstr(24,3360,150.0,r) *10+		/* Black Knight			*/
			mstr(26,3650,165.0,r) *10+		/* Doom Guard			*/
			mstr(28,4252,180.0,r) *10+		/* Steel Lord			*/
			mstr(30,5130,200.0,r) *10+		/* Blood Knight			*/
			mstr(25,3876, 70.0,r) *10+		/* Counselor			*/
			mstr(27,4478, 85.0,r) *10)		/* Magistrate			*/
			/ 223;
	case 15:					/* Dungeon level 15:			*/
		return (mstr(26,3643,190.0,r) *17+		/* Balrog			*/
			mstr(27,3791,180.0,r) *33+		/* Azure Drake			*/
			mstr(26,4084,155.0,r) *33+		/* Snow Witch			*/
			mstr(28,4480,175.0,r) *34+		/* Hell Spawn			*/
			mstr(30,4644,182.5,r) *33+		/* Soul Burner			*/
			mstr(26,3650,165.0,r) *17+		/* Doom Guard			*/
			mstr(28,4252,180.0,r) *17+		/* Steel Lord			*/
			mstr(27,4478, 85.0,r) *17+		/* Magistrate			*/
			mstr(29,4929,100.0,r) *17)		/* Cabalist			*/
			/ 218;
	case 16:					/* Dungeon level 16:			*/	
		return (mstr(30,5130,200.0,r)+			/* Blood Knight			*/
			mstr(30,4968,145.0,r))			/* Advocate			*/
			/ 2;
	}
}
