#include <SFML/Graphics.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159

// Structure Map
typedef struct Map {
    sfTexture* texture;
    sfSprite* spr;
} Map;

//Structure Ship
typedef struct Ship{
    sfVector2f pos,dir,force,normal;
    sfTexture* texture;
    sfTexture* texture_mouv;
    sfSprite* spr;
    float rota, speed,unspeed,rotaspeed,len;
}Ship;

//Structures Asteroids
typedef struct Asteroids{
    sfVector2f pos;
    sfTexture* texture;
    sfSprite* spr;
}Asteroids;
typedef struct Asteroids2{
    sfVector2f pos;
    sfTexture* texture;
    sfSprite* spr;
}Asteroids2;
typedef struct Asteroids3{
    sfVector2f pos;
    sfTexture* texture;
    sfSprite* spr;
}Asteroids3;



void setup(sfImage** icon, sfRenderWindow** window, Map* map,Ship* ship);
void draw(sfRenderWindow* window, Map* map,Ship* ship);
void mouv(Ship* ship);

int main() {
    srand(time(NULL));
    sfRenderWindow* window = NULL;
    sfImage* icon = NULL;
    Map map;
    Ship ship;


    setup(&icon, &window, &map,&ship, &clock);



    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        mouv(&ship);
        draw(window, &map,&ship);

    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||Destroy||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfTexture_destroy(ship.texture);
    sfTexture_destroy(ship.texture_mouv);
    sfSprite_destroy(ship.spr);
    sfTexture_destroy(map.texture);
    sfSprite_destroy(map.spr);
    sfRenderWindow_destroy(window);
    sfImage_destroy(icon);


    return 0;
}

void setup(sfImage** icon, sfRenderWindow** window, Map* map,Ship* ship) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    *icon = sfImage_createFromFile("ship.png");
    sfVideoMode mode = { 1080, 1080, 32 };
    *window = sfRenderWindow_create(mode, "Staroids",sfDefaultStyle, NULL);
    sfWindow_setIcon(*window, 65, 65, sfImage_getPixelsPtr(*icon));

    sfRenderWindow_setFramerateLimit(*window, 60);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    map->texture = sfTexture_createFromFile("space_background.png", NULL);
    map->spr = sfSprite_create();
    sfSprite_setTexture(map->spr, map->texture, 1);
    sfSprite_setPosition(map->spr, (sfVector2f) { 0.0f, 0.0f });

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    ship->texture = sfTexture_createFromFile("ship.png", NULL);
    ship->texture_mouv = sfTexture_createFromFile("ship_mouv.png", NULL);
    ship->spr = sfSprite_create();
    sfSprite_setTexture(ship->spr, ship->texture, 1);
    ship->rota = fmod(ship->rota, 360);
    ship->pos.x=400;
    ship->pos.y=400;
    ship->speed=0.1f;
    ship->unspeed=0.03f;
    ship->rotaspeed = 1.5f;
    sfSprite_setPosition(ship->spr, ship->pos);
}

void draw(sfRenderWindow* window, Map* map, Ship* ship) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_drawSprite(window, map->spr, NULL);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setRotation(ship->spr, ship->rota);
    sfSprite_setPosition(ship->spr, ship->pos);
    sfRenderWindow_drawSprite(window, ship->spr, NULL);
}

void mouv(Ship* ship){
    if (sfKeyboard_isKeyPressed(sfKeyUp)) {
        ship->dir.x = cosf(ship->rota * PI / 180);
        ship->dir.y = sinf(ship->rota * PI / 180);
        ship->force.x += ship->speed * ship->dir.x;
        ship->force.y += ship->speed * ship->dir.y;
    }
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
        ship->rota -= ship->rotaspeed;
    }
    if (sfKeyboard_isKeyPressed(sfKeyRight)) {
        ship->rota += ship->rotaspeed;
    }

    ship->pos.x += ship->force.x;
    ship->pos.y += ship->force.y;

    ship->len = sqrt(pow(ship->force.x,2) + pow(ship->force.y,2));
    if(ship->len != 0) {
        ship->normal.x = ship->force.x / ship->len;
        ship->normal.y = ship->force.y / ship->len;

        ship->force.x -= ship->unspeed * ship->normal.x;
        ship->force.y -= ship->unspeed * ship->normal.y;
    }
}