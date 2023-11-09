#include <SFML/Graphics.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

// Structure Map
typedef struct Map {
    sfTexture* texture;
    sfSprite* spr;
} Map;

//Structure Ship
typedef struct Ship{
    sfVector2f pos;
    sfTexture* texture;
    sfTexture* texture_mouv;
    sfSprite* spr;
    float rota;
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

//Structures Other
typedef struct Misc{
    int score
}Misc;


void setup(sfImage** icon, sfRenderWindow** window, Map* map,Ship* ship);
void draw(sfRenderWindow* window, Map* map);

int main() {
    srand(time(NULL));
    sfRenderWindow* window = NULL;
    sfImage* icon = NULL;
    Map map;
    Ship ship;
    Misc misc;


    setup(&icon, &window, &map,&ship, &clock);

    sfText* score = sfText_create();
    sfText_setFont(score, font);
    sfText_scale(score, (sfVector2f) { 1.5, 1.5 });


    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
        }

        char str_score[13];
        snprintf(str_score, 13, "Score: %d", misc.score);
        sfText_setString(score, str_score);
        draw(window, &map);
        sfRenderWindow_drawText(window, score, NULL);

    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||Destroy||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_destroy(map.spr);
    sfTexture_destroy(map.texture);
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
    map->texture = sfTexture_createFromFile("Space_Background.png", NULL);
    map->spr = sfSprite_create();
    sfSprite_setTexture(map->spr, map->texture, 1);
    sfSprite_setPosition(map->spr, (sfVector2f) { 0.0f, 0.0f });

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    ship->texture = sfTexture_createFromFile("Ship.png", NULL);
    ship->texture_mouv = sfTexture_createFromFile("Ship_mouv.png", NULL);
    ship->spr = sfSprite_create();
    sfSprite_setTexture(map->spr, map->texture, 1);
    sfSprite_setPosition(map->spr, (sfVector2f) { 0.0f, 0.0f });

}

void draw(sfRenderWindow* window, Map* map) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_drawSprite(window, map->spr, NULL);
}