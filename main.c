#include <SFML/Graphics.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.14159
#define max_bullet 250
#define max_roids 2500

// Structure Map
typedef struct Map {
    sfTexture* texture;
    sfTexture* texture_paused;
    sfSprite* spr;
} Map;

//Structure Ship
typedef struct Ship {
    sfVector2f pos, dir, power, normal;
    sfTexture* texture;
    sfTexture* texture_mouv;
    sfSprite* spr;
    sfFloatRect bound;
    int score, pv;
    float rota, speed, unspeed, rotaspeed, len;
}Ship;

//Structure Bullets
typedef struct Bullet {
    sfVector2f pos, dir;
    sfTexture* texture;
    sfSprite* spr;
    sfFloatRect bound;
    float speed, rota, time;
    sfClock* clock;
}   Bullet;

//Structures Asteroids
typedef struct Asteroids {
    sfVector2f pos, dir;
    sfTexture* texture;
    sfSprite* spr;
    sfFloatRect bound;
    float speed, len;
    int border, orient;
}Asteroids;

typedef struct Menu {
    int state;
    //logo
    sfTexture* texture_logo;
    sfSprite* spr_logo;
    // game over
    sfTexture* texture_over;
    sfSprite* spr_over;
    //hearts
    sfTexture* texture_pv1;
    sfTexture* texture_pv2;
    sfTexture* texture_pv3;
    sfSprite* spr_pv;
    //main button
    sfFont* font;
    sfText* text_play;
    sfFloatRect bound_play;
    sfText* text_quit;
    sfFloatRect bound_quit;
    sfText* text_menu;
    sfFloatRect bound_menu;
    //difficulty text
    sfTexture* texture_difficulty;
    sfSprite* spr_difficulty;
    int diff;
    //difficulty button
    sfText* text_difficulty_easy;
    sfFloatRect bound_difficulty_easy;
    sfText* text_difficulty_normal;
    sfFloatRect bound_difficulty_normal;
    sfText* text_difficulty_hard;
    sfFloatRect bound_difficulty_hard;
    sfText* text_difficulty_hell;
    sfFloatRect bound_difficulty_hell;
    //score
    sfText* text_score;

}Menu;

typedef struct Laser {
    sfVector2f pos, dir;
    sfTexture* texture;
    sfSprite* spr;
    sfFloatRect bound;
    sfClock* clock;
}Laser;

void setup(sfImage** icon, sfRenderWindow** window, Map* map, Ship* ship, Bullet* bullet, Asteroids* asteroids, Menu* menu, Laser* laser);
void draw_main_menu(sfRenderWindow* window, Map* map, Menu* menu);
void main_menu(sfRenderWindow* window, Menu* menu);
void draw_selection_menu(sfRenderWindow* window, Map* map, Menu* menu);
void selection_menu(sfRenderWindow* window, Menu* menu);
void draw_game(sfRenderWindow* window, Map* map, Ship* ship, Bullet* bullet, Asteroids* asteroids, Menu* menu, int nb_roids);
void update(Ship* ship, Bullet* bullet, Asteroids* asteroids, Menu* menu, int nb_roids);
void create_bullet(Bullet* bullet, Ship* ship);
void create_laser(Laser* laser, Ship* ship);
void respawn_astero(Asteroids* asteroids, int n);
void respawn_ship(Ship* ship);
void draw_end_menu(sfRenderWindow* window, Map* map, Menu* menu);
void end_menu(sfRenderWindow* window, Menu* menu);

