// This program was written by Yuming Li
// on Sun Nov 10, 2024
//
// Version 1.0.0: Assignment released.
//
// A fun dungeon game :) 
// Get to design a map of connected dungeons filled with monsters, items, 
// and a final boss at the end to defeat. Players will play as either a 
// Fighter or a Wizard, using special skills and stats to defeat these 
// monsters and beat the game!
//
// Program TO-DOs:
// 1. Design a map of dungeons connected using linked lists
// 2. Add monsters, items and a final boss to the corresponding items
// 3. Finish the implementation of player movements and monster (including 
//    boss) attacks
// 4. Implement the interation with the items in dungeons
// 5. Check ways the game ends, which will lead to different ending words

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cs_dungeon.h"

// Add any extra #includes your code needs here.
#include <stdbool.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
//                                 Constants                                  //
////////////////////////////////////////////////////////////////////////////////

// Add your own #defines here:
#define PLAYER 1
#define NO_PLAYER 0
#define PASSED 100
#define NOT_PASSED 0
#define MOVE 1
#define NOT_USED 1
#define USED 0
#define TELEPORTED 0
#define NOT_TELEPORTED 1
#define ATTACKED 1
#define NOT_ATTACKED 0
#define INCREASED 1
#define NOT_INCREASED 0
#define DEFEATED 1
#define NOT_DEFEATED 0
#define INITIAL_PLAYER_POINTS 0
#define SAME_STRING 0
#define INITIAL_FIGHTER_HEALTH 30
#define INITIAL_FIGHTER_SHIELD 2
#define INITIAL_FIGHTER_DAMAGE 8
#define INITIAL_FIGHTER_MODIFIER 0.9
#define INITIAL_WIZARD_HEALTH 15
#define INITIAL_WIZARD_SHIELD 0
#define INITIAL_WIZARD_DAMAGE 7
#define INITIAL_WIZARD_MODIFIER 1.5
#define INITIAL_POSITION 1
#define INITIAL_BOSS_HEALTH 35
#define INITIAL_BOSS_DAMAGE 10
#define INITIAL_BOSS_POINTS 20
#define INITIAL_LENGTH 0
#define NO_MONSTER 0
#define BOSS_NO_HEALTH 0
#define PLAYER_NO_HEALTH 0
#define FIGHTER_CLASS_POWER 1.5
#define INITIAL_INDEX 1
#define USING_MAGIC 1.0
#define BOSS_INCREASE_POWER 1.5

// Your enums go here:

////////////////////////////////////////////////////////////////////////////////
//                            Struct Definitions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided structs:

// Stores information about a dungeon
struct dungeon {
    // name of the dungeon
    char name[MAX_STR_LEN];

    // what type of monster it contains (enum described in cs_dungeon.h)
    enum monster_type monster;

    // the amount of monsters it contains
    int num_monsters;

    // the initial amount of monsters it contains
    int original_num_monsters;

    // a pointer to the boss monster (if it contains one) or NULL
    struct boss *boss;

    // a pointer to the first item it contains or NULL
    struct item *items;

    // an int indicating if the player is inside the dungeon
    //      1: The player is inside the dungeon
    //      0: The player is not inside the dungeon
    int contains_player;

    // a pointer to the next dungeon in the map or NULL
    struct dungeon *next;

    // an integer indicating whether this dungeon has been teleported to
    // or has been a start to start the teleportation cycle
    int is_teleported;
};

// Stores information about an item
struct item {
    // what type of item (enum described in cs_dungeon.h)
    enum item_type type;

    // how many points the item is worth
    int points;

    // a pointer to the next item in the list or NULL
    struct item *next;
};

// Stores information about the player who is exploring the dungeon map
struct player {
    // name of the player
    char name[MAX_STR_LEN];

    // what type of class they are playing (Figher or Wizard)
    char class_type[MAX_STR_LEN];

    // how many health points they have
    int health_points;

    // how much shield power they have
    int shield_power;

    // how much damage they have
    int damage;

    // what their magic modifier is when they make magical attacks
    double magic_modifier;

    // a pointer to the first item in their inventory or NULL
    struct item *inventory;

    // the number of points they have collected over the course of the game
    int points;

    // indicate whether the player has used the class power or not;
    int use_class_power;
};


// Stores information about the boss-level monster initially placed in the last
// dungeon in the map
struct boss {
    // how many health points they have
    int health_points;

    // how much damage they have
    int damage;

    // how many points they are worth when defeated
    int points;

    // the item type the player is required to have to fight them
    enum item_type required_item;

    // indicates that whether the boss has been activated
    int being_attacted;

    // indicates that whether the boss has increased its damage
    int is_increased_damage;

    // indicates that whether the boss has been defeated
    int is_defeated;
};

// Add any other structs you define here.

////////////////////////////////////////////////////////////////////////////////
//                           Additional Prototypes                            //
////////////////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.
// Stage 1
struct dungeon *create_dungeon(char *name, enum monster_type monster,
                               int num_monsters, int contains_player);

struct boss *create_boss(int health_points, int damage, int points,
                         enum item_type required_item);

int test_dungeon_name(struct map *map, char *name);

int test_monster_type(enum monster_type monster_type);

int test_item_type(enum item_type item_type);

// Stage 2
int test_insert_dungeon(struct map *map, char *name, 
                        enum monster_type monster_type, int num_monsters, 
                        int position);

int get_dungeon_num(struct dungeon *head);

struct dungeon *find_player(struct map *map);

int get_player_dungeon_index(struct map *map);

struct dungeon *move_to_a_dungeon(struct map *map, int index);

struct dungeon *attact_player(struct dungeon *current, struct map *map);

// Stage 3
int check_defeat_all_monsters(struct map *map);

int check_defeat_boss(struct map *map);

int test_add_item(struct map *map, 
                  int dungeon_number,
                  enum item_type type,
                  int points);

int check_same_type_item(struct item *item, enum item_type type);

int find_same_item_index(struct item *item, enum item_type type);

struct item *move_to_an_item(struct item *item, int index);

int find_item_length(struct item *item);

