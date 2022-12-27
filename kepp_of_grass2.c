#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct s_cell {
    int scrap_amount;
    int owner;
    int units;
    int recycler;
    int can_build;
    int can_spawn;
    int in_range_of_recycler;
    int spawn_objetive;
} t_cell;

int ft_max_rcy(int my_cells) { //Calcular numero de recicladores optimo
    int max = (my_cells / 10);
    if (max < 5)
       max++;
    if (max >= 6 && max < 12)
        max--;
    else if (max >= 12)
        max -= 2;
    //fprintf(stderr, "cells: %d, max recyclers: %d\n", my_cells, max);
    return (max);
    //return(1);
}

time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int in_line(int y, int width, t_cell ***map)
{
    int x = 0;
    while (x < width)
    {
        if (map[y][x]->owner == 1 && map[y][x]->units > 0)
            return (0);
        x++;
    }
    return (0);
}

void ft_read_map(t_cell ***map){
    fprintf(stderr, "hola\n");
}

int main()
{
    int width;
    int height;
    int x, y;         //recorrer mapa
    int spw_x, spw_y; //punto de spwan
    int spawn;
    int center_x, center_y;
    int to_x, to_y;
    int r_x, r_y;
    int units;
    int build;
    int cells = 0;
    int build_last = 0;
    int build_limit = 2;
    int first = 1;
    int vX, vY;     //vanguardia spawn
    int dirX, dirY; //direccions 
    time_t time;
    scanf("%d%d", &width, &height);
    t_cell map[height][width];
    center_x = width/2;
    center_y = height/2;
    int distX = -1;
    int turn = 0;

    
    while (1) {                                 // game loop
        int my_matter;
        int opp_matter;
        cells = 0;
        scanf("%d%d", &my_matter, &opp_matter);
        for (int i = 0; i < height; i++) {       // lectura del mapa
            for (int j = 0; j < width; j++) {
                int scrap_amount;                // puntos para ceciclar  
                int owner;                       // 1 = me, 0 = foe, -1 = neutral
                int units;
                int recycler;
                int can_build;
                int can_spawn;
                int in_range_of_recycler;
                scanf("%d%d%d%d%d%d%d", &scrap_amount, &owner, &units, &recycler, &can_build, &can_spawn, &in_range_of_recycler);
                map[i][j].scrap_amount = scrap_amount;
                map[i][j].owner = owner;
                map[i][j].units = units;
                map[i][j].recycler = recycler;
                map[i][j].can_build = can_build;
                map[i][j].can_spawn = can_spawn;
                map[i][j].in_range_of_recycler = in_range_of_recycler;
                if(owner == 1)
                    cells++;        
            }
        }
        turn++;
        time = get_time_in_ms();
        
        y = 0;
        build = 0;
        if (turn >= 2)
        {
            printf("SPAWN %d %d %d;", my_matter/10, vX, vY);
            fprintf(stderr, "vx: %d, vy: %d\n", vX, vY);
        }
        while (y < height)                      /* Reading map */
        { 
            x = 0;
            while (x < width)
            {
                if (map[y][x].owner == 1 && map[y][x].recycler)
                    build++;
                if (map[y][x].owner == 0 && map[y][x].units > 0)
                {
                    if(first)
                    {
                        if (x < center_x)
                            dirX = -1;
                        else
                            dirX = 1;
                        if (y < center_y)
                            dirY = -1;
                        else
                            dirY = 1;
                        first = 0;
                        fprintf(stderr, "debug dirX: %d\n", dirX);
                    }
                    to_x = x;
                    to_y = y;
                }
                if (map[y][x].can_spawn)
                {
                    if (dirX == 1)
                    {
                        if (distX == -1)
                        {
                            distX = center_x - x;
                            vX = x;
                            vY = y;
                        }
                        else if (distX > center_x - x && distX < center_x)
                        {
                            distX = center_x -x;
                            vX = x;
                            vY = y;
                        }
                    }
                    else
                    {
                        if (distX == -1)
                        {
                            distX = x - center_x;
                            vX = x;
                            vY = y;
                        }
                        else if (distX > x - center_x && distX > center_x)
                        {
                            distX = x - center_x;
                            vX = x;
                            vY = y;
                        }
                    }
                    /*spawn junto a casilla enemiga*/
                    if ((map[y+1][x].owner == 0 && y+1 < height && map[y+1][x].scrap_amount > 0 && !(map[y+1][x].in_range_of_recycler && map[y][x].scrap_amount == 1)) || (map[y-1][x].owner == 0 && y > 0 && map[y-1][x].scrap_amount > 0 && !(map[y-1][x].in_range_of_recycler && map[y][x].scrap_amount == 1)) || 
                        (map[y][x+1].owner == 0  && x+1 < width && map[y][x+1].scrap_amount > 0 && !(map[y][x+1].in_range_of_recycler && map[y][x].scrap_amount == 1)) || (map[y][x-1].owner == 0 && x > 0 && map[y][x-1].scrap_amount > 0 && !(map[y][x-1].in_range_of_recycler && map[y][x].scrap_amount == 1)))
                    {
                        printf("SPAWN %d %d %d;", 1, x, y);
                    }
                    //else if (dirX == 1 || dirY == 1)
                    //    printf("SPAWN %d %d %d;", 1, vX, vY);
                    /*spawn junto a casilla no controlada*/
                    else if ((map[y+1][x].owner == -1 && y+1 < height && map[y+1][x].scrap_amount > 0) || (map[y-1][x].owner == -1 && y > 0 && map[y-1][x].scrap_amount > 0) || 
                            (map[y][x+1].owner == -1  && x+1 < width && map[y][x+1].scrap_amount > 0)|| (map[y][x-1].owner == -1 && x > 0 && map[y][x-1].scrap_amount > 0))
                    {   
                            printf("SPAWN %d %d %d;", 1, x, y);
                    }
                    r_x = x;
                    r_y = y;
                }
                if (map[y][x].owner == 1 && map[y][x].units > 0 && dirX == 1)
                {
                    units = map[y][x].units;
                    if (map[y][x + 1].scrap_amount > 0 && x+1 < width && map[y][x + 1].owner != 1 && !map[y][x+1].recycler && !(map[y][x+1].scrap_amount == 1 && map[y][x+1].in_range_of_recycler)) // && !(map[y][x+1].scrap_amount == 1 && map[y][x+1].in_range_of_recycler)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x+1, y);
                    else if (map[y][x - 1].scrap_amount > 0 && x-1 >= 0 &&  map[y][x - 1].owner != 1 && !map[y][x-1].recycler && !(map[y][x-1].scrap_amount == 1 && map[y][x-1].in_range_of_recycler))
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x-1, y);
                    else if (dirY == -1)
                    {
                        if (map[y + 1][x].scrap_amount > 0 && y+1 < height &&  map[y + 1][x].owner != 1 && !map[y+1][x].recycler)
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y+1);
                        else if (map[y - 1][x].scrap_amount > 0 && y-1 >= 0 &&  map[y - 1][x].owner != 1 && !map[y-1][x].recycler)
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y-1);
                        else
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, to_x, to_y);
                    }
                    else if (dirY == 1)
                    {        
                        if (map[y - 1][x].scrap_amount > 0 && y-1 >= 0 &&  map[y - 1][x].owner != 1 && !map[y-1][x].recycler && !(map[y][x-1].scrap_amount == 1 && map[y][x-1].in_range_of_recycler))
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y-1);
                        else if (map[y + 1][x].scrap_amount > 0 && y+1 < height &&  map[y + 1][x].owner != 1 && !map[y+1][x].recycler && !(map[y][x+1].scrap_amount == 1 && map[y][x+1].in_range_of_recycler))
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y+1);
                        else
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, to_x, to_y);
                    }
                }
                else if (map[y][x].owner == 1 && map[y][x].units > 0 && dirX == -1)//al otro lado
                {
                    units = map[y][x].units;
                    if (map[y][x - 1].scrap_amount > 0 && x-1 >= 0 &&  map[y][x - 1].owner != 1 && !map[y][x-1].recycler)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x-1, y);
                    else if (map[y][x + 1].scrap_amount > 0 && x+1 < width && map[y][x + 1].owner != 1 && !map[y][x+1].recycler)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x+1, y);
                    else if (dirY == -1)
                    {
                        if (map[y + 1][x].scrap_amount > 0 && y+1 < height &&  map[y + 1][x].owner != 1 && !map[y+1][x].recycler)
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y+1);
                        else if (map[y - 1][x].scrap_amount > 0 && y-1 >= 0 &&  map[y - 1][x].owner != 1 && !map[y-1][x].recycler)
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y-1);
                        else
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, to_x, to_y);
                    }
                    else if (dirY == 1)
                    {        
                        if (map[y - 1][x].scrap_amount > 0 && y-1 >= 0 &&  map[y - 1][x].owner != 1 && !map[y-1][x].recycler)
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y-1);
                        else if (map[y + 1][x].scrap_amount > 0 && y+1 < height &&  map[y + 1][x].owner != 1 && !map[y+1][x].recycler)
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y+1);
                        else
                            printf("MOVE %d %d %d %d %d;" ,units , x, y, to_x, to_y);
                    }
                }
                /* Build */
                if (map[y][x].can_build  && build_last < build_limit && turn > 1)
                {
                    if ((map[y][x+1].owner == 0 && map[y][x+1].units > 0 && x+1 < width) || (map[y][x-1].owner == 0 && map[y][x-1].units > 0 && x > 0) || (map[y+1][x].owner == 0 && map[y+1][x].units > 0  && y+1 < height) || (map[y-1][x].owner == 0 && map[y-1][x].units > 0 && y > 0))
                    {
                        printf("BUILD %d %d;", x, y);
                        build_last++;   
                    }
                    else if (y + 1 < height && y - 1 > 0 && x + 1 < width && x - 1 > 0 && map[y][x].scrap_amount > 8 && !map[y][x].in_range_of_recycler)
                    {
                        printf("BUILD %d %d;", x, y);
                        build_last++;      
                    }
                }
                x++;
            }
            y++;
        } /* final */
        fprintf(stderr, "turn: %d\n", turn);
        //distX == -1;
        //if (dirX == 1 || dirY == 1)
        //    printf("SPAWN %d %d %d;", my_matter/10, vX, vY);
        //ft_read_map(&map);
        build_last = build;
        build = 0;
        if (my_matter > build_limit * 10)
            printf("SPAWN %d %d %d;", 1, r_x, r_y);
        printf("\n");
        build_limit = ft_max_rcy(cells);
        fprintf(stderr, "%ld\n", get_time_in_ms() - time);
    } //fin partida
    return 0;
}

/*
else if (map[y][x + 1].scrap_amount > 0 && x+1 < width)
    printf("MOVE %d %d %d %d %d;" ,units , x, y, x+1, y);
else if (map[y][x - 1].scrap_amount > 0 && x-1 >= 0)
    printf("MOVE %d %d %d %d %d;" ,units , x, y, x-1, y);
else if (map[y + 1][x].scrap_amount > 0 && y+1 < height)
    printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y+1);
else if (map[y - 1][x].scrap_amount > 0 && y-1 >= 0)
    printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y-1);
*/
//fprintf(stderr, "%d, %d\n", map[i][j].y, map[i][j].x);
// Write an action using printf(). DON'T FORGET THE TRAILING \n
//printf("BUILD 0 3\n");
// To debug: fprintf(stderr, "Debug messages...\n");
//printf("WAIT\n");