int main() {
    srand(time(NULL));
    sfRenderWindow* window = NULL;
    sfImage* icon = NULL;
    Map map;
    Ship ship;
    Bullet bullet[max_bullet];
    Laser laser;
    Asteroids asteroids[max_roids];
    Menu menu;
    int nb_roids  = 0 ;
    int spacePressCount = 0;
    sfClock* clock = sfClock_create();
    sfTime elapsed = sfClock_getElapsedTime(clock);

    setup(&icon, &window, &map, &ship, bullet, asteroids, &menu, &laser);
    
     while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }
            else if (event.type == sfEvtKeyPressed) {
                if (event.key.code == sfKeySpace) {
                    sfTime currentElapsed = sfClock_getElapsedTime(clock);
                    sfTime deltaTime = sfTime_subtract(currentElapsed, elapsed);
                    elapsed = currentElapsed;
                    create_bullet(bullet, &ship);
                    if (sfTime_asSeconds(deltaTime) > 2.0) {
                        spacePressCount = 0;
                    }
                    spacePressCount++;
                    if (spacePressCount == 15) {
                        create_laser(&laser, &ship);
                    }
                }
            }
        }

        switch (menu.state) {
        case 0: //main menu
            draw_main_menu(window, &map, &menu);
            main_menu(window, &menu);
            break;
        case 1: //difficulty selection
            draw_selection_menu(window, &map, &menu);
            selection_menu(window, &menu);
                if (menu.diff != 0) {
                    switch (menu.diff) {
                        case 1: //easy
                            nb_roids = 10;
                            restart (&ship, asteroids,nb_roids, &menu);
                            break;
                        case 2: //normal
                            nb_roids = 20;
                            restart (&ship, asteroids,nb_roids, &menu);
                            break;
                        case 3: //hard
                            nb_roids = 40;
                            restart (&ship, asteroids,nb_roids, &menu);
                            break;
                        case 4 : //hell
                            nb_roids = 2500;
                            restart (&ship, asteroids,nb_roids, &menu);
                            break;
                    }
                }
            break;
        case 2: //game
            draw_game(window, &map, &ship, bullet, asteroids, &menu,nb_roids);
            update(&ship, bullet, asteroids, &menu,nb_roids);
            break;
        case 3: //end screen
            draw_end_menu(window, &map, &menu);
            end_menu(window, &menu);
            break;
        case 4: //pause
            break;
        }

    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||Destroy||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    sfSprite_destroy(ship.spr);
    sfTexture_destroy(ship.texture);
    sfTexture_destroy(ship.texture_mouv);
    for (int u = 0; u < max_bullet; u++) {
        sfSprite_destroy(bullet[u].spr);
        sfTexture_destroy(bullet[u].texture);
        if (bullet[u].clock != NULL) {
            sfClock_destroy(bullet[u].clock);
            bullet[u].clock = NULL;
        }
    }
    for (int n = 0; n < nb_roids; n++) {
        sfSprite_destroy(asteroids[n].spr);
        sfTexture_destroy(asteroids[n].texture);
    }
    sfSprite_destroy(menu.spr_logo);
    sfSprite_destroy(menu.spr_pv);
    sfTexture_destroy(menu.texture_logo);
    sfTexture_destroy(menu.texture_pv1);
    sfTexture_destroy(menu.texture_pv2);
    sfTexture_destroy(menu.texture_pv3);
    sfTexture_destroy(map.texture);
    sfSprite_destroy(map.spr);
    sfRenderWindow_destroy(window);
    sfImage_destroy(icon);

    return 0;
}