struct item *organize_items_in_order(struct item *items, 
                                     struct item *new_item,
                                     int list_length);

int test_collect_item(struct map *map, int item_number);

void move_item_to_player(struct map *map, int item_number);

int test_use_item(struct map *map, int item_number);

void use_item_effect(struct map *map, struct item *item);

int test_free_map(struct dungeon *current);

void free_dungeon(struct map *map);

void free_items(struct item *items);

// Stage 4
void check_all_dungeon_teleported(struct map *map);

void teleportation(struct map *map);

int find_current_dungeon_index(struct map *map);

int find_target_dungeon_index(int current_index, struct map *map);

void restart_teleportation_cycle(struct map *map);

int test_boss_fight(struct map *map);

struct dungeon *find_boss(struct map *map);

int find_required_item(struct map *map, enum item_type item);

void shuffle_dungeons(struct map *map);

void swap_pair_dungeon(struct map *map, 
                       int entrance_index, 
                       int index1, 
                       int index2);

void boss_fight_or_move(struct map *map);

void boss_attack(struct map *map);

void boss_move(struct map *map);

int get_boss_dungeon_index(struct map *map);

// You need to implement the following functions.
// You can find descriptions of what each function
// should do in dungeon.h

////////////////////////////////////////////////////////////////////////////////
//                             Stage 1 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided Function stubs:

// Creates a new struct map
// 1. Creates a new struct map.
// 2. Copies the name and win_requirement arguments into the corresponding 
//    struct fields.
// 3. Initialises all other fields to a reasonable value.
// 4. Returns a pointer to the newly created struct map.
//
struct map *create_map(char *name, int win_requirement) {
    // TODO: implement this function
    struct map *map = malloc(sizeof(struct map));

    strcpy(map->name, name);
    map->win_requirement = win_requirement;
    map->entrance = NULL;
    map->player = NULL;

    return map;
}

// Creates a new struct player
// 1. Creates a new struct player.
// 2. Copies the name and class_type arguments into the corresponding 
//    struct fields.
// 3. Initialises health_points, shield_power, damage and magic_modifier 
//    based on the chosen class (Fighter or Wizard).
// 4. Initialises all other fields to some reasonable value.
// 5. Returns a pointer to the newly created struct player.
//
struct player *create_player(char *name, char *class_type) {
    // TODO: implement this function
    struct player *player = malloc(sizeof(struct player));

    strcpy(player->name, name);
    strcpy(player->class_type, class_type);
    player->points = INITIAL_PLAYER_POINTS;
    player->inventory = NULL;
    player->use_class_power = NOT_USED;

    if (strcmp(class_type, "Fighter") == SAME_STRING) {
        player->health_points = INITIAL_FIGHTER_HEALTH;
        player->shield_power = INITIAL_FIGHTER_SHIELD;
        player->damage = INITIAL_FIGHTER_DAMAGE;
        player->magic_modifier = INITIAL_FIGHTER_MODIFIER;
    } else if (strcmp(class_type, "Wizard") == SAME_STRING) {
        player->health_points = INITIAL_WIZARD_HEALTH;
        player->shield_power = INITIAL_WIZARD_SHIELD;
        player->damage = INITIAL_WIZARD_DAMAGE;
        player->magic_modifier = INITIAL_WIZARD_MODIFIER;
    }

    return player;
}

// Creates a new dungeon
//
// Parameters:
//      monster         - type of monster occupying the dungeon
//      num_monsters    - number of monsters occupying the dungeon
//      contains_player - 1 if the player is contained in this dungeon,
//                        0 otherwise
// Returns:
//      pointer to newly created struct dungeon
//
// 1. Creates a new struct dungeon (using malloc).
// 2. Copies the name, monster, num_monsters and contains_player arguments 
//    into the corresponding struct fields.
// 3. Initialises all other fields to some reasonable value.
// 4. Returns a pointer to the newly created struct dungeon.

struct dungeon *create_dungeon(char *name, enum monster_type monster,
                               int num_monsters, int contains_player) {
    // TODO: implement this function
    struct dungeon *dungeon = malloc(sizeof(struct dungeon));

    strcpy(dungeon->name, name);
    dungeon->monster = monster;
    dungeon->num_monsters = num_monsters;
    dungeon->original_num_monsters = dungeon->num_monsters;
    dungeon->contains_player = contains_player;
    dungeon->boss = NULL;
    dungeon->items = NULL;
    dungeon->next = NULL;
    dungeon->is_teleported = NOT_TELEPORTED;

    return dungeon;
}

// Append a newly created dungeon to the end of the map's list of dungeons, 
// where entrance is the first dungeon in the list. 
//
// The first dungeon in the map is where the player should start, so it 
// should contain the player.
int append_dungeon(struct map *map,
                   char *name,
                   enum monster_type monster,
                   int num_monsters) {
    // TODO: implement this function
    if (test_dungeon_name(map, name) != PASSED) {
        return INVALID_NAME;
    } else if (test_monster_type(monster) != PASSED) {
        return INVALID_MONSTER;
    } else if (num_monsters < MIN_MONSTERS || num_monsters > MAX_MONSTERS) {
        return INVALID_AMOUNT;
    }

    if (map->entrance == NULL) {
        struct dungeon *new_dungeon = create_dungeon(name, monster, 
            num_monsters, PLAYER);
        map->entrance = new_dungeon;
    } else {
        struct dungeon *current = map->entrance;
        while (current->next != NULL) {
            current = current->next;
        }
        struct dungeon *new_dungeon = create_dungeon(name, monster, 
            num_monsters, NO_PLAYER);
        current->next = new_dungeon;
    }
    
    return VALID;
}

// To display all dungeons in the map and their basic details.
void print_map(struct map *map) {
    // TODO: implement this function
    int position = INITIAL_POSITION;

    if (map->entrance == NULL) {
        print_empty_map();
    } else {
        struct dungeon *current = map->entrance;

        print_map_name(map->name);

        while (current != NULL) {
            print_basic_dungeon(current, map->player->name, position);
            if (current->next != NULL) {
                print_connection();
            }

            current = current->next;
            position++;
        }
    }

    return;
}

