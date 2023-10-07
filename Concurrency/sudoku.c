#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row;
    int col;
}startpoint;

void *checkrow(void *);
void *checkcol(void *);
void *checkbox(void *);

        // [row][col]
int sudoku[9][9] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6} 
};

int valid[11] = {0};

int main()
{
    pthread_t colt;
    pthread_t rowt;

    startpoint rowncol;
    startpoint *param = (startpoint *) malloc(sizeof(startpoint));
    param->row = 0;
    param->col = 0;
    
    pthread_create(&colt, NULL, checkcol, param);
    pthread_create(&rowt, NULL, checkrow, param);

    //for each box
    pthread_t boxt[9];
    startpoint *params[9];
    startpoint box;
    
    int col = 0;
    int row = 0;
    for (int i = 0; i < 9; ++i)
    {
        params[i] = (startpoint *)malloc(sizeof(startpoint));
        params[i]->row = row;
        params[i]->col = col;
        pthread_create(&boxt[i], NULL, checkbox, params[i]);
        
        if ((i+1) % 3 == 0)
        {
            col += 3;
            row = 0;
        }
        else
            row += 3;
    }    
    pthread_join(colt, NULL);
    pthread_join(rowt, NULL);
    for (int i = 0; i < 9; ++i)
        pthread_join(boxt[i], NULL);
    for (int i = 0; i < 0; ++i)
        free(params[i]);
    free(param);

    int solved = 1;
    for (int i = 0; i < 11; ++i)
        solved &= valid[i];
       //printf("Valid %d : %d\n", i, valid[i]);

    if (solved)
        printf("Sudoku solved!\n");
    else
        printf("Try again buddy :(\n");
    return 0;
}


void *checkcol(void *parm)
{
    startpoint sp;
    startpoint *param = parm;
    sp.row = param->row;
    sp.col = param->col;

    int unique[10] = {0};
    int br = 0;
    for (int i = sp.col; i < 9; ++i)
    {
        if (br == 1)
            break;
        for (int j = sp.row; j < 9; ++j)
        {
            int num = sudoku[j][i];
            if (num > 0 && num < 10 && unique[num] == 0)
                unique[num] = 1;
            else
            {
                br = 1;
                break;
            }
        }
        for (int k = 1; k <= 9; ++k)
            unique[k] = 0;
    }
    if (br == 0)
        valid[1] = 1; 

    pthread_exit(0);
}

void *checkrow(void *parm)
{
    startpoint sp;
    startpoint *param = parm;
    sp.row = param->row;
    sp.col = param->col;

    int unique[10] = {0};
    int br = 0;
    for (int i = sp.row; i < 9; ++i)
    {
        if (br == 1)
            break;
        for (int j = sp.col; j < 9; ++j)
        {
            int num = sudoku[i][j];
            if (num > 0 && num < 10 && unique[num] == 0)
                    unique[num] = 1;
            else
            {
                br = 1;
                break;     
            }
        }
        for (int k = 1; k <= 9; ++k)
            unique[k] = 0;
    }
    if (br == 0)
        valid[0] = 1;
    pthread_exit(0);
}

void *checkbox(void *parm)
{
    startpoint sp;
    startpoint *param = parm;
    sp.row = param->row;
    sp.col = param->col;    

    static int validx = 2;

    int unique[10] = {0};
    int br = 0;
    for (int i = sp.row; i < (sp.row + 3); ++i)
    {
        if (br == 1)
            break;
        for (int j = sp.col; j < (sp.col + 3); ++j)
        {
            int num = sudoku[i][j];
            if (num > 0 && num < 10 && unique[num] == 0)
                unique[num] = 1;
            else
            {
                br = 1;
                break;
            }
        }
    }
    if (br == 0)
        valid[validx++] = 1;

    pthread_exit(0);
}