void setup(sfImage** icon, sfRenderWindow** window, Map* map, Ship* ship, Bullet* bullet, Asteroids* asteroids, Menu* menu, Laser* laser) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    *icon = sfImage_createFromFile("ship_icon.png");
    sfVideoMode mode = { 1080, 1080, 32 };
    *window = sfRenderWindow_create(mode, "Staroids", sfTitlebar | sfClose, NULL);
    sfWindow_setIcon(*window, 65, 65, sfImage_getPixelsPtr(*icon));

    sfRenderWindow_setFramerateLimit(*window, 60);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    map->texture = sfTexture_createFromFile("space_background.png", NULL);
    map->texture_paused = sfTexture_createFromFile("space_background2.png", NULL);
    map->spr = sfSprite_create();
    sfSprite_setTexture(map->spr, map->texture, 1);
    sfSprite_setPosition(map->spr, (sfVector2f) { 0.0f, 0.0f });

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||Init UI||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    menu->state = 0;
    menu->diff = 0;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Logo~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    menu->texture_logo = sfTexture_createFromFile("logo.png", NULL);
    menu->spr_logo = sfSprite_create();
    sfSprite_setTexture(menu->spr_logo, menu->texture_logo, 1);
    sfSprite_setPosition(menu->spr_logo, (sfVector2f) { 0.0f, 0.0f });

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Hearts~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    menu->texture_pv1 = sfTexture_createFromFile("1pv.png", NULL);
    menu->texture_pv2 = sfTexture_createFromFile("2pv.png", NULL);
    menu->texture_pv3 = sfTexture_createFromFile("3pv.png", NULL);
    menu->spr_pv = sfSprite_create();
    sfSprite_setPosition(menu->spr_pv, (sfVector2f) { 0.0f, 48.0f });
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Game over~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    menu->texture_over = sfTexture_createFromFile("game_over.png", NULL);
    menu->spr_over = sfSprite_create();
    sfSprite_setTexture(menu->spr_over, menu->texture_over, 1);
    sfSprite_setPosition(menu->spr_over, (sfVector2f) { 0.0f, 0.0f });

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Difficulty~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    menu->texture_difficulty = sfTexture_createFromFile("difficulty.png", NULL);
    menu->spr_difficulty = sfSprite_create();
    sfSprite_setTexture(menu->spr_difficulty, menu->texture_difficulty, 1);
    sfSprite_setPosition(menu->spr_difficulty, (sfVector2f) { 0.0f, 0.0f });

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Buttons~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    menu->font = sfFont_createFromFile("font/alagard.ttf");
    menu->text_play = sfText_create();
    sfText_setFont(menu->text_play, menu->font);
    sfText_setString(menu->text_play, "PLAY !");
    sfText_setCharacterSize(menu->text_play, 72);
    sfText_setPosition(menu->text_play, (sfVector2f) { 435, 520 });
    sfText_setColor(menu->text_play, sfWhite);

    menu->text_quit = sfText_create();
    sfText_setFont(menu->text_quit, menu->font);
    sfText_setString(menu->text_quit, "QUIT !");
    sfText_setCharacterSize(menu->text_quit, 72);
    sfText_setPosition(menu->text_quit, (sfVector2f) { 435, 680 });
    sfText_setColor(menu->text_quit, sfWhite);

    menu->text_menu = sfText_create();
    sfText_setFont(menu->text_menu, menu->font);
    sfText_setString(menu->text_menu, "Main menu");
    sfText_setCharacterSize(menu->text_menu, 72);
    sfText_setPosition(menu->text_menu, (sfVector2f) { 350, 520 });
    sfText_setColor(menu->text_menu, sfWhite);

    menu->text_difficulty_easy = sfText_create();
    sfText_setFont(menu->text_difficulty_easy, menu->font);
    sfText_setString(menu->text_difficulty_easy, "Easy");
    sfText_setCharacterSize(menu->text_difficulty_easy, 48);
    sfText_setPosition(menu->text_difficulty_easy, (sfVector2f) { 64, 685 });
    sfText_setColor(menu->text_difficulty_easy, sfWhite);

    menu->text_difficulty_normal = sfText_create();
    sfText_setFont(menu->text_difficulty_normal, menu->font);
    sfText_setString(menu->text_difficulty_normal, "Normal");
    sfText_setCharacterSize(menu->text_difficulty_normal, 48);
    sfText_setPosition(menu->text_difficulty_normal, (sfVector2f) { 316, 685 });
    sfText_setColor(menu->text_difficulty_normal, sfWhite);

    menu->text_difficulty_hard = sfText_create();
    sfText_setFont(menu->text_difficulty_hard, menu->font);
    sfText_setString(menu->text_difficulty_hard, "Hard");
    sfText_setCharacterSize(menu->text_difficulty_hard, 48);
    sfText_setPosition(menu->text_difficulty_hard, (sfVector2f) { 648, 685 });
    sfText_setColor(menu->text_difficulty_hard, sfWhite);

    menu->text_difficulty_hell = sfText_create();
    sfText_setFont(menu->text_difficulty_hell, menu->font);
    sfText_setString(menu->text_difficulty_hell, "Hell");
    sfText_setCharacterSize(menu->text_difficulty_hell, 48);
    sfText_setPosition(menu->text_difficulty_hell, (sfVector2f) { 908, 685 });
    sfText_setColor(menu->text_difficulty_hell, sfWhite);

    menu->text_score = sfText_create();
    sfText_setFont(menu->text_score, menu->font);
    sfText_setString(menu->text_score, "Score :");
    sfText_setCharacterSize(menu->text_score, 48);
    sfText_setPosition(menu->text_score, (sfVector2f) { 0, -5 });
    sfText_setColor(menu->text_score, sfWhite);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    ship->texture = sfTexture_createFromFile("ship.png", NULL);
    ship->texture_mouv = sfTexture_createFromFile("ship_mouv.png", NULL);
    ship->spr = sfSprite_create();
    sfSprite_setTexture(ship->spr, ship->texture, 1);
    sfSprite_setOrigin(ship->spr, (sfVector2f) { 33, 33 });
    sfSprite_setPosition(ship->spr, ship->pos);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Bullets~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    for (int u = 0; u < max_bullet; u++) {
        bullet[u].texture = sfTexture_createFromFile("bullet.png", NULL);
        bullet[u].spr = sfSprite_create();
        sfSprite_setTexture(bullet[u].spr, bullet[u].texture, 1);
        sfSprite_setOrigin(bullet[u].spr, (sfVector2f) { 12, 2 });
        bullet[u].clock = NULL;
    }
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Laser~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    laser->texture = sfTexture_createFromFile("bullet.png", NULL);
    laser->spr = sfSprite_create();
    sfSprite_setTexture(laser->spr, laser->texture, 1);
    sfSprite_setOrigin(laser->.spr, (sfVector2f) { 12, 2 });
    laser->clock = NULL;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Init Asteroids~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    asteroids->speed = 3.0f;
}

