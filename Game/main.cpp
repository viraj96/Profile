#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include "quickcg.h"
using namespace QuickCG;
using namespace std;
#define width 1256
#define width_clouds 1255
#define maxw_grass 1255
#define width_grass 400
#define height 1256
#define height_clouds 300
#define maxh_grass 1255
#define height_grass 955
#define factor_1 0.293193717 // 280/955
#define factor_2 0.314136126 // 300/955
double clouds_Noise[width][height];
double grass_Noise1[maxw_grass][maxh_grass];
double grass_Noise2[maxw_grass][maxh_grass];
Uint8 clouds_Values[width_clouds][height_clouds];
Uint8 grass_Values1[maxw_grass][maxh_grass];
Uint8 grass_Values2[maxw_grass][maxh_grass];
class player;
class car;
class obstacle;
time_t timer1, timer2;
struct gamer{
	std::string name;
	double finaltime;
};
void grassNoise()
{
	for(int y=300; y<=maxh_grass; y++)
	{
		int threshold = abs(((-1)*(y-maxh_grass)*(factor_1)) + 120);
		for(int x=0; x<threshold; x++)
		{
			grass_Noise1[x][y] = (rand()%32768)/32768.0;
		}
		threshold = abs(((y-maxh_grass)*(factor_2)) + 1100);
		for(int x=threshold; x<=1255; x++)
		{
			grass_Noise2[x][y] = (rand()%32768)/32768.0;
		}	
	}
}
void cloudsNoise()
{
	for(int x=0; x<width_clouds; x++)
	{
		for(int y=0; y<height_clouds; y++)
		{
			clouds_Noise[x][y] = (rand()%32768)/32768.0;
		}
	}
}
double grassSmoothNoise(double x, double y, bool left, bool right)
{
	double fracx = x - int(x);
	double fracy = y - int(y);
	int y1 = (int(y) + height_grass)%height_grass;
	int y2 = (y1 + height_grass - 1)%height_grass;
	double value = 0.0;
	if(left == false && right == true)
	{
		int x1 = 1000 + (int(x) + width_grass)%width_grass;
		int x2 = 1000 + (x1 + width_grass - 1)%width_grass;
		value += (fracx*fracy*grass_Noise2[x1][y1]);
		value += (fracx*(1-fracy)*grass_Noise2[x2][y1]);
		value += ((1-fracx)*fracy*grass_Noise2[x1][y2]);
		value += ((1-fracx)*(1-fracy)*grass_Noise2[x2][y2]);
		return value;
	}
	else if(left == true && right == false)
	{
		int x1 = (int(x) + width_grass)%width_grass;
		int x2 = (x1 + width_grass - 1)%width_grass;
		value += (fracx*fracy*grass_Noise1[x1][y1]);
		value += (fracx*(1-fracy)*grass_Noise1[x2][y1]);
		value += ((1-fracx)*fracy*grass_Noise1[x1][y2]);
		value += ((1-fracx)*(1-fracy)*grass_Noise1[x2][y2]);
		return value;
	}
}
double cloudsSmoothNoise(double x, double y)
{
	double fracx = x - int(x);
	double fracy = y - int(y);
	int x1 = (int(x) + width_clouds)%width_clouds;
	int y1 = (int(y) + height_clouds)%height_clouds;
	int x2 = (x1 + width_clouds - 1)%width_clouds;
	int y2 = (y1 + height_clouds - 1)%height_clouds;
	double value = 0.0;
	value += (fracx*fracy*clouds_Noise[x1][y1]);
	value += (fracx*(1-fracy)*clouds_Noise[x1][y2]);
	value += ((1-fracx)*fracy*clouds_Noise[x2][y1]);
	value += ((1-fracx)*(1-fracy)*clouds_Noise[x2][y2]);
	return value;
}
double grassTurbulence(double x, double y, double size, bool left, bool right)
{
	double value = 0.0, initialSize = size;
	while(size >= 1)
	{
		if(left == false && right == true)
		{
			value += grassSmoothNoise(x/size,y/size,left,right)*size;
		}
		else if(left == true && right == false)
		{
			value += grassSmoothNoise(x/size,y/size,left,right)*size;
		}
		value = value * 100;
		size /= 2.0;
	}
	return (128.0*value/initialSize);
}
double cloudsTurbulence(double x, double y, double size)
{
	double value = 0.0, initialSize = size;
	while(size >= 1)
	{
		value += cloudsSmoothNoise(x/size,y/size)*size;
		size /= 2.0;
	}
	return (128.0*value/initialSize);
}
void clouds()
{
	cloudsNoise();
	Uint8 val;
	for(int x=0; x<width_clouds; x++)
	{
		for(int y=0; y<height_clouds; y++)
		{
			val = 192 + Uint8(cloudsTurbulence(x,y,64))/4;
			clouds_Values[x][y] = val;
		}
	}
}
void grass()
{
	grassNoise();
	Uint8 value1, value2;
	for(int y=300; y<=maxh_grass; y++)
	{
		int threshold = abs(((-1)*(y-maxh_grass)*(factor_1)) + 120);
		for(int x=0; x<threshold; x++)
		{
			value1 = 100 + Uint8(grassTurbulence(x,y,64,true,false))/4;
			grass_Values1[x][y] = value1;
		}
		threshold = abs(((y-maxh_grass)*(factor_2)) + 1100);
		for(int x=threshold; x<=1255; x++)
		{
			value2 = 100 + Uint8(grassTurbulence(x,y,64,false,true))/4;
			grass_Values2[x][y] = value2;
		}
	}
}
void makeClouds()
{
	ColorRGB color;
	for(int x=0; x<width_clouds; x++)
	{
		for(int y=0; y<height_clouds; y++)
		{
			color = HSLtoRGB(ColorHSL(169,255,clouds_Values[x][y]));
			pset(x,y,color);
		}
	}
}
void makeGrass()
{
	ColorRGB color;
	for(int y=300; y<=maxh_grass; y++)
	{
		int threshold = abs(((-1)*(y-maxh_grass)*(factor_1)) + 120);
		for(int x=0; x<threshold; x++)
		{
			color = HSLtoRGB(ColorHSL(90,255,grass_Values1[x][y]));
			pset(x,y,color);
		}
		threshold = abs(((y-maxh_grass)*(factor_2)) + 1100);
		for(int x=threshold; x<=1255; x++)
		{
			color = HSLtoRGB(ColorHSL(90,255,grass_Values2[x][y]));
			pset(x,y,color);
		}
	}
}
void design()
{
	//horizon
	drawLine(0, 300, 1255, 300, ColorRGB(255, 0, 0));
	drawLine(0, 301, 1255, 301, ColorRGB(255, 0, 0));
	drawLine(0, 302, 1255, 302, ColorRGB(255, 0, 0));
	//first building
	drawRect(0,60,100,299,ColorRGB(51,0,0));
	verLine(0,60,299, ColorRGB(0, 0, 0));
	verLine(1,60,299, ColorRGB(0, 0, 0));
	verLine(2,60,299, ColorRGB(0, 0, 0));
	verLine(3,60,299, ColorRGB(0, 0, 0));
	verLine(4,60,299, ColorRGB(0, 0, 0));
	horLine(60,0,100, ColorRGB(0 ,0, 0));
	horLine(61,0,100, ColorRGB(0 ,0, 0));
	horLine(62,0,100, ColorRGB(0 ,0, 0));
	horLine(63,0,100, ColorRGB(0 ,0, 0));
	horLine(64,0,100, ColorRGB(0 ,0, 0));
	//second building
	drawRect(100,30,250,299,ColorRGB(0,153,0));
	verLine(100,30,299, ColorRGB(0, 0, 0));
	verLine(101,30,299, ColorRGB(0, 0, 0));
	verLine(102,30,299, ColorRGB(0, 0, 0));
	verLine(103,30,299, ColorRGB(0, 0, 0));
	verLine(104,30,299, ColorRGB(0, 0, 0));
	horLine(30,100,250, ColorRGB(0 ,0, 0));
	horLine(31,100,250, ColorRGB(0 ,0, 0));
	horLine(32,100,250, ColorRGB(0 ,0, 0));
	horLine(33,100,250, ColorRGB(0 ,0, 0));
	horLine(34,100,250, ColorRGB(0 ,0, 0));
	verLine(250,30,299, ColorRGB(0, 0, 0));
	verLine(249,30,299, ColorRGB(0, 0, 0));
	verLine(248,30,299, ColorRGB(0, 0, 0));
	verLine(247,30,299, ColorRGB(0, 0, 0));
	verLine(246,30,299, ColorRGB(0, 0, 0));
	//third building
	drawRect(250,100,300,299,ColorRGB(0,128,255));
	horLine(100,250,300, ColorRGB(0 ,0, 0));
	horLine(101,250,300, ColorRGB(0 ,0, 0));
	horLine(102,250,300, ColorRGB(0 ,0, 0));
	horLine(103,250,300, ColorRGB(0 ,0, 0));
	horLine(104,250,300, ColorRGB(0 ,0, 0));
	//fourth building
	drawRect(300,85,400,299,ColorRGB(128,255,0));
	verLine(300,85,299, ColorRGB(0, 0, 0));
	verLine(301,85,299, ColorRGB(0, 0, 0));
	verLine(302,85,299, ColorRGB(0, 0, 0));
	verLine(303,85,299, ColorRGB(0, 0, 0));
	verLine(304,85,299, ColorRGB(0, 0, 0));
	horLine(85,300,400, ColorRGB(0 ,0, 0));
	horLine(86,300,400, ColorRGB(0 ,0, 0));
	horLine(87,300,400, ColorRGB(0 ,0, 0));
	horLine(88,300,400, ColorRGB(0 ,0, 0));
	horLine(89,300,400, ColorRGB(0 ,0, 0));
	verLine(400,85,299, ColorRGB(0, 0, 0));
	verLine(399,85,299, ColorRGB(0, 0, 0));
	verLine(398,85,299, ColorRGB(0, 0, 0));
	verLine(397,85,299, ColorRGB(0, 0, 0));
	verLine(396,85,299, ColorRGB(0, 0, 0));
	//fifth building
	drawRect(800,100,850,299,ColorRGB(102,51,0));
	verLine(800,100,299, ColorRGB(0, 0, 0));
	verLine(801,100,299, ColorRGB(0, 0, 0));
	verLine(802,100,299, ColorRGB(0, 0, 0));
	verLine(803,100,299, ColorRGB(0, 0, 0));
	verLine(804,100,299, ColorRGB(0, 0, 0));
	horLine(100,800,850, ColorRGB(0 ,0, 0));
	horLine(101,800,850, ColorRGB(0 ,0, 0));
	horLine(102,800,850, ColorRGB(0 ,0, 0));
	horLine(103,800,850, ColorRGB(0 ,0, 0));
	horLine(104,800,850, ColorRGB(0 ,0, 0));
	//sixth building
	drawRect(850,80,950,299,ColorRGB(204,102,0));
	verLine(850,80,299, ColorRGB(0, 0, 0));
	verLine(851,80,299, ColorRGB(0, 0, 0));
	verLine(852,80,299, ColorRGB(0, 0, 0));
	verLine(853,80,299, ColorRGB(0, 0, 0));
	verLine(854,80,299, ColorRGB(0, 0, 0));
	horLine(80,850,950, ColorRGB(0 ,0, 0));
	horLine(81,850,950, ColorRGB(0 ,0, 0));
	horLine(82,850,950, ColorRGB(0 ,0, 0));
	horLine(83,850,950, ColorRGB(0 ,0, 0));
	horLine(84,850,950, ColorRGB(0 ,0, 0));
	//seventh building
	drawRect(950,20,1000,299,ColorRGB(102,102,0));
	verLine(950,20,299, ColorRGB(0, 0, 0));
	verLine(951,20,299, ColorRGB(0, 0, 0));
	verLine(952,20,299, ColorRGB(0, 0, 0));
	verLine(953,20,299, ColorRGB(0, 0, 0));
	verLine(954,20,299, ColorRGB(0, 0, 0));
	horLine(20,950,1000, ColorRGB(0 ,0, 0));
	horLine(21,950,1000, ColorRGB(0 ,0, 0));
	horLine(22,950,1000, ColorRGB(0 ,0, 0));
	horLine(23,950,1000, ColorRGB(0 ,0, 0));
	horLine(24,950,1000, ColorRGB(0 ,0, 0));
	verLine(1000,20,299, ColorRGB(0, 0, 0));
	verLine(999,20,299, ColorRGB(0, 0, 0));
	verLine(998,20,299, ColorRGB(0, 0, 0));
	verLine(997,20,299, ColorRGB(0, 0, 0));
	verLine(996,20,299, ColorRGB(0, 0, 0));
	//eigth building  
	drawRect(1000,120,1150,299,ColorRGB(128,128,128));
	horLine(120,1000,1150, ColorRGB(0 ,0, 0));
	horLine(121,1000,1150, ColorRGB(0 ,0, 0));
	horLine(122,1000,1150, ColorRGB(0 ,0, 0));
	horLine(123,1000,1150, ColorRGB(0 ,0, 0));
	horLine(124,1000,1150, ColorRGB(0 ,0, 0));
	//ninth building
	drawRect(1150,60,1255,299,ColorRGB(153,153,0));
	verLine(1150,60,299, ColorRGB(0, 0, 0));
	verLine(1151,60,299, ColorRGB(0, 0, 0));
	verLine(1152,60,299, ColorRGB(0, 0, 0));
	verLine(1153,60,299, ColorRGB(0, 0, 0));
	verLine(1154,60,299, ColorRGB(0, 0, 0));
	horLine(60,1150,1255, ColorRGB(0 ,0, 0));
	horLine(61,1150,1255, ColorRGB(0 ,0, 0));
	horLine(62,1150,1255, ColorRGB(0 ,0, 0));
	horLine(63,1150,1255, ColorRGB(0 ,0, 0));
	horLine(64,1150,1255, ColorRGB(0 ,0, 0));
	verLine(1255,60,299, ColorRGB(0, 0, 0));
	verLine(1254,60,299, ColorRGB(0, 0, 0));
	verLine(1253,60,299, ColorRGB(0, 0, 0));
	verLine(1252,60,299, ColorRGB(0, 0, 0));
	verLine(1251,60,299, ColorRGB(0, 0, 0));
	//left road boundary
	drawLine(400,303,120,1255,ColorRGB(0,255,0));
	drawLine(399,303,119,1255,ColorRGB(0,255,0));
	drawLine(401,303,121,1255,ColorRGB(0,255,0));
	drawLine(398,303,121,1255,ColorRGB(0,255,0));
	//right road boundary
	drawLine(800,303,1100,1255,ColorRGB(0,255,0));
	drawLine(799,303,1099,1255,ColorRGB(0,255,0));
	drawLine(801,303,1101,1255,ColorRGB(0,255,0));
}
void startscreen()
{
	int position;
	position = print("1. NEW GAME",500,300,RGB_Yellow);
	position = print("2. HOW TO PLAY",500,320,RGB_Yellow);
	position = print("3. LEADER BOARD",500,340,RGB_Yellow);
	position = print("4. EXIT",500,360,RGB_Yellow);
	redraw();
}
void sort()
{
	int i = 0,j = 0,k = 0;
	std::string word;
	struct gamer gamers[100];
	std::ifstream leaderboard;
	leaderboard.open("High_Scores.txt");
	if(leaderboard.is_open())
	{
		while(leaderboard >> word)
		{
			if(i%2==0)
			{
				gamers[j].name = word;
			}
			else
			{
				gamers[j].finaltime = atof(word.c_str());
				j += 1;
			}
			i += 1;
		}
		for(i=0; i<j; i++)
		{
			k = i;
			while(k>0 && gamers[k].finaltime > gamers[k-1].finaltime)
			{
				struct gamer temp = gamers[k];
				gamers[k] = gamers[k-1];
				gamers[k-1] = temp;
				k--;
			}
		}
		std::ofstream new_leaderboard;
		new_leaderboard.open("High_Scores.txt");
		if(new_leaderboard.is_open())
		{
			for(i=0; i<j; i++)
			{
				new_leaderboard << gamers[i].name << " " << gamers[i].finaltime << "\n";
			}
			new_leaderboard.close();
		}
		else
		{
			cout << "Unable to open new file\n"; 
		}
		leaderboard.close();
	}
	else
	{
		cout << "Unable to open new file\n"; 
	}
}
class player
{
	char name[20];
	float finaltime,td,timedelay;
	public:
		player();
		void ftimedelay();
		float ftd();
		void ffinaltime(double);
};
player::player()
{
	td = 0;
	timedelay = 0;
}
void player::ftimedelay()
{
	timedelay += 10;
}
float player::ftd()
{
	td = sqrt(timedelay);
	return td;
}
void player::ffinaltime(double seconds)
{
	cls();
	finaltime = seconds;
	int position;
	position = print("Now... That was tough !!",500,280,RGB_Yellow);
	position = print("Your score was = ",500,300,RGB_Red);
	position = print(finaltime,650,300,RGB_Red);
	position = print("Press Esc key to go back to menu",470,600,RGB_Yellow);
	redraw();
	std::string gamer,line;
	std::ofstream leaderboard;
	getInputString(gamer,"Enter your name :- ",false,500,400);
	leaderboard.open("High_Scores.txt",ios::app);
	if(leaderboard.is_open())
	{
		leaderboard << gamer << " " << finaltime << "\n";
		leaderboard.close();
		sort();
		cout << "Successfully entered the score.\n";
	}
	else
	{
		cout << "Unable to open the file\n";
	}
}
class car
{
	int cx,cy;
	friend int collision(car,obstacle);
	public:
		car();
		void constructcar();
		void movecar(char);
};
car::car()
{
	cx = 260;
	cy = 620;
}
void car::constructcar()
{
	drawLine(cx+420,620,cx+520,620,ColorRGB(0,255,0));
	drawLine(cx+420,660,cx+520,660,ColorRGB(0,255,0));
	drawLine(cx+420,620,cx+420,660,ColorRGB(0,255,0));
	drawLine(cx+520,620,cx+520,660,ColorRGB(0,255,0));
	drawCircle(cx+420,660,10,RGB_Yellow);
	drawCircle(cx+520,660,10,RGB_Yellow);
}
void  car::movecar(char move)
{
	if(move == 'd' || move == 'D')
	{
		if(cx == 0)
		{
			cx += 260;
		}
	}
	if(move == 'a' || move == 'A')
	{
		if(cx == 260)
		{
			cx -= 260;
		}
	}
}
class obstacle
{
	int obx,oby;
	public:
		obstacle();
		friend int collision(car,obstacle);
		void incrementob(int);
		void drawobstacle();
};
obstacle::obstacle()
{
	obx = 20;
	oby = 1;
}
void obstacle::drawobstacle()
{
	if(oby >= 350)
	{
		oby = oby%350;
		int random;
		srand(time(NULL));
		random = rand()%3;
		cout << "Random number = " << random << endl;
		if(random == 0)
		{
			obx = 20;
		}
		else if(random == 1)
		{
			obx = 300;
		}
		else if(random == 2)
		{
			obx = 1500;
		}
	}
	drawCircle(440+obx,350+oby,25,RGB_Yellow);
}
void obstacle::incrementob(int n)
{
	if(n == 1)
	{
		oby += 20; 
	}
	else if(n == 2)
	{
		oby += 25;
	}
}
int collision(car c, obstacle o)
{
	if(o.obx+440 >= c.cx+420 && o.obx+440 <= c.cx+520 && o.oby+350+15 >= c.cy && o.oby+350-15 <= c.cy+40)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void maingame(car c, obstacle obj1, obstacle obj2, player person)
{
	char move;
	double seconds;
	int i = 1;
	int collision_with_obj1 = -1,collision_with_obj2 = -1;
	time(&timer1);
	while(collision_with_obj1 != 1 && collision_with_obj2 != 1)
	{
		cls();
		makeClouds();
		makeGrass();
		design();
		readKeys();
		if(keyDown(SDLK_a))
		{
			move = 'a';
			c.movecar(move);
		}
		else if(keyPressed(SDLK_d))
		{
			move = 'd';
			c.movecar(move);
		}
		c.constructcar();
		for(int j = 0; j <= 400; j += 30)
		{
			if(i+j >= 280)
			{
				drawRect(600,301+(j+i)%400,610,321+(j+i)%400,RGB_Yellow);
			}
			else
			{
				drawRect(600,301+j+i,610,321+j+i,RGB_Yellow);
			}
		}
		i += 20;
		obj1.drawobstacle();
		obj2.drawobstacle();
		collision_with_obj1 = collision(c,obj1);
		obj1.incrementob(1);
		collision_with_obj2 = collision(c,obj2);
		obj2.incrementob(2);
		redraw();
		cout << "person.ftd() = " << person.ftd() << endl;
		sleep((100 - person.ftd())/2000); //can go negative
		person.ftimedelay();
	}
	cls();
	time(&timer2);
	seconds = abs(difftime(timer1,timer2));
	cout << "seconds = " << seconds << endl; 
	person.ffinaltime(seconds);
}
int main()
{
	screen(1256, 1256, 0, "Ashphalt Retro");
	car lamborghini;
	obstacle object_1,object_2;
	player person;
	int color;
	std::string line;
	std::ifstream leaderboard;
	do
	{
		startscreen();
		int choice;
		choice = getInput<int>("Enter choice :- ",false,510,400);
		switch(choice)
		{
			case 1:		clouds();
						grass();
						maingame(lamborghini,object_1,object_2,person);
						break;
			case 2:		while(!done())
						{
							cls();
							int position;
							position = print("D --> TURN RIGHT",500,300,RGB_Yellow);
							position = print("A --> TURN LEFT",500,320,RGB_Yellow);
							position = print("OBJECTIVE --> AVOID ALL THE OBSTACLES",500,340,RGB_Yellow);
							position = print("Press Esc key to go back",500,500,RGB_Yellow);
							redraw();
						}
						break;
			case 3:		cls();
						leaderboard.open("High_Scores.txt");
						if(leaderboard.is_open())
						{
							cls();
							int i = 0;
							player p;
							while(!done())
							{
								int position;
								while(getline(leaderboard,line))
								{
									position = print(line,500,320+i,RGB_Yellow);
									i += 20;
									redraw();
								}
								print("Press Esc key to go back to menu",480,350+i,RGB_Yellow);
								redraw();
							}
							leaderboard.close();
						}
						else
						{
							cout << "Unable to open the file.\n";
						}
						break;
			case 4:		end();
						break;
			default:	exit(0);
						break;
		}
		cls();
	}while(true);
	return 0;
}