#include <SFML/Graphics.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159
#define max_bullet 5

// Structure Map
typedef struct Map {
    sfTexture* texture;
    sfSprite* spr;
} Map;

//Structure Ship
typedef struct Ship{
    sfVector2f pos,dir,power,normal;
    sfTexture* texture;
    sfTexture* texture_mouv;
    sfSprite* spr;
    float rota, speed,unspeed,rotaspeed,len;
}Ship;

//Structure Bullet
typedef struct Bullet{
    sfVector2f pos,dir;
    sfTexture* texture;
    sfSprite* spr;
    float speed, dura, time,rota;
    bool active;
    int nb;
    sfClock* clock;
    sfTime elapsed;
}Bullet;

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



void setup(sfImage** icon, sfRenderWindow** window, Map* map, Ship* ship, Bullet bullet[]);
void update(Ship* ship, Bullet bullet[]);
void draw(sfRenderWindow* window, Map* map, Ship* ship, Bullet bullet[]);

int main() {
    srand(time(NULL));
    sfRenderWindow* window = NULL;
    sfImage* icon = NULL;
    Map map;
    Ship ship;
    Bullet bullet[max_bullet];

    setup(&icon, &window, &map, &ship, bullet);

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        draw(window, &map, &ship, bullet);
        update(&ship, bullet);
    }

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||Destroy||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    sfSprite_destroy(ship.spr);
    sfTexture_destroy(ship.texture);
    sfTexture_destroy(ship.texture_mouv);
    for (int n = 0; n < max_bullet; n++) {
        sfSprite_destroy(bullet[n].spr);
    }
    sfTexture_destroy(map.texture);
    sfSprite_destroy(map.spr);
    sfRenderWindow_destroy(window);
    sfImage_destroy(icon);

    return 0;
}

void setup(sfImage** icon, sfRenderWindow** window, Map* map, Ship* ship, Bullet bullet[]) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    *icon = sfImage_createFromFile("ship_icon.png");
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
    sfSprite_setOrigin(ship->spr, (sfVector2f){33,33});
    ship->len=0;
    ship->rota = -90;
    ship->pos = (sfVector2f){540,900};
    ship->power = (sfVector2f){0,0};
    ship->speed = 0.1f;
    ship->unspeed = 0.05f;
    ship->rotaspeed = 2.5f;
    sfSprite_setPosition(ship->spr, ship->pos);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Bullet~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    bullet->texture = sfTexture_createFromFile("bullet.png", NULL);
    bullet->speed = 15.0f;
    bullet->dura = 15.0f;
    bullet->nb = 0;
    bullet->clock = sfClock_create();
    bullet->elapsed = sfClock_getElapsedTime(bullet->clock);

   for (int n = 0; n < max_bullet; n++) {
        bullet[n].texture = sfTexture_createFromFile("bullet.png", NULL);
        bullet[n].spr = sfSprite_create();
        bullet[n].pos = (sfVector2f){0,0};
        bullet[n].active = false;
        sfSprite_setTexture(bullet[n].spr, bullet[n].texture, 1);
        sfSprite_setOrigin(bullet[n].spr, (sfVector2f){33,33});
        bullet[n].time = 0.0f;
        bullet[n].rota = 0.0f;
    }

    
}

void update(Ship* ship, Bullet bullet[]) {
    if (sfKeyboard_isKeyPressed(sfKeyUp)) {
        ship->dir.x = cosf(ship->rota * PI / 180);
        ship->dir.y = sinf(ship->rota * PI / 180);
        ship->power.x += ship->speed * ship->dir.x;
        ship->power.y += ship->speed * ship->dir.y;
        sfSprite_setTexture(ship->spr, ship->texture_mouv, 1);
    } else {
        sfSprite_setTexture(ship->spr, ship->texture, 1);
    }
    if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
        ship->rota -= ship->rotaspeed;
    }
    if (sfKeyboard_isKeyPressed(sfKeyRight)) {
        ship->rota += ship->rotaspeed;
    }

    ship->pos.x += ship->power.x;
    ship->pos.y += ship->power.y;

    ship->len = sqrt(pow(ship->power.x,2) + pow(ship->power.y,2));
    if(ship->len != 0) {
        ship->normal.x = ship->power.x / ship->len;
        ship->normal.y = ship->power.y / ship->len;

        ship->power.x -= ship->unspeed * ship->normal.x;
        ship->power.y -= ship->unspeed * ship->normal.y;
    }

    if (ship->pos.x >= 1120) {
        ship->pos.x = 30;
    }
    if (ship->pos.x <= -30) {
        ship->pos.x = 1050;
    }
    if (ship->pos.y >= 1120) {
        ship->pos.y = 30;
    }
    if (ship->pos.y <= -30) {
        ship->pos.y = 1050;
    }

    if (sfKeyboard_isKeyPressed(sfKeySpace)) {
        for (int n = 0; n < max_bullet; n++) {
            if (!bullet[n].active) {
                bullet->nb+=1;
                bullet[n].pos = ship->pos;
                bullet[n].dir = ship->dir;
                bullet[n].rota = ship->rota;
                bullet[n].active = true;
                break;
            }
        }
    }
    if (bullet->nb != 0) {
        for (int n = 0; n < max_bullet; n++) {
            if (bullet[n].active) {
                bullet[n].elapsed = sfClock_getElapsedTime(bullet->clock);
                float Time = sfTime_asSeconds(bullet[n].elapsed);
                if (Time - bullet[n].time > bullet[n].dura) {
                    bullet[n].active = false;
                    bullet[n].time = 0.0f;
                } else {
                    bullet[n].pos.x += bullet[n].speed * bullet[n].dir.x;
                    bullet[n].pos.y += bullet[n].speed * bullet[n].dir.y;
                }
            }
        }
    }
}

void draw(sfRenderWindow* window, Map* map, Ship* ship, Bullet bullet[]) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_drawSprite(window, map->spr, NULL);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setRotation(ship->spr, ship->rota);
    sfSprite_setPosition(ship->spr, ship->pos);
    sfRenderWindow_drawSprite(window, ship->spr, NULL);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bullet~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setRotation(ship->spr, ship->rota);
    for (int n = 0; n < max_bullet; n++) {
        if (bullet[n].active) {
            sfSprite_setRotation(bullet[n].spr, bullet[n].rota);
            sfSprite_setPosition(bullet[n].spr, bullet[n].pos);
            sfRenderWindow_drawSprite(window, bullet[n].spr, NULL);
        }
    }
}