void draw_main_menu(sfRenderWindow* window, Map* map, Menu* menu) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setTexture(map->spr, map->texture_paused, 1);
    sfRenderWindow_drawSprite(window, map->spr, NULL);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_drawSprite(window, menu->spr_logo, NULL);
    sfRenderWindow_drawText(window, menu->text_play, NULL);
    sfRenderWindow_drawText(window, menu->text_quit, NULL);
}

void main_menu(sfRenderWindow* window, Menu* menu) {
    menu->bound_play = sfText_getGlobalBounds(menu->text_play);
    menu->bound_quit = sfText_getGlobalBounds(menu->text_quit);
    sfFloatRect mouse = (sfFloatRect){ sfMouse_getPosition(window).x, sfMouse_getPosition(window).y, 1, 1 };

    if (sfFloatRect_intersects(&menu->bound_play, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            menu->state = 1;
        }
    }

    if (sfFloatRect_intersects(&menu->bound_quit, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            sfRenderWindow_close(window);
        }
    }
}

void draw_selection_menu(sfRenderWindow* window, Map* map, Menu* menu) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setTexture(map->spr, map->texture_paused, 1);
    sfRenderWindow_drawSprite(window, map->spr, NULL);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_drawSprite(window, menu->spr_difficulty, NULL);
    sfRenderWindow_drawText(window, menu->text_difficulty_easy, NULL);
    sfRenderWindow_drawText(window, menu->text_difficulty_normal, NULL);
    sfRenderWindow_drawText(window, menu->text_difficulty_hard, NULL);
    sfRenderWindow_drawText(window, menu->text_difficulty_hell, NULL);
}