// Creates a new boss
//
// Parameters:
//      health_points   - starting health points of the final boss monster
//      damage          - damage dealt by final boss monster in one turn
//      points          - when the boss is defeated, the player will gain
//                        this many points
//      required_item   - the player needs this type of item in their inventory
//                        in order to fight the boss each turn
// Returns:
//      pointer to newly created struct boss
//
struct boss *create_boss(int health_points, int damage, int points,
                         enum item_type required_item) {
    // TODO: implement this function
    struct boss *boss = malloc(sizeof(struct boss));

    boss->health_points = health_points;
    boss->damage = damage;
    boss->points = points;
    boss->required_item = required_item;
    boss->being_attacted = NOT_ATTACKED;
    boss->is_increased_damage = NOT_INCREASED;
    boss->is_defeated = NOT_DEFEATED;

    return boss;
}

// Find the last dungeon in the map and add the final boss to the last 
// dungeon in the map.
int final_boss(struct map *map, enum item_type required_item) {
    // TODO: implement this function
    struct dungeon *current = map->entrance;

    if (test_item_type(required_item) != PASSED) {
        return INVALID_ITEM;
    }

    while (current->next != NULL) {
        current = current->next;
    }

    struct boss *boss = create_boss(INITIAL_BOSS_HEALTH, 
    INITIAL_BOSS_DAMAGE, INITIAL_BOSS_POINTS, required_item);
    current->boss = boss;

    return VALID;
}

// Prints the player's statistics: their name, health points, shield 
// power, damage, magic modifier, points collected, and all items in their
// inventory.
void player_stats(struct map *map) {
    // TODO: implement this function
    struct dungeon *current = map->entrance;

    if (current == NULL) {
        print_player(map->player, NULL);
    } else {
        while (current->contains_player != PLAYER) {
            current = current->next;
        }
        
        print_player(map->player, current->name);
    }
    
    if (map->player->inventory == NULL) {
        print_no_items();
    } else {
        struct item *items = map->player->inventory;
        for (int i = 1; items != NULL; i++) {
            print_item(items, i);
            items = items->next;
        }
    }

    return;
}

// Your functions go here (include function comments):

// Test whether the dungeon has been used before, if not, return PASSED,
// else return NOT_PASSED.
int test_dungeon_name(struct map *map, char *name) {
    struct dungeon *current = map->entrance;

    if (map->entrance == NULL) {
        return PASSED;
    }

    while (current != NULL) {
        if (strcmp(current->name, name) == SAME_STRING) {
            return NOT_PASSED;
        }

        current = current->next;
    }

    return PASSED;
}

// Test whether the monster type in the dungeon is within the given types,
// if not, return NOT_PASSED, else return PASSED.
int test_monster_type(enum monster_type monster_type) {
    if (monster_type == SLIME) {
        return PASSED;
    } else if (monster_type == GOBLIN) {
        return PASSED;
    } else if (monster_type == SKELETON) {
        return PASSED;
    } else if (monster_type == WOLF) {
        return PASSED;
    }

    return NOT_PASSED;
}

// Test whether the item type in the dungeon is within the given types,
// if not, return NOT_PASSED, else return PASSED.
int test_item_type(enum item_type item_type) {
    if (item_type == PHYSICAL_WEAPON) {
        return PASSED;
    } else if (item_type == MAGICAL_TOME) {
        return PASSED;
    } else if (item_type == ARMOR) {
        return PASSED;
    } else if (item_type == HEALTH_POTION) {
        return PASSED;
    } else if (item_type == TREASURE) {
        return PASSED;
    }

    return NOT_PASSED;
}

////////////////////////////////////////////////////////////////////////////////
//                             Stage 2 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

// Insert a new dungeon into a specified position in the dungeon map
//
// If the given position is 1, the new dungeon should become the new 
// entrance of the map.
//
// If the given position is larger than the length of the list, the new 
// dungeon should be added to the tail of the list.
//
// If a dungeon is invalid, it should not be inserted to the list of dungeons 
// in the map.
int insert_dungeon(struct map *map, char *name, enum monster_type monster,
                   int num_monsters, int position) {
    // TODO: implement this function
    int validity;
    validity = test_insert_dungeon(map, name, monster, num_monsters, 
        position);
    if (validity != PASSED) {
        return validity;
    }

    struct dungeon *new_dungeon = create_dungeon(name, monster, num_monsters, 
        NO_PLAYER);
    struct dungeon *current = map->entrance;
    struct dungeon *prev = NULL;
    int num = get_dungeon_num(map->entrance);

    if (current == NULL) {
        new_dungeon->contains_player = PLAYER;
        map->entrance = new_dungeon;
    } else if (position == INITIAL_POSITION) {
        new_dungeon->contains_player = PLAYER;
        map->entrance->contains_player = NO_PLAYER;
        new_dungeon->next = map->entrance;
        map->entrance = new_dungeon;
    } else if (position > num) {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_dungeon;
    } else {
        for (int i = 1; i < position; i++) {
            prev = current;
            current = current->next;
        }
        prev->next = new_dungeon;
        new_dungeon->next = current;
    }

    return VALID;
}

// Print the specific details of the dungeon the player is currently in
// after finding the player's dungeon.
void print_dungeon(struct map *map) {
    // TODO: implement this function
    if (map->entrance == NULL) {
        return;
    }

    struct dungeon *current = find_player(map);

    print_detail_dungeon(map->player->name, current);
    if (current->items == NULL) {
        print_no_items();
    } else {
        struct item *items = current->items;
        for (int i = 1; items != NULL; i++) {
            print_item(items, i);
            items = items->next;
        }
    }

    return;
}

