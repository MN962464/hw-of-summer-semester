#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>  
#define ROW 7
#define COL 8
int map[ROW][COL] = {
	{ 0,1,1,1,1,1,1,0 },
	{ 1,1,0,0,4,0,1,1,},
	{ 1,0,2,0,2,2,0,1 },
	{ 1,3,3,3,3,3,3,1 },
	{ 1,0,2,2,0,2,0,1 },
	{ 1,1,1,0,0,1,1,1 },
	{ 0,0,1,1,1,1,0,0 },
};
int end = 0;                         //中途退出的标记
int personX, personY;               //一开始人的坐标
int boxNumber, win;                  //boxNumber表示地图上目的地的个数，win表示已经推到目的地的箱子个数

int Repaint(HDC hdc, HWND hwnd)
{
	RECT clientRect;
	HRGN bgRgn;
	HBRUSH hBrush;
	HPEN hPen;

	// Fill the client area with a brush
	GetClientRect(hwnd, &clientRect);
	bgRgn = CreateRectRgnIndirect(&clientRect);
	hBrush = CreateSolidBrush(RGB(173, 172, 172));
	FillRgn(hdc, bgRgn, hBrush);
	int pos_x;
	int pos_y;
	hPen = CreatePen(PS_SOLID, 0, RGB(195, 195, 195));
	SelectObject(hdc, hPen);
	//SetBkColor(hdc, RGB(400, 26, 211));
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			pos_x = 80 + 50 * j;
			pos_y = 80 + 50 * i;
			switch (map[i][j])
			{
			case 1:
				hBrush = CreateSolidBrush(RGB(56, 51, 80));
				SelectObject(hdc, hBrush);
				Rectangle(hdc, pos_x, pos_y, pos_x + 50, pos_y + 50);
				break;
			case 2:
			case 6:
				hBrush = CreateSolidBrush(RGB(196, 28, 28));
				SelectObject(hdc, hBrush);
				Rectangle(hdc, pos_x, pos_y, pos_x + 50, pos_y + 50);
				break;
			case 3:
				hBrush = CreateSolidBrush(RGB(244, 108, 50));
				SelectObject(hdc, hBrush);
				Ellipse(hdc, pos_x + 8, pos_y + 8, pos_x + 42, pos_y + 42);
				break;
			case 4:
				hBrush = CreateSolidBrush(RGB(27, 148, 254));
				SelectObject(hdc, hBrush);
				Ellipse(hdc, pos_x + 15, pos_y, pos_x + 35, pos_y + 50);
				break;
			case 5:
				hBrush = CreateSolidBrush(RGB(27, 148, 254));
				SelectObject(hdc, hBrush);
				Ellipse(hdc, pos_x + 15, pos_y, pos_x + 35, pos_y + 50);
				hBrush = CreateSolidBrush(RGB(244, 108, 50));
				SelectObject(hdc, hBrush);
				Ellipse(hdc, pos_x + 8, pos_y + 8, pos_x + 42, pos_y + 42);
				break;
			}
		}
		printf("\n");
	}

	// Clean up
	DeleteObject(bgRgn);
	DeleteObject(hBrush);
	DeleteObject(hPen);

	GetStockObject(WHITE_BRUSH);
	GetStockObject(DC_PEN);

	return 0;
}
void start()
{
	personX = 0, personY = 0;
	boxNumber = 0;
	end = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			switch (map[i][j])
			{
			case 3:
				boxNumber += 1;
				break;
			case 4:
			case 5:
				personX = i, personY = j;
				break;
			}
		}
	}

}
void moveToUp()
{
	//人的下一个坐标
	int personNextX = personX-1;
	int personNextY = personY;

	//箱子的坐标和下一个坐标
	int BoxX, BoxY, BoxNextX, BoxNextY;
	if (map[personX][personY] == 5 || map[personX][personY] == 6)
		map[personX][personY] = 3;
	else
		map[personX][personY] = 0;
	if (map[personNextX][personNextY] == 0)            //如果下一个坐标是0即空地   人到空地上，人的坐标变为空地
	{
		map[personNextX][personNextY] = 4;
		personX = personNextX;
		personY = personNextY;
	}
	else if (map[personNextX][personNextY] == 1)      //如果下一个坐标是1即墙
	{
		if (map[personX][personY]) map[personX][personY] = 5;
		else map[personX][personY] = 4;//不进行操作
	}
	else if (map[personNextX][personNextY] == 2|| map[personNextX][personNextY] == 6)      //如果下一个坐标是2即箱子
	{
		BoxX = personNextX;                           //当前的箱子就是人的下一个坐标
		BoxY = personNextY;
		BoxNextX = BoxX-1;
		BoxNextY = BoxY;
		if (map[BoxNextX][BoxNextY] == 2 || map[BoxNextX][BoxNextY] == 1|| map[BoxNextX][BoxNextY] == 6)
			if(map[personX][personY]) map[personX][personY]=5;
			else map[personX][personY] = 4;
		else
		{
			if (map[BoxX][BoxY] == 6)
			{
				map[BoxX][BoxY] = 5;
				win--;
			}
			else
				map[BoxX][BoxY] = 4;
			if (map[BoxNextX][BoxNextY] == 0)
			{
				map[BoxNextX][BoxNextY] = 2;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
			}
			else if (map[BoxNextX][BoxNextY] == 3)
			{
				map[BoxNextX][BoxNextY] = 6;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
				win++;
		     //已经推进去的箱子数加1
			}
		}
	}
	else if (map[personNextX][personNextY] == 3|| map[personNextX][personNextY] == 5)            //如果下一个坐标是0目的地  
	{
		map[personNextX][personNextY] = 5;
		personX = personNextX;
		personY = personNextY;
	}
}
//小人向下移动的方法  
void moveToDown()
{
	//人的下一个坐标
	int personNextX = personX+1;
	int personNextY = personY;


	//箱子的坐标和下一个坐标
	int BoxX, BoxY, BoxNextX, BoxNextY;
	if (map[personX][personY] == 5|| map[personX][personY] == 6)
		map[personX][personY] = 3;
	else
		map[personX][personY] = 0;
	if (map[personNextX][personNextY] == 0)            //如果下一个坐标是0即空地   人到空地上，人的坐标变为空地
	{
		map[personNextX][personNextY] = 4;
		personX = personNextX;
		personY = personNextY;
	}
	else if (map[personNextX][personNextY] == 1)      //如果下一个坐标是1即墙
	{
		if (map[personX][personY]) map[personX][personY] = 5;
		else map[personX][personY] = 4;//不进行操作
	}
	else if (map[personNextX][personNextY] == 2|| map[personNextX][personNextY] == 6)      //如果下一个坐标是2即箱子
	{
		BoxX = personNextX;                           //当前的箱子就是人的下一个坐标
		BoxY = personNextY;
		BoxNextX = BoxX+1;
		BoxNextY = BoxY;
		if (map[BoxNextX][BoxNextY] == 2 || map[BoxNextX][BoxNextY] == 1||map[BoxNextX][BoxNextY] == 6)
			if (map[personX][personY]) map[personX][personY] = 5;
			else map[personX][personY] = 4;
		else
		{
			if (map[BoxX][BoxY] == 6)
				map[BoxX][BoxY] = 5;
			else
				map[BoxX][BoxY] = 4;
			if (map[BoxNextX][BoxNextY] == 0)
			{
				map[BoxNextX][BoxNextY] = 2;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
			}
			else if (map[BoxNextX][BoxNextY] == 3)
			{
				map[BoxNextX][BoxNextY] = 6;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
				win += 1;    //已经推进去的箱子数加1
			}
		}
	}
	else if (map[personNextX][personNextY] == 3|| map[personNextX][personNextY] == 5)            //如果下一个坐标是0目的地  
	{
		map[personNextX][personNextY] = 5;
		personX = personNextX;
		personY = personNextY;
	}
}

