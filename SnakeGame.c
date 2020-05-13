#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

//�趨ǽ�ڵĸ߶ȺͿ��
#define HIGH (20)
#define WIDTH (20)

//ש������
#define EMPTY (0)
#define WALL (-1)
#define SNAKE_HEAD (1)
#define SNAKE_BODY (1)

//С���ƶ�����
#define UP (1)
#define DOWN (2)
#define LEFT (3)
#define RIGHT (4)

//ʳ��
#define FOOD (-2)

char DrawBrick[HIGH][WIDTH] = {EMPTY}; //��ש�����飬������ǽ�ڡ��������ͷ

char move_flag = 0; //С���ƶ���־

int speed_time = 100; //�ٶ�ʱ��,ֵԽС��С���ƶ��ٶ�Խ��

int food_x = 0, food_y = 0; //ʳ������

int score = 0; //��Ϸ�÷�

//��Ϸ��ʼ��
void Game_Init(void)
{
    int i, j, k;
    for (i = 0; i < HIGH; i++) //�������߻�ǽ
    {
        DrawBrick[i][0] = WALL;
        DrawBrick[i][WIDTH - 1] = WALL;
    }
    for (j = 0; j < WIDTH; j++) //�������߻�ǽ
    {
        DrawBrick[0][j] = WALL;
        DrawBrick[HIGH - 1][j] = WALL;
    }
    DrawBrick[HIGH / 2][WIDTH / 2] = SNAKE_HEAD; //������ͷ1
    for (k = 1; k <= 3; k++)
    {
        DrawBrick[HIGH / 2][WIDTH / 2 - k] = SNAKE_BODY + k; //��������234��Ĭ�ϳ���Ϊ3
    }

    //��ʼ��С�������ƶ�
    move_flag = RIGHT;

    //��ʼ��ʳ��λ��
    food_x = rand() % (WIDTH - 6) + 2; //����(0~WIDTH-5)+2,������(2,WIDTH-3)
    food_y = rand() % (HIGH - 6) + 2;  //����(0~HIGH-5)+2,������(2,HIGH-3)
    DrawBrick[food_x][food_y] = FOOD;  //����ʳ��
}

//��ӡ����ĳ�ʼ����ÿ��ӡһ�Σ���Ҫ�ص�ԭʼ����
void Coordinate_Init(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

//��������Ԫ��
void Show_Interface(void)
{
    int i, j;
    Coordinate_Init(0, 0); //�ص�ԭʼ���꣬���ڴ�ӡǰ������
    for (i = 0; i < HIGH; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (DrawBrick[i][j] == EMPTY)
                printf("��");
            else if (DrawBrick[i][j] == WALL)
                printf("��");
            else if (DrawBrick[i][j] == SNAKE_HEAD)
                printf("��");
            else if (DrawBrick[i][j] > SNAKE_BODY)
                printf("��");
            else if (DrawBrick[i][j] == FOOD)
                printf("��");
        }
        printf("\n");
    }
    Sleep(speed_time); //ÿ��speed_time����һ��
}

//�ƶ�С��
void Snake_Move(void)
{
    int i, j;
    int old_tail_i = 0, old_tail_j = 0; //�ɵ���β����
    int old_head_i = 0, old_head_j = 0; //�ɵ���ͷ����
    int new_head_i = 0, new_head_j = 0; //�µ���ͷ����
    int max = 0;
    for (i = 1; i < HIGH - 1; i++)
    {
        for (j = 1; j < WIDTH - 1; j++)
        {
            if (DrawBrick[i][j] > EMPTY) //����С��λ��
                DrawBrick[i][j]++;
        }
    }
    for (i = 1; i < HIGH - 1; i++)
    {
        for (j = 1; j < WIDTH - 1; j++)
        {
            if (DrawBrick[i][j] > EMPTY)
            {
                if (DrawBrick[i][j] == SNAKE_HEAD + 1) //������ͷ
                {
                    old_head_i = i;
                    old_head_j = j; //��¼��ͷ���꣬����Ϊ�����ƶ���ͷ��׼��
                }
                if (DrawBrick[i][j] > max) //����̨�㷨�����ֵ���Ӷ��ҵ���β
                {
                    max = DrawBrick[i][j];
                    old_tail_i = i;
                    old_tail_j = j; //��¼��β����
                }
            }
        }
    }

    //�������뷽���־���ı���ͷ����
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
    if (DrawBrick[new_head_i][new_head_j] == FOOD) //��ͷ����ʳ��
    {
        DrawBrick[food_x][food_y] = EMPTY; //���ʳ��
        food_x = rand() % (WIDTH - 6) + 2; //����(0~WIDTH-5)+2,������(2,WIDTH-3)
        food_y = rand() % (HIGH - 6) + 2;  //����(0~HIGH-5)+2,������(2,HIGH-3)
        DrawBrick[food_x][food_y] = FOOD;  //�ٴβ���ʳ��
        //DrawBrick[old_tail_i][old_tail_j] = EMPTY; //���������β���������ȼ�1
        speed_time -= 1; //ÿ��һ��ʳ�С�߾ͻ����һ��
        score += 10;     //ÿ��һ��ʳ���10��
        if (speed_time <= 50)
        {
            printf("��ϲ�㣬ͨ�سɹ�! ���յ÷�Ϊ:%d\n", score);
            exit(0);
        }
    }
    else
    {
        DrawBrick[old_tail_i][old_tail_j] = EMPTY; //�������β
    }

    //��������������µ���ͷ
    if (DrawBrick[new_head_i][new_head_j] == EMPTY)
    {
        DrawBrick[new_head_i][new_head_j] = SNAKE_HEAD;
    }
    //��ײ���(�����Լ���������ǽ)
    else if (DrawBrick[new_head_i][new_head_j] > SNAKE_HEAD || DrawBrick[new_head_i][new_head_j] == WALL)
    {
        printf("��Ϸ����! ���յ÷�Ϊ:%d\n", score);
        exit(0); //�˳���Ϸ
    }
}

//С���˶�������
void Direction_Test(void)
{
    char input;  //��ȡ�û�����ķ����ַ�
    if (kbhit()) //����Ƿ������룬һ������ͽ��룬�Ž�һ����ȡgetch()��ֵ����ֹ��getch()��ѭ����ѯ�Ƿ�������
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

//���������
int main()
{
    Game_Init(); //��ʼ����Ϸ
    while (1)
    {
        Show_Interface(); //��ʾ��Ϸ����
        Direction_Test(); //������
        printf("��ǰ�÷�:%d\n", score);
    }
}