// Move the player forward or backward in the dungeon map.
//
// If there is no dungeon the player can move into, the player should 
// not move and the function will return INVALID.
int move_player(struct map *map, char command) {
    // TODO: implement this function
    if (map->entrance == NULL) {
        return INVALID;
    } 

    struct dungeon *current = find_player(map);
    int index = get_player_dungeon_index (map);
    int length = get_dungeon_num (map->entrance);

    if (command == PREVIOUS_DUNGEON) {
        if (index - MOVE < INITIAL_POSITION) {
            return INVALID;
        } else {
            current->contains_player = NO_PLAYER;
            current = move_to_a_dungeon (map, index - MOVE);
            current->contains_player = PLAYER;
        }
    } else if (command == NEXT_DUNGEON) {
        if (index + MOVE > length) {
            return INVALID;
        } else {
            current->contains_player = NO_PLAYER;
            current = current->next;
            current->contains_player = PLAYER;
        }
    }

    restart_teleportation_cycle(map);

    return VALID;
}

// Implement the fight between the player and the monsters in the 
// current dungeon. 
//
// Return INVALID when there are no monsters in the current dungeon, 
// and return VALID otherwise.
int fight(struct map *map, char command) {
    // TODO: implement this function
    struct dungeon *current = find_player(map);

    if (current->num_monsters == NO_MONSTER) {
        return INVALID;
    }

    int original_num_monsters = current->num_monsters;
    
    if (command == PHYSICAL_ATTACK) {
        int player_damage = map->player->damage / current->monster;
        current->num_monsters -= player_damage;
        if (current->num_monsters <= NO_MONSTER) {
            current->num_monsters = NO_MONSTER;
            player_damage = original_num_monsters;
        }
        map->player->points += player_damage * current->monster;
    } else if (command == MAGICAL_ATTACK) {
        int player_damage = (map->player->damage * 
        map->player->magic_modifier) / current->monster;
        current->num_monsters -= player_damage;
        if (current->num_monsters <= NO_MONSTER) {
            current->num_monsters = NO_MONSTER;
            player_damage = original_num_monsters;
        }
        map->player->points += player_damage * current->monster;
    }

    return VALID;
}

// Handles any actions that need to occur at the end of a turn as 
// the following turn:
// 1. Monster attacks;
// 2. Remove any empty dungeons;
// 3. Boss attack or move;
// 4. Check if the game is over.
//
int end_turn(struct map *map) {
    // TODO: implement this function
    struct dungeon *current = find_player(map);

    if (current->num_monsters > NO_MONSTER) {
        if (current->monster == WOLF) {
            current = attact_player(current, map);
        } else {
            if (current->num_monsters < current->original_num_monsters) {
                current = attact_player(current, map);
            }
        }
    }

    free_dungeon(map);

    boss_fight_or_move(map);

    struct dungeon *boss_dungeon = find_boss(map);
    if (boss_dungeon->boss->health_points <= BOSS_NO_HEALTH && 
    boss_dungeon->boss->is_defeated == NOT_DEFEATED) {
        boss_dungeon->boss->health_points = BOSS_NO_HEALTH;
        map->player->points += boss_dungeon->boss->points;
        boss_dungeon->boss->is_defeated = DEFEATED;
        print_boss_defeat();
    }

    if (map->player->health_points <= PLAYER_NO_HEALTH) {
        return PLAYER_DEFEATED;
    }
    
    if (check_defeat_all_monsters(map) == PASSED && 
    map->player->points >= map->win_requirement &&
    map->player->health_points > PLAYER_NO_HEALTH) {
        return WON_MONSTERS;
    } else if (check_defeat_boss(map) == PASSED && 
    map->player->points >= map->win_requirement &&
    map->player->health_points > PLAYER_NO_HEALTH) {
        return WON_BOSS;
    }

    return CONTINUE_GAME;
}

// Activates the player's class power, and there will be only one
// chance for the player to activate his/her class power.
int class_power(struct map *map) {
    // TODO: implement this function
    struct dungeon *current = find_player(map);

    if (map->player->use_class_power == USED) {
        return INVALID;
    }

    if (strcmp(map->player->class_type, "Fighter") == SAME_STRING) {
        map->player->damage *= FIGHTER_CLASS_POWER;
    } else {
        map->player->points += current->num_monsters * current->monster;
        current->num_monsters = NO_MONSTER;
    }

    map->player->use_class_power = USED;
    return VALID;
}

// Your functions go here (include function comments):

// Check whether a new dungeon can be added into the map:
// Returns:
// 1. INVALID_POSITION when the position is less than 1;
// 2. INVALID_NAME when the name is already being used by a dungeon in the map;
// 3. INVALID_MONSTER when the monster type is not one of:
//             - SLIME,
//             - GOBLIN,
//             - SKELETON,
//             - WOLF;
//  4. INVALID_AMOUNT when the amount of monsters is too small or too large;
//  5. VALID otherwise (no errors found).
//
int test_insert_dungeon(struct map *map, char *name, 
                         enum monster_type monster_type, 
                         int num_monsters, 
                         int position) {
    if (position < INITIAL_POSITION) {
        return INVALID_POSITION;
    } else if (test_dungeon_name(map, name) != PASSED) {
        return INVALID_NAME;
    } else if (test_monster_type(monster_type) != PASSED) {
        return INVALID_MONSTER;
    } else if (num_monsters <  MIN_MONSTERS|| num_monsters > MAX_MONSTERS) {
        return INVALID_AMOUNT;
    }

    return PASSED;
}

// Get the total number of dungeons in the current map.
int get_dungeon_num(struct dungeon *head) {
    int length = INITIAL_LENGTH;
    struct dungeon *current = head;

    if (head == NULL) {
        return length;
    }

    while (current != NULL) {
        current = current->next;
        length++;
    }

    return length;
}

// Find the dungeon the player is in and return the address
// of the corresponding dungeon.
struct dungeon *find_player(struct map *map) {
    struct dungeon *current = map->entrance;
    if (map->entrance == NULL) {
        return NULL;
    }

    while (current->contains_player != PLAYER) {
        current = current->next;
    }

    return current;
}

// Find the index(in order) of the dungeon the player is in.
int get_player_dungeon_index(struct map *map) {
    struct dungeon *current = map->entrance;

    int index = INITIAL_INDEX;
    while (current->contains_player != PLAYER) {
        current = current->next;
        index++;
    }

    return index;
}