void selection_menu(sfRenderWindow* window, Menu* menu) {
    menu->bound_difficulty_easy = sfText_getGlobalBounds(menu->text_difficulty_easy);
    menu->bound_difficulty_normal = sfText_getGlobalBounds(menu->text_difficulty_normal);
    menu->bound_difficulty_hard = sfText_getGlobalBounds(menu->text_difficulty_hard);
    menu->bound_difficulty_hell = sfText_getGlobalBounds(menu->text_difficulty_hell);
    sfFloatRect mouse = (sfFloatRect){ sfMouse_getPosition(window).x, sfMouse_getPosition(window).y, 1, 1 };

    if (sfFloatRect_intersects(&menu->bound_difficulty_easy, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            menu->diff = 1;
        }
    }

    if (sfFloatRect_intersects(&menu->bound_difficulty_normal, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            menu->diff = 2;
        }
    }

    if (sfFloatRect_intersects(&menu->bound_difficulty_hard, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            menu->diff = 3;
        }
    }

    if (sfFloatRect_intersects(&menu->bound_difficulty_hell, &mouse, NULL)) {
        sfText_setColor(menu->text_difficulty_hell, sfRed);
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            menu->diff = 4;
        }
    } else {
        sfText_setColor(menu->text_difficulty_hell, sfWhite);
    }
}

void restart (Ship* ship, Asteroids* asteroids, int nb_roids, Menu* menu) {
    ship->pv = 3;
    ship->score = 0;
    ship->len = 0;
    ship->rota = -90;
    ship->pos = (sfVector2f){ 540,900 };
    ship->power = (sfVector2f){ 0,0 };
    ship->speed = 0.1f;
    ship->unspeed = 0.05f;
    ship->rotaspeed = 2.5f;

    for (int n = 0; n < nb_roids; n++) {
        asteroids[n].texture = sfTexture_createFromFile("asteroids.png", NULL);
        asteroids[n].spr = sfSprite_create();
        sfSprite_setTexture(asteroids[n].spr, asteroids[n].texture, 1);
        sfSprite_setOrigin(asteroids[n].spr, (sfVector2f) { 33, 33 });

        asteroids[n].border = rand() % 4;
        switch (asteroids[n].border) {
        case 0: //top
            asteroids[n].pos.x = rand() % 1080 + 1;
            asteroids[n].pos.y = 0;
            asteroids[n].dir.y = ((float)rand() / 180) - 0.5f;
            asteroids[n].orient = rand() % 2;
            switch (asteroids[n].orient) {
            case 0:
                asteroids[n].dir.x = ((float)rand() / 360) - 0.5f;
                break;
            case 1:
                asteroids[n].dir.x = -((float)rand() / 360) - 0.5f;
                break;
            }
            break;
        case 1: //right
            asteroids[n].pos.x = 1080;
            asteroids[n].pos.y = rand() % 1080 + 1;
            asteroids[n].dir.x = -((float)rand() / 180) - 0.5f;
            switch (asteroids[n].orient) {
            case 0:
                asteroids[n].dir.y = ((float)rand() / 360) - 0.5f;
                break;
            case 1:
                asteroids[n].dir.y = -((float)rand() / 360) - 0.5f;
                break;
            }
            break;
        case 2: //bottom
            asteroids[n].pos.x = rand() % 1080 + 1;
            asteroids[n].pos.y = 1080;
            asteroids[n].dir.y = -((float)rand() / 180) - 0.5f;
            asteroids[n].orient = rand() % 2;
            switch (asteroids[n].orient) {
            case 0:
                asteroids[n].dir.x = ((float)rand() / 360) - 0.5f;
                break;
            case 1:
                asteroids[n].dir.x = -((float)rand() / 360) - 0.5f;
                break;
            }
            break;
        case 3: // left
            asteroids[n].pos.x = 0;
            asteroids[n].pos.y = rand() % 1080 + 1;
            asteroids[n].dir.x = ((float)rand() / 180) - 0.5f;
            asteroids[n].orient = rand() % 2;
            switch (asteroids[n].orient) {
            case 0:
                asteroids[n].dir.y = ((float)rand() / 360) - 0.5f;
                break;
            case 1:
                asteroids[n].dir.y = -((float)rand() / 360) - 0.5f;
                break;
            }
            break;
        }

        asteroids[n].len = sqrt(pow(asteroids[n].dir.x, 2) + pow(asteroids[n].dir.y, 2));
        if (asteroids[n].len != 0) {
            asteroids[n].dir.x /= asteroids[n].len;
            asteroids[n].dir.y /= asteroids[n].len;
        }

        sfSprite_setPosition(asteroids[n].spr, asteroids[n].pos);
    }
    menu->state=2;
}