//小人向左移动的方法  
void moveToLeft()
{
	//人的下一个坐标
	int personNextX = personX;
	int personNextY = personY-1;

	//箱子的坐标和下一个坐标
	int BoxX, BoxY, BoxNextX, BoxNextY;
	if (map[personX][personY]==5 || map[personX][personY] == 6)
		map[personX][personY] = 3;
	else	
		map[personX][personY] = 0;
	if (map[personNextX][personNextY] == 0)            //如果下一个坐标是0即空地   人到空地上，人的坐标变为空地
	{
		map[personNextX][personNextY] = 4;
		personX = personNextX;
		personY = personNextY;
	}
	else if (map[personNextX][personNextY] == 1)      //如果下一个坐标是1即墙
	{
		if (map[personX][personY]) map[personX][personY] = 5;
		else map[personX][personY] = 4;//不进行操作
	}
	else if (map[personNextX][personNextY] == 2|| map[personNextX][personNextY] == 6)      //如果下一个坐标是2即箱子
	{
		BoxX = personNextX;                           //当前的箱子就是人的下一个坐标
		BoxY = personNextY;
		BoxNextX = BoxX;
		BoxNextY = BoxY-1;
		if (map[BoxNextX][BoxNextY] == 2|| map[BoxNextX][BoxNextY] == 1||map[BoxNextX][BoxNextY] == 6)
			if (map[personX][personY]) map[personX][personY] = 5;
			else map[personX][personY] = 4;
		else
		{
			if (map[BoxX][BoxY] == 6)
				map[BoxX][BoxY] = 5;
			else
				map[BoxX][BoxY] = 4;
			if (map[BoxNextX][BoxNextY] == 0)
			{
				map[BoxNextX][BoxNextY] = 2;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
			}
			else if (map[BoxNextX][BoxNextY] == 3 || map[BoxNextX][BoxNextY] == 6)
			{
				map[BoxNextX][BoxNextY] = 6;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
				win += 1;    //已经推进去的箱子数加1
			}
		}
	}
	else if (map[personNextX][personNextY] == 3)            //如果下一个坐标是0目的地  
	{
		map[personNextX][personNextY] = 5;
		personX = personNextX;
		personY = personNextY;
	}
}
//小人向右移动的方法  
void moveToRight()
{
	//人的下一个坐标
	int personNextX = personX;
	int personNextY = personY+1;

	//箱子的坐标和下一个坐标
	int BoxX, BoxY, BoxNextX, BoxNextY;
	if (map[personX][personY] == 5 || map[personX][personY] == 6)
		map[personX][personY] = 3;
	else
		map[personX][personY] = 0;
	if (map[personNextX][personNextY] == 0)            //如果下一个坐标是0即空地   人到空地上，人的坐标变为空地
	{
		map[personNextX][personNextY] = 4;
		personX = personNextX;
		personY = personNextY;
	}
	else if (map[personNextX][personNextY] == 1)      //如果下一个坐标是1即墙
	{
		if (map[personX][personY]) map[personX][personY] = 5;
		else map[personX][personY] = 4;//不进行操作
	}
	else if (map[personNextX][personNextY] == 2 || map[personNextX][personNextY] == 6)      //如果下一个坐标是2即箱子
	{
		BoxX = personNextX;                           //当前的箱子就是人的下一个坐标
		BoxY = personNextY;
		BoxNextX = BoxX;
		BoxNextY = BoxY + 1;
		if (map[BoxNextX][BoxNextY] == 2 || map[BoxNextX][BoxNextY] == 1||map[BoxNextX][BoxNextY] == 6)
			if (map[personX][personY]) map[personX][personY] = 5;
			else map[personX][personY] = 4;
		else
		{
			if (map[BoxX][BoxY] == 6)
				map[BoxX][BoxY] = 5;
			else
				map[BoxX][BoxY] = 4;
			if (map[BoxNextX][BoxNextY] == 0)
			{
				map[BoxNextX][BoxNextY] = 2;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
			}
			else if (map[BoxNextX][BoxNextY] == 3)
			{
				map[BoxNextX][BoxNextY] = 6;
				personX = personNextX;                   //人的下一个坐标变为人的当前坐标
				personY = personNextY;
			}
		}
	}
	else if (map[personNextX][personNextY] == 3|| map[personNextX][personNextY] == 5)            //如果下一个坐标是0目的地  
	{
		map[personNextX][personNextY] = 5;
		personX = personNextX;
		personY = personNextY;
	}
}

int getwin() 
{
	int count = 0;
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			if (map[i][j] == 6)
				count++;
	return count;
}