// Move to a specific dungeon by looping through the given index(start 
// from 1), and return the address of the required dungeon.
struct dungeon *move_to_a_dungeon(struct map *map, int index) {
    struct dungeon *current = map->entrance;
    if (map->entrance == NULL) {
        return NULL;
    }

    for (int i = INITIAL_INDEX; i < index; i++) {
        current = current->next;
    }

    return current;
}

// Contains the movements that the monsters in current dungeons attack the
// player, and return the current dungeon to update the attacks.
struct dungeon *attact_player(struct dungeon *current, struct map *map) {
    if (current->num_monsters * current->monster > map->player->shield_power) {
        map->player->health_points = map->player->health_points - 
        (current->num_monsters * current->monster - map->player->shield_power);
    }

    return current;
}

////////////////////////////////////////////////////////////////////////////////
//                             Stage 3 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

// Creates a new struct item by initialising its item type and points.
struct item *create_item(enum item_type type, int points) {
    // TODO: implement this function
    struct item *item = malloc(sizeof(struct item));

    item->type = type;
    item->points = points;
    item->next = NULL;

    return item;
}

// Creates and adds an item to the specified dungeon in enum order. If 
// there is already an item of the same type, add it to the end of 
// that type's section.
int add_item(struct map *map,
             int dungeon_number,
             enum item_type type,
             int points) {
    // TODO: implement this function
    int validity = test_add_item(map, dungeon_number, type, points);
    if (validity != PASSED) {
        return validity;
    }

    struct item *new_item = create_item(type, points);
    struct dungeon *dungeon = move_to_a_dungeon(map, dungeon_number);
    struct item *items = dungeon->items;

    if (items == NULL) {
        dungeon->items = new_item;
        return VALID;
    } 

    int item_length = find_item_length(dungeon->items);

    if (check_same_type_item(dungeon->items, type) == NOT_PASSED) {
        int index = find_same_item_index(dungeon->items, type);
        struct item *current_item = move_to_an_item(dungeon->items, index);
        if (index == item_length) {
            current_item->next = new_item;
        } else {
            new_item->next = current_item->next;
            current_item->next = new_item;
        }
    } else {
        dungeon->items = organize_items_in_order(dungeon->items, new_item,
        item_length);
    }

    return VALID;
}

// Moves a specified item from the current dungeon to the player's inventory.
int collect_item(struct map *map, int item_number) {
    // TODO: implement this function
    int validity = test_collect_item(map, item_number);
    if (validity != PASSED) {
        return validity;
    }

    move_item_to_player(map, item_number);

    return VALID;
}

// Uses a specified item in the player's inventory and frees the used item
// from the computer's memory.
int use_item(struct map *map, int item_number) {
    // TODO: implement this function
    int validity = test_use_item(map, item_number);
    if (validity != VALID) {
        return INVALID;
    }

    struct item *current = map->player->inventory;
    struct item *prev = NULL;

    for (int i = 1; i < item_number; i++) {
        prev = current;
        current = current->next;
    }

    use_item_effect(map, current);

    if (prev == NULL) {
        map->player->inventory = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);

    return VALID;
}

// Frees all malloc'd memory in the map, and then the map itself in the 
// following order:
// 1. Free all of the dungeons;
//      --- all items in the current dungeon
//      --- boss in the current dungeon(if exists)
// 2. Free all the inventories from the player;
// 3. Free player itself;
// 4. Free the map itself.
void free_map(struct map *map) {
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (current->boss != NULL) {
            free(current->boss);
        }

        if (current->items != NULL) {
            free_items(current->items);
        }

        struct dungeon *delete_dungeon = current;
        current = current->next;
        free(delete_dungeon);
    }

    if (map->player->inventory != NULL) {
        free_items(map->player->inventory);
    }
    
    free(map->player);
    free(map);

    return;
}

// Your functions here (include function comments):

// Checks that whether all of the monsters in the map has been
// defeated, if not, return NOT_PASSED; if so, return PASSED.
int check_defeat_all_monsters(struct map *map) {
    int validity = PASSED;
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        if (current->num_monsters > NO_MONSTER) {
            validity = NOT_PASSED;
        }
        current = current->next;
    }

    return validity;
}

// Checks whether the boss in the map has been defeated, if not, 
// return NOT_PASSED; if so, return PASSED.
int check_defeat_boss(struct map *map) {
    int validity = PASSED;
    struct dungeon *current = map->entrance;

    while (current->boss == NULL) {
        current = current->next;
    }

    if (current->boss->health_points > BOSS_NO_HEALTH) {
        validity = NOT_PASSED;
    }

    return validity;
}

// Tests whether an item can be added to the current dungeon.
//
// Returns:
// 1. INVALID_DUNGEON when the dungeon number does not correspond to a dungeon
//    in the map;
// 2. INVALID_ITEM when the item is not a valid type;
// 3. INVALID_POINTS when the item is worth too little or too many points;
// 4. VALID otherwise (no errors).
//
int test_add_item(struct map *map, 
                  int dungeon_number,
                  enum item_type type,
                  int points) {
    int validity = PASSED;
    int length = get_dungeon_num(map->entrance);

    if (dungeon_number < INITIAL_INDEX || dungeon_number > length) {
        return INVALID_DUNGEON;
    } else if (type < PHYSICAL_WEAPON || type > TREASURE) {
        return INVALID_ITEM;
    } else if (points < MIN_ITEM_POINT_VALUE || 
    points > MAX_ITEM_POINT_VALUE) {
        return INVALID_POINTS;
    }

    return validity;
}

// Checks if there is already an item of the same type the player want
// to add to the dungeon, if there is at least one, then return NOT_PASSED,
// otherwise return PASSED(no same items are in this dungeon).
int check_same_type_item(struct item *item, enum item_type type) {
    struct item *current = item;

    while (current != NULL) {
        if (current->type == type) {
            return NOT_PASSED;
        }
        current = current->next;
    }

    return PASSED;
}

// Find the last index (start from 1) of the item of the same type in
// the current dungeon.
int find_same_item_index(struct item *item, enum item_type type) {
    struct item *current = item;
    int index = INITIAL_INDEX;

    for (int i = 1; current != NULL; i++) {
        if (current->type == type) {
            index = i;
        }
        current = current->next;
    }

    return index;
}