void draw_game(sfRenderWindow* window, Map* map, Ship* ship, Bullet* bullet, Asteroids* asteroids, Menu* menu,int nb_roids) {
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setTexture(map->spr, map->texture, 1);
    sfRenderWindow_drawSprite(window, map->spr, NULL);

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    char scorechar[20];
    snprintf(scorechar, 20, "Score : %d", ship->score);
    sfText_setString(menu->text_score, scorechar);
    sfRenderWindow_drawSprite(window, menu->text_score, NULL);
    
    switch (ship->pv) {
    case 3:
        sfSprite_setTexture(menu->spr_pv, menu->texture_pv3, 1);
        sfRenderWindow_drawSprite(window, menu->spr_pv, NULL);
        break;
    case 2:
        sfSprite_setTexture(menu->spr_pv, menu->texture_pv2, 1);
        sfRenderWindow_drawSprite(window, menu->spr_pv, NULL);
        break;
    case 1:
        sfSprite_setTexture(menu->spr_pv, menu->texture_pv1, 1);
        sfRenderWindow_drawSprite(window, menu->spr_pv, NULL);
        break;
    default:
        break;
    }




    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Asteroids~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    for (int n = 0; n < nb_roids; n++) {
        sfSprite_setPosition(asteroids[n].spr, asteroids[n].pos);
        sfRenderWindow_drawSprite(window, asteroids[n].spr, NULL);
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bullet~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    for (int u = 0; u < max_bullet; u++) {
        if (bullet[u].clock != NULL) {
            sfSprite_setRotation(bullet[u].spr, bullet[u].rota);
            sfSprite_setPosition(bullet[u].spr, bullet[u].pos);
            sfRenderWindow_drawSprite(window, bullet[u].spr, NULL);
        }
    }

    if (laser->clock != NULL) {
        sfSprite_setRotation(laser->spr, laser->rota);
            sfSprite_setPosition(laser->.spr, laser->wpos);
            sfRenderWindow_drawSprite(window, laser->spr, NULL);
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setRotation(ship->spr, ship->rota);
    sfSprite_setPosition(ship->spr, ship->pos);
    sfRenderWindow_drawSprite(window, ship->spr, NULL);
}

void update(Ship* ship, Bullet* bullet, Asteroids* asteroids, Menu* menu, int nb_roids) {
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Ship~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    if (sfKeyboard_isKeyPressed(sfKeyUp)) {
        ship->dir.x = cosf(ship->rota * PI / 180);
        ship->dir.y = sinf(ship->rota * PI / 180);
        ship->power.x += ship->speed * ship->dir.x;
        ship->power.y += ship->speed * ship->dir.y;
        sfSprite_setTexture(ship->spr, ship->texture_mouv, 1);
    }
    else {
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

    ship->len = sqrt(pow(ship->power.x, 2) + pow(ship->power.y, 2));
    if (ship->len != 0) {
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Bullet~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    for (int u = 0; u < max_bullet; u++) {
        if (bullet[u].clock != NULL) {
            sfTime elapsed_bul = sfClock_getElapsedTime(bullet[u].clock);
            if (sfTime_asSeconds(elapsed_bul) < bullet[u].time) {
                bullet[u].pos.x += bullet[u].speed * bullet[u].dir.x;
                bullet[u].pos.y += bullet[u].speed * bullet[u].dir.y;
                if (bullet[u].pos.x >= 1120) {
                    bullet[u].pos.x = 30;
                }
                if (bullet[u].pos.x <= -30) {
                    bullet[u].pos.x = 1050;
                }
                if (bullet[u].pos.y >= 1120) {
                    bullet[u].pos.y = 30;
                }
                if (bullet[u].pos.y <= -30) {
                    bullet[u].pos.y = 1050;
                }
            }
            else {
                sfClock_destroy(bullet[u].clock);
                bullet[u].clock = NULL;
            }
        }
    }
    
    if (laser->clock != NULL) {
            sfTime elapsed_las = sfClock_getElapsedTime(laser->clock);
            if (sfTime_asSeconds(elapsed_las) < 3) {
                laser->pos = laser->dir;
            }
            else {
                sfClock_destroy(laser->clock);
                laser->clock = NULL;
            }
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Asteroid~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    for (int n = 0; n < nb_roids; n++) {
        asteroids[n].pos.x += asteroids->speed * asteroids[n].dir.x;
        asteroids[n].pos.y += asteroids->speed * asteroids[n].dir.y;

        if ((asteroids[n].pos.x <= -10) || (asteroids[n].pos.x >= 1090) || (asteroids[n].pos.y <= -10) || (asteroids[n].pos.y >= 1090)) {
            respawn_astero(asteroids, n);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Collisions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    ship->bound = sfSprite_getGlobalBounds(ship->spr);
    ship->bound.height -= 100;
    ship->bound.top += 40;
    ship->bound.width -= 100;
    ship->bound.left += 50;

    for (int n = 0; n < nb_roids; n++) {
        asteroids[n].bound = sfSprite_getGlobalBounds(asteroids[n].spr);
        if (sfFloatRect_intersects(&ship->bound, &asteroids[n].bound, NULL) == sfTrue) {
            respawn_astero(asteroids, n);
            if (ship->pv > 0) {
                respawn_ship(ship);
            }
            else {
                menu->state = 3;
            }
        }
    }

    for (int u = 0; u < max_bullet; u++) {
        if (bullet[u].clock != NULL) {
            bullet[u].bound = sfSprite_getGlobalBounds(bullet[u].spr);

            for (int n = 0; n < nb_roids; n++) {
                asteroids[n].bound = sfSprite_getGlobalBounds(asteroids[n].spr);
                if (sfFloatRect_intersects(&bullet[u].bound, &asteroids[n].bound, NULL) == sfTrue) {
                    ship->score+=25;
                    respawn_astero(asteroids, n);
                    bullet[u].clock = NULL;
                }
            }
        }
    }


}

void create_bullet(Bullet* bullet, Ship* ship) {
    for (int u = 0; u < max_bullet; u++) {
        if (bullet[u].clock == NULL) {
            bullet[u].time = 7.0f;
            bullet[u].pos = ship->pos;
            bullet[u].dir = (sfVector2f){ cosf(ship->rota * PI / 180), sinf(ship->rota * PI / 180) };
            bullet[u].rota = ship->rota;
            bullet[u].speed = 7.0f;
            bullet[u].clock = sfClock_create();
            break;
        }
    }
}

void create_laser(Laser* laser, Ship* ship) {

}

void respawn_astero(Asteroids* asteroids, int n) {
    asteroids[n].border = rand() % 4;
    switch (asteroids[n].border) {
    case 0: //top
        asteroids[n].pos.x = rand() % 1080 + 1;
        asteroids[n].pos.y = 0;
        asteroids[n].dir.y = ((float)rand() / 180) - 0.5f;
        asteroids[n].orient = rand() % 2;
        switch (asteroids[n].orient) {
        case 0:
            asteroids[n].dir.x = ((float)rand() / 360) - 0.5f;
            break;
        case 1:
            asteroids[n].dir.x = -((float)rand() / 360) - 0.5f;
            break;
        }
        break;
    case 1: //right
        asteroids[n].pos.x = 1080;
        asteroids[n].pos.y = rand() % 1080 + 1;
        asteroids[n].dir.x = -((float)rand() / 180) - 0.5f;
        switch (asteroids[n].orient) {
        case 0:
            asteroids[n].dir.y = ((float)rand() / 360) - 0.5f;
            break;
        case 1:
            asteroids[n].dir.y = -((float)rand() / 360) - 0.5f;
            break;
        }
        break;
    case 2: //bottom
        asteroids[n].pos.x = rand() % 1080 + 1;
        asteroids[n].pos.y = 1080;
        asteroids[n].dir.y = -((float)rand() / 180) - 0.5f;
        asteroids[n].orient = rand() % 2;
        switch (asteroids[n].orient) {
        case 0:
            asteroids[n].dir.x = ((float)rand() / 360) - 0.5f;
            break;
        case 1:
            asteroids[n].dir.x = -((float)rand() / 360) - 0.5f;
            break;
        }
        break;
    case 3: // left
        asteroids[n].pos.x = 0;
        asteroids[n].pos.y = rand() % 1080 + 1;
        asteroids[n].dir.x = ((float)rand() / 180) - 0.5f;
        asteroids[n].orient = rand() % 2;
        switch (asteroids[n].orient) {
        case 0:
            asteroids[n].dir.y = ((float)rand() / 360) - 0.5f;
            break;
        case 1:
            asteroids[n].dir.y = -((float)rand() / 360) - 0.5f;
            break;
        }
        break;
    }

    asteroids[n].len = sqrt(pow(asteroids[n].dir.x, 2) + pow(asteroids[n].dir.y, 2));
    if (asteroids[n].len != 0) {
        asteroids[n].dir.x /= asteroids[n].len;
        asteroids[n].dir.y /= asteroids[n].len;
    }

    sfSprite_setPosition(asteroids[n].spr, asteroids[n].pos);
}

void respawn_ship(Ship* ship) {
    ship->pv -= 1;
    ship->len = 0;
    ship->rota = -90;
    ship->pos = (sfVector2f){ 540,900 };
    ship->power = (sfVector2f){ 0,0 };
    ship->speed = 0.1f;
    ship->unspeed = 0.05f;
    ship->rotaspeed = 2.5f;
}

void draw_end_menu(sfRenderWindow* window, Map* map, Menu* menu) {
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Window~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_display(window);
    sfRenderWindow_clear(window, sfBlack);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Map~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfSprite_setTexture(map->spr, map->texture_paused, 1);
    sfRenderWindow_drawSprite(window, map->spr, NULL);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UI~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    sfRenderWindow_drawSprite(window, menu->spr_over, NULL);
    sfRenderWindow_drawText(window, menu->text_menu, NULL);
    sfRenderWindow_drawText(window, menu->text_quit, NULL);
    snprintf(scorechar, 20, "Score : %d", ship->score);
    sfText_setString(menu->text_score, scorechar);
    sfRenderWindow_drawSprite(window, menu->text_score, NULL);
}

void end_menu(sfRenderWindow* window, Menu* menu) {
    menu->bound_menu = sfText_getGlobalBounds(menu->text_menu);
    menu->bound_quit = sfText_getGlobalBounds(menu->text_quit);
    sfFloatRect mouse = (sfFloatRect){ sfMouse_getPosition(window).x, sfMouse_getPosition(window).y, 1, 1 };

    if (sfFloatRect_intersects(&menu->bound_menu, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            menu->diff = 0;
            menu->state = 0;
        }
    }

    if (sfFloatRect_intersects(&menu->bound_quit, &mouse, NULL)) {
        if (sfMouse_isButtonPressed(sfMouseLeft)) {
            sfRenderWindow_close(window);
        }
    }

}