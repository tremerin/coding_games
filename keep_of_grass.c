#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_cell
{
    int scrap_amount;
    int owner;
    int units;
    int recycler;
    int can_build;
    int can_spawn;
    int in_range_of_recycler;
} t_cell;

int main()
{
    int width;
    int height;
    int x;
    int y;
    int spawn;
    int to_x;
    int to_y;
    int r_x;
    int r_y;
    int units;
    scanf("%d%d", &width, &height);
    t_cell map[height][width];
    
    while (1) {                                 // game loop
        int my_matter;
        int opp_matter;
        scanf("%d%d", &my_matter, &opp_matter);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int scrap_amount;                //   
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
            }
        }
        /* Reading map */
        y = 0;
        while (y < height)
        { 
            x = 0;
            while (x < width)
            {
                if (map[y][x].owner == 0 && map[y][x].units > 0)
                {
                    to_x = x;
                    to_y = y;
                }
                if (map[y][x].owner == 1 && map[y][x].units == 0)
                {
                    r_x = x;
                    r_y = y;
                }
                if (map[y][x].owner == 1 && map[y][x].units > 0)
                {
                    units = map[y][x].units;
                    if (map[y][x + 1].scrap_amount > 0 && x+1 < width && map[y][x + 1].owner != 1)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x+1, y);
                    else if (map[y][x - 1].scrap_amount > 0 && x-1 >= 0 &&  map[y][x - 1].owner != 1)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x-1, y);
                    else if (map[y + 1][x].scrap_amount > 0 && y+1 < height &&  map[y + 1][x].owner != 1)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y+1);
                    else if (map[y - 1][x].scrap_amount > 0 && y-1 >= 0 &&  map[y - 1][x].owner != 1)
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, x, y-1);
                    else 
                        printf("MOVE %d %d %d %d %d;" ,units , x, y, to_x, to_y);

                    if (my_matter >= 30)
                        printf("SPAWN %d %d %d;", 1, x, y);
                }
                if (map[y][x].owner == 1 && !map[y][x].in_range_of_recycler && my_matter >= 10)
                {
                    //printf("BUILD %d %d;", x, y);
                }
                x++;
            }
            y++;
            if (my_matter >= 30)
                printf("SPAWN %d %d %d;", 1, r_x, r_y);
        }
        printf("\n");
    }
    return 0;
}

//fprintf(stderr, "%d, %d\n", map[i][j].y, map[i][j].x);  
// Write an action using printf(). DON'T FORGET THE TRAILING \n
//printf("BUILD 0 3\n");
// To debug: fprintf(stderr, "Debug messages...\n");
//printf("WAIT\n");
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