// Moves to a specific item looping through the index (start from 1), and
// returns the address of the required item.
struct item *move_to_an_item(struct item *item, int index) {
    struct item *current = item;

    int i = INITIAL_INDEX;
    while (i < index) {
        current = current->next;
        i++;
    }

    return current;
}

// Find the total number of the items in the player's inventory or 
// in a dungeon.
int find_item_length(struct item *item) {
    struct item *current = item;
    int i;

    for (i = INITIAL_LENGTH; current != NULL; i++) {
        current = current->next;
    }

    return i;
}

// Organize the new added item to the end of that type's section
// when there is already an item of the same type.
struct item *organize_items_in_order(struct item *items, 
                                     struct item *new_item,
                                     int list_length) {
    int index = 0;
    struct item *current = items;

    int i = 1;
    while (current != NULL) {
        if (new_item->type > current->type) {
            index = i;
        }
        current = current->next;
        i++;
    }

    if (index == 0) {
        new_item->next = items;
        items = new_item;
    } else if (index == list_length) {
        current = move_to_an_item(items, list_length);
        current->next = new_item;
    } else {
        current = move_to_an_item(items, index);
        new_item->next = current->next;
        current->next = new_item;
    }

    return items;
}

// Test whether an item can be added to the player's inventory, if the 
// item number does not correspond to an item in the dungeon, return
// INVALID_ITEM, otherwise return PASSED.
int test_collect_item(struct map *map, int item_number) {
    struct dungeon *current = find_player(map);
    if (current->items == NULL) {
        return INVALID_ITEM;
    }

    int list_length = find_item_length(current->items);
    
    if (item_number < INITIAL_INDEX || item_number > list_length) {
        return INVALID_ITEM;
    }
    
    return PASSED;
}

// Move the selected item from the current dungeon to player's inventory
// in the order of picking up (the new pick_up will be the entrance of 
// player's inventory).
void move_item_to_player(struct map *map, int item_number) {
    struct dungeon *current_dungeon = find_player(map);
    struct item *current_item = current_dungeon->items;
    struct item *prev_item = NULL;

    for (int i = 1; i < item_number; i++) {
        prev_item = current_item;
        current_item = current_item->next;
    }

    if (prev_item == NULL) {
        current_dungeon->items = current_item->next;
        current_item->next = map->player->inventory;
        map->player->inventory = current_item;
    } else {
        prev_item->next = current_item->next;
        current_item->next = map->player->inventory;
        map->player->inventory = current_item;
    }

    return;
}

// Test whether the player can use a specified item in the inventory. If
// the item number does not correspond to an item in the player's inventory,
// return INVALID, otherwise return VALID.
int test_use_item(struct map *map, int item_number) {
    int list_length = find_item_length(map->player->inventory);

    if (list_length == INITIAL_LENGTH) {
        return INVALID;
    } else if (item_number < INITIAL_INDEX || item_number > list_length) {
        return INVALID;
    }

    return VALID;
}

// Implement the effect of the used item to the player based on its type.
void use_item_effect(struct map *map, struct item *item) {
    if (item->type == PHYSICAL_WEAPON) {
        map->player->damage += item->points;
    } else if (item->type == MAGICAL_TOME) {
        map->player->magic_modifier = map->player->magic_modifier + 
        (item->points / 10.0);
    } else if (item->type == ARMOR) {
        map->player->shield_power = map->player->shield_power + 
        (item->points / 2);
    } else if (item->type == HEALTH_POTION) {
        map->player->health_points += item->points + 5;
        if (map->player->health_points >= MAX_HEALTH) {
            map->player->health_points = MAX_HEALTH;
        }
    }
    map->player->points += item->points;

    return;
}

// Test whether a dungeon is empty or not:
// 1. no monsters in the current dungeon;
// 2. no items in the current dungeon;
// 3. the boss is not in this dungeon;
// 4. the player is not in this dungeon.
//
int test_free_map(struct dungeon *current) {
    if (current->num_monsters != NO_MONSTER) {
        return NOT_PASSED;
    } else if (current->boss != NULL) {
        return NOT_PASSED;
    } else if (current->items != NULL) {
        return NOT_PASSED;
    } else if (current->contains_player != NO_PLAYER) {
        return NOT_PASSED;
    }

    return PASSED;
}

