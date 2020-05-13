#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

//设定墙壁的高度和宽度
#define HIGH (20)
#define WIDTH (20)

//砖块类型
#define EMPTY (0)
#define WALL (-1)
#define SNAKE_HEAD (1)
#define SNAKE_BODY (1)

//小蛇移动方向
#define UP (1)
#define DOWN (2)
#define LEFT (3)
#define RIGHT (4)

//食物
#define FOOD (-2)

char DrawBrick[HIGH][WIDTH] = {EMPTY}; //画砖块数组，包括画墙壁、蛇身和蛇头

char move_flag = 0; //小蛇移动标志

int speed_time = 100; //速度时间,值越小，小蛇移动速度越快

int food_x = 0, food_y = 0; //食物坐标

int score = 0; //游戏得分

//游戏初始化
void Game_Init(void)
{
    int i, j, k;
    for (i = 0; i < HIGH; i++) //左右两边画墙
    {
        DrawBrick[i][0] = WALL;
        DrawBrick[i][WIDTH - 1] = WALL;
    }
    for (j = 0; j < WIDTH; j++) //上下两边画墙
    {
        DrawBrick[0][j] = WALL;
        DrawBrick[HIGH - 1][j] = WALL;
    }
    DrawBrick[HIGH / 2][WIDTH / 2] = SNAKE_HEAD; //画出蛇头1
    for (k = 1; k <= 3; k++)
    {
        DrawBrick[HIGH / 2][WIDTH / 2 - k] = SNAKE_BODY + k; //画出蛇身234，默认长度为3
    }

    //初始化小蛇向右移动
    move_flag = RIGHT;

    //初始化食物位置
    food_x = rand() % (WIDTH - 6) + 2; //产生(0~WIDTH-5)+2,即产生(2,WIDTH-3)
    food_y = rand() % (HIGH - 6) + 2;  //产生(0~HIGH-5)+2,即产生(2,HIGH-3)
    DrawBrick[food_x][food_y] = FOOD;  //产生食物
}

//打印坐标的初始化，每打印一次，都要回到原始坐标
void Coordinate_Init(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

//画出界面元素
void Show_Interface(void)
{
    int i, j;
    Coordinate_Init(0, 0); //回到原始坐标，用于打印前的清屏
    for (i = 0; i < HIGH; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (DrawBrick[i][j] == EMPTY)
                printf("□");
            else if (DrawBrick[i][j] == WALL)
                printf("■");
            else if (DrawBrick[i][j] == SNAKE_HEAD)
                printf("◆");
            else if (DrawBrick[i][j] > SNAKE_BODY)
                printf("■");
            else if (DrawBrick[i][j] == FOOD)
                printf("●");
        }
        printf("\n");
    }
    Sleep(speed_time); //每隔speed_time更新一次
}

//移动小蛇
void Snake_Move(void)
{
    int i, j;
    int old_tail_i = 0, old_tail_j = 0; //旧的蛇尾坐标
    int old_head_i = 0, old_head_j = 0; //旧的蛇头坐标
    int new_head_i = 0, new_head_j = 0; //新的蛇头坐标
    int max = 0;
    for (i = 1; i < HIGH - 1; i++)
    {
        for (j = 1; j < WIDTH - 1; j++)
        {
            if (DrawBrick[i][j] > EMPTY) //发现小蛇位置
                DrawBrick[i][j]++;
        }
    }
    for (i = 1; i < HIGH - 1; i++)
    {
        for (j = 1; j < WIDTH - 1; j++)
        {
            if (DrawBrick[i][j] > EMPTY)
            {
                if (DrawBrick[i][j] == SNAKE_HEAD + 1) //发现蛇头
                {
                    old_head_i = i;
                    old_head_j = j; //记录蛇头坐标，用来为后面移动蛇头做准备
                }
                if (DrawBrick[i][j] > max) //打擂台算法找最大值，从而找到蛇尾
                {
                    max = DrawBrick[i][j];
                    old_tail_i = i;
                    old_tail_j = j; //记录蛇尾坐标
                }
            }
        }
    }

    //根据输入方向标志，改变蛇头坐标
    if (move_flag == UP)
    {
        new_head_i = old_head_i - 1;
        new_head_j = old_head_j;
    }
    else if (move_flag == DOWN)
    {
        new_head_i = old_head_i + 1;
        new_head_j = old_head_j;
    }
    else if (move_flag == LEFT)
    {
        new_head_i = old_head_i;
        new_head_j = old_head_j - 1;
    }
    else if (move_flag == RIGHT)
    {
        new_head_i = old_head_i;
        new_head_j = old_head_j + 1;
    }
    if (DrawBrick[new_head_i][new_head_j] == FOOD) //蛇头碰到食物
    {
        DrawBrick[food_x][food_y] = EMPTY; //清除食物
        food_x = rand() % (WIDTH - 6) + 2; //产生(0~WIDTH-5)+2,即产生(2,WIDTH-3)
        food_y = rand() % (HIGH - 6) + 2;  //产生(0~HIGH-5)+2,即产生(2,HIGH-3)
        DrawBrick[food_x][food_y] = FOOD;  //再次产生食物
        //DrawBrick[old_tail_i][old_tail_j] = EMPTY; //不清除旧蛇尾，即蛇身长度加1
        speed_time -= 1; //每吃一次食物，小蛇就会加速一下
        score += 10;     //每吃一次食物加10分
        if (speed_time <= 50)
        {
            printf("恭喜你，通关成功! 最终得分为:%d\n", score);
            exit(0);
        }
    }
    else
    {
        DrawBrick[old_tail_i][old_tail_j] = EMPTY; //清除旧蛇尾
    }

    //正常情况，产生新的蛇头
    if (DrawBrick[new_head_i][new_head_j] == EMPTY)
    {
        DrawBrick[new_head_i][new_head_j] = SNAKE_HEAD;
    }
    //碰撞检测(碰到自己或者碰到墙)
    else if (DrawBrick[new_head_i][new_head_j] > SNAKE_HEAD || DrawBrick[new_head_i][new_head_j] == WALL)
    {
        printf("游戏结束! 最终得分为:%d\n", score);
        exit(0); //退出游戏
    }
}

//小蛇运动方向检测
void Direction_Test(void)
{
    char input;  //获取用户输入的方向字符
    if (kbhit()) //检测是否有输入，一有输入就进入，才进一步获取getch()的值，防止在getch()中循环查询是否有输入
    {
        input = getch();
        if (input == 'w')
            move_flag = UP;
        if (input == 's')
            move_flag = DOWN;
        if (input == 'a')
            move_flag = LEFT;
        if (input == 'd')
            move_flag = RIGHT;
    }
    if (move_flag > 0)
    {
        Snake_Move();
    }
}

//主程序入口
int main()
{
    Game_Init(); //初始化游戏
    while (1)
    {
        Show_Interface(); //显示游戏界面
        Direction_Test(); //方向检测
        printf("当前得分:%d\n", score);
    }
}