// Loop through all of the dungeons in the current map, deleting and freeing
// empty dungeons.
void free_dungeon(struct map *map) {
    struct dungeon *current = map->entrance;
    struct dungeon *prev = NULL;

    while (current != NULL) {
        if (test_free_map(current) == PASSED) {
            if (prev == NULL) {
                map->entrance = current->next;
                free(current);
                current = map->entrance;
            } else {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            restart_teleportation_cycle(map);
        } else {
            prev = current;
            current = current->next;
        }
    }

    return;
}

// Free all items in a dungeon or a player's inventory by looping through
// all the items.
void free_items(struct item *items) {
    struct item *current = items;
 
    while (current != NULL) {
        struct item *delete = current;
        current = current->next;
        free (delete);
    }

    return;
}
////////////////////////////////////////////////////////////////////////////////
//                             Stage 4 Functions                              //
////////////////////////////////////////////////////////////////////////////////

// Provided function stubs:

// Teleports the player from the current dungeon to the furthest dungeon away
// that has not been visited yet via teleportation. When all of the dungeons
// have been teleported, restart the teleportation cycle.
int teleport(struct map *map) {
    // TODO: implement this function
    if (map->entrance == NULL || map->entrance->next == NULL) {
        return INVALID;
    }

    check_all_dungeon_teleported (map);
    teleportation(map);

    return VALID;
}

// Attempts to have the player fight the boss monster.
int boss_fight(struct map *map) {
    // TODO: implement this function
    if (test_boss_fight(map) != VALID) {
        return test_boss_fight(map);
    }

    struct dungeon *current = find_player(map);

    if (map->player->magic_modifier >= USING_MAGIC) {
        current->boss->health_points -= (int)(map->player->damage * 
        map->player->magic_modifier);
    } else {
        current->boss->health_points -= map->player->damage;
    }

    if (current->boss->being_attacted == NOT_ATTACKED) {
        shuffle_dungeons(map);
        current->boss->being_attacted = ATTACKED;
    }

    return VALID;
}

// Your functions here (include function comments):

// Check if all of the dungeons have been teleported, if so, then restart the
// teleportation cycle.
void check_all_dungeon_teleported(struct map *map) {
    struct dungeon *current = map->entrance;

    while (current != NULL) {
        if (current->is_teleported == NOT_TELEPORTED) {
            return;
        }
        current = current->next;
    }

    restart_teleportation_cycle(map);
    return;
}

// Implement the movement of teleporting to the furthest dungeon which hasn't
// been teleported to.
void teleportation(struct map *map) {
    int current_dungeon_index = find_current_dungeon_index(map);

    struct dungeon *current = find_player(map);
    current->is_teleported = TELEPORTED;

    int target_index = find_target_dungeon_index(current_dungeon_index, map);
    struct dungeon *target = move_to_a_dungeon(map, target_index);

    current->contains_player = NO_PLAYER;
    target->contains_player = PLAYER;
    target->is_teleported = TELEPORTED;

    return;
}

// Find the index (start from 1) of the dungeon where the player in.
int find_current_dungeon_index(struct map *map) {
    struct dungeon *current = map->entrance;

    int index = INITIAL_INDEX;
    while (current->contains_player != PLAYER) {
        current = current->next;
        index++;
    }

    return index;
}

// Find the furthese dungeon which has not been teleported to that the player
// is able to teleport to.
int find_target_dungeon_index(int current_index, struct map *map) {
    int target_index;

    int forward_index = INITIAL_INDEX;
    struct dungeon *forward_current = map->entrance;

    int length = get_dungeon_num(map->entrance);
    int backward_index = length;
    struct dungeon *backward_current = move_to_a_dungeon(map, backward_index);

    while (forward_current->is_teleported != NOT_TELEPORTED && 
    forward_index <= length) {
        forward_index++;
        forward_current = forward_current->next;
    }

    while (backward_current->is_teleported != NOT_TELEPORTED && 
    backward_index >= INITIAL_INDEX) {
        backward_index--;
        backward_current = move_to_a_dungeon(map, backward_index);
    }

    int forward_distance = abs(current_index - forward_index);
    int backward_distance = abs(current_index - backward_index);

    if (forward_distance >= backward_distance) {
        target_index = forward_index;
    } else {
        target_index = backward_index;
    }

    return target_index;
}

// This function restart the teleportation cycle, makes all of the dungeons
// able to be teleported or be the start for teleportation.
void restart_teleportation_cycle(struct map *map) {
    struct dungeon *current = map->entrance;
    while (current != NULL) {
        current->is_teleported = NOT_TELEPORTED;
        current = current->next;
    }

    return;
}

// Test whether the player can start the fight with the boss.
// Returns:
// 1. NO_BOSS when there is no boss in the current dungeon;
// 2.  NO_ITEM when the player does not have the required item to fight the
//     boss;
// 3. VALID otherwise (no errors).
int test_boss_fight(struct map *map) {
    struct dungeon *player_dungeon = find_player(map);
    struct dungeon *boss_dungeon = find_boss(map);

    if (player_dungeon->boss == NULL) {
        return NO_BOSS;
    } else if (find_required_item(map, boss_dungeon->boss->required_item) == 
    NOT_PASSED) {
        return NO_ITEM;
    } else {
        return VALID;
    }
}

// Find the dungeon where the boss is currently in, and return the address
// of the dungeon found.
struct dungeon *find_boss(struct map *map) {
    struct dungeon *current = map->entrance;

    while (current != NULL) {
        if (current->boss != NULL) {
            return current;
        }
        current = current->next;
    }

    return current;
}

// Find if there's the required item to fight the boss in the player's
// inventory, if no, return NOT_PASSED, otherwise return PASSED.
int find_required_item(struct map *map, enum item_type item) {
    struct item *current = map->player->inventory;
    
    while (current != NULL) {
        if (current->type == item) {
            return PASSED;
        }
        current = current->next;
    }

    return NOT_PASSED;
}

// Shuffle the dungeons when it is the first time attacking the boss:
// All pairs of the dungeons should be swapped. If there is an odd number 
// of dungeons, e.g. 5 dungeons, swap the first 4, leaving the last dungeon
// in its original position.
void shuffle_dungeons(struct map *map) {
    int length = get_dungeon_num(map->entrance);
    int group_num = length / 2;

    if (group_num < INITIAL_INDEX) {
        return;
    }

    struct dungeon *prev = map->entrance;
    struct dungeon *curr = map->entrance->next;
    
    for (int i = 1; i <= group_num; i++) {
        if (i == INITIAL_INDEX) {
            map->entrance = curr;
            prev->next = curr->next;
            curr->next = prev;
        } else {
            swap_pair_dungeon(map, 2 * i - 2, 2 * i - 1, 2 * i);
        }
    }
    
    restart_teleportation_cycle(map);

    return;
}

// Swap the position of a pair of dungeon in the current map.
void swap_pair_dungeon(struct map *map, 
                       int entrance_index, 
                       int index1, 
                       int index2) {
    struct dungeon *entrance = move_to_a_dungeon(map, entrance_index);
    struct dungeon *prev = move_to_a_dungeon(map, index1);
    struct dungeon *curr = move_to_a_dungeon(map, index2);

    entrance->next = curr;
    prev->next = curr->next;
    curr->next = prev;

    return;
}

// Implement the action of the boss after being attacked: fight against the
// player if it is in the same dungeon with the player or chase the player
// towards the dungeon the player is currently in.
void boss_fight_or_move(struct map *map) {
    struct dungeon *player_dungeon = find_player(map);
    struct dungeon *boss_dungeon = find_boss(map);

    if (boss_dungeon->boss->health_points <= BOSS_NO_HEALTH) {
        return;
    }

    if (boss_dungeon->boss->being_attacted == NOT_ATTACKED) {
        return;
    }

    if (player_dungeon->boss == NULL) {
        boss_move(map);
    } else {
        boss_attack(map);
    }
}

// Implement the attacks from the boss to the player.
void boss_attack (struct map *map) {
    struct dungeon *current = find_player(map);

    if (current->boss->health_points <= INITIAL_BOSS_HEALTH / 2 && 
    current->boss->is_increased_damage == NOT_INCREASED) {
        current->boss->damage *= BOSS_INCREASE_POWER;
        current->boss->is_increased_damage = INCREASED;
    }

    if (current->boss->damage > map->player->shield_power) {
        map->player->health_points -= current->boss->damage - 
        map->player->shield_power;
    }

    return;
}

// Implement the moves of the boss towards the dungeon where the player
// is currently in.
void boss_move (struct map *map) {
    int player_dungeon_index = get_player_dungeon_index(map);
    int boss_dungeon_index = get_boss_dungeon_index(map);

    struct dungeon *boss_dungeon = find_boss(map);

    if (player_dungeon_index > boss_dungeon_index) {
        struct dungeon *move_to = move_to_a_dungeon(map, 
        boss_dungeon_index + MOVE);
        move_to->boss = boss_dungeon->boss;
        boss_dungeon->boss = NULL;
    } else {
        struct dungeon *move_to = move_to_a_dungeon(map, 
        boss_dungeon_index - MOVE);
        move_to->boss = boss_dungeon->boss;
        boss_dungeon->boss = NULL;
    }
}

// Get the index of the dungeon where the boss is currently in.
int get_boss_dungeon_index(struct map *map) {
    struct dungeon *current = map->entrance;
    
    int index = INITIAL_INDEX;
    while (current->boss == NULL) {
        current = current->next;
        index++;
    }

    return index;
}

////////////////////////////////////////////////////////////////////////////////
//                              Helper Functions                              //
////////////////////////////////////////////////////////////////////////////////

// The following are helper functions that you can use to print any output
// that main.c cannot handle for you.
// The specification will tell you which functions you should use for each 
// stage.

char *get_player_name(struct player *player) {
    return player->name;
}

void print_map_name(char *name) {
    printf("Map of %s!\n", name);
}

void print_basic_dungeon(struct dungeon *dungeon, char *player_name,
                         int position) {
    printf("|^|^|^|^|^|   |^|^|^|^|^|\n\n");

    printf("%d. %s\n", position, dungeon->name);

    if (dungeon->boss == NULL) {
        printf("Boss: None\n");
    } else {
        printf("Boss: Present\n");
    }

    printf("Monster: %s\n", monster_string(dungeon->monster));

    if (dungeon->contains_player) {
        printf("%s is here\n", player_name);
    } else {
        printf("Empty\n");
    }

    printf("\n|^|^|^|^|^|   |^|^|^|^|^|\n");
}

void print_connection(void) {
    printf("          |   |\n          |   |\n          |   |\n");
}

void print_player(struct player *player, char *dungeon_name) {
    printf("=======Player Stats=======\n");
    if (dungeon_name == NULL) {
        printf("%s has not entered the map yet!\n", player->name);
    } else {
        printf("%s is currently in %s\n", player->name, dungeon_name);
    }

    printf("%s\n", class_string(player));

    printf("Health Points: %d\n", player->health_points);
    printf("Shield Power: %d\n", player->shield_power);
    printf("Damage: %d\n", player->damage);
    printf("Magic Modifier: %.1lf\n", player->magic_modifier);
    printf("Points Collected: %d\n", player->points);
    printf("%s has the following items in their inventory:\n", player->name);
}

void print_item(struct item *item, int position) {
    printf("%d. ", position);
    printf("%s", item_string(item->type));
    printf(", worth %d point(s).\n", item->points);
}

void print_no_items() {
    printf("No Items\n");
}

void print_detail_dungeon(char *player_name, struct dungeon *dungeon) {
    printf("======Dungeon Details======\n");
    printf("%s is currently in %s\n", player_name, dungeon->name);

    if (dungeon->monster == WOLF) {
        printf("There are %d wolves\n", dungeon->num_monsters);
    } else {
        printf("There are %d %ss\n", dungeon->num_monsters,
                                      monster_string(dungeon->monster));
    }

    if (dungeon->boss == NULL) {
        printf("No boss in this dungeon\n");
    } else {
        printf("The boss is in this dungeon\n");
        printf("\tHealth Points: %d\n", dungeon->boss->health_points);
        printf("\tDamage: %d\n", dungeon->boss->damage);
        printf("\tPoints: %d\n", dungeon->boss->points);
        printf("\tRequired Item: %s\n", 
               item_string(dungeon->boss->required_item));
    }

    printf("The dungeon %s has the following items:\n", dungeon->name);
}

char *class_string(struct player *player) {
    if (strncmp(player->class_type, "Fighter", 8) == 0) {
        return "Fighter";
    } else if (strncmp(player->class_type, "Wizard", 7) == 0) {
        return "Wizard";
    } else {
        return "Unknown Class";
    }
}

char *item_string(enum item_type item) {
    if (item == PHYSICAL_WEAPON) {
        return "Physical Weapon";
    } else if (item == MAGICAL_TOME) {
        return "Magical Tome";
    } else if (item == ARMOR) {
        return "Armor";
    } else if (item == HEALTH_POTION) {
        return "Health Potion";
    } else if (item == TREASURE) {
        return "Treasure";
    } else {
        return "Unknown Item";
    }
}

char *monster_string(enum monster_type monster) {
    if (monster == SLIME) {
        return "Slime";
    } else if (monster == GOBLIN) {
        return "Goblin";
    } else if (monster == SKELETON) {
        return "Skeleton";
    } else if (monster == WOLF) {
        return "Wolf";
    } else {
        return "Unknown Monster";
    }
}

void print_boss_defeat() {
    printf("The boss has been defeated!\n");
}

void print_empty_map() {
    printf("There are no dungeons currently in the dungeon.\n");
}