#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK_DEPTH 10

// Forward declaration
typedef struct MenuObject Menu;

// --- 1. The CG Menu Object Struct ---
struct MenuObject {
    char *title;
    void (*display)(Menu *self);
    void (*handleInput)(Menu *self, int choice, struct MenuManager *manager);
};

// --- 2. The Menu Manager Object (Tracks the Active Menu Array as a Stack) ---
typedef struct MenuManager {
    Menu *stack[MAX_STACK_DEPTH]; // Array storing active menus
    int top;                      // Points to the active menu at the top of the stack
    bool isEngineRunning;         // Master execution flag
} MenuManager;

// --- 3. Memory & Stack Management Functions ---

// Constructor for individual menu nodes
Menu* Menu_Create(const char *title, void (*display)(Menu*), void (*handleInput)(Menu*, int, MenuManager*)) {
    Menu *newMenu = (Menu*)malloc(sizeof(Menu));
    if (!newMenu) return NULL;

    newMenu->title = (char*)malloc(strlen(title) + 1);
    if (!newMenu->title) {
        free(newMenu);
        return NULL;
    }
    strcpy(newMenu->title, title);
    newMenu->display = display;
    newMenu->handleInput = handleInput;
    return newMenu;
}

// Destructor for individual menu nodes
void Menu_Destroy(Menu *menu) {
    if (menu) {
        if (menu->title) free(menu->title);
        free(menu);
    }
}

// Push a newly generated menu onto the active array tracker
void Manager_Push(MenuManager *manager, Menu *menu) {
    if (manager->top >= MAX_STACK_DEPTH - 1) {
        printf("[Error] Menu stack overflow! Cannot open more sub-menus.\n");
        Menu_Destroy(menu); // Free it to avoid leaking memory
        return;
    }
    manager->top++;
    manager->stack[manager->top] = menu;
    printf("[System] Pushed '%s' to active stack. Depth Tracker: %d/%d\n", 
           menu->title, manager->top + 1, MAX_STACK_DEPTH);
}

// Pop the active menu off the tracker and run cleanup immediately
void Manager_Pop(MenuManager *manager) {
    if (manager->top < 0) return;
    
    Menu *toDestroy = manager->stack[manager->top];
    printf("[System] Popping '%s' off stack and destroying memory... ", toDestroy->title);
    
    Menu_Destroy(toDestroy);
    manager->stack[manager->top] = NULL;
    manager->top--;
    
    printf("Depth Tracker: %d/%d\n", manager->top + 1, MAX_STACK_DEPTH);
}

// --- 4. Concrete Menu Implementations ---

// Forward declarations of behavioral logic
void displayMainMenu(Menu *self);
void handleMainMenu(Menu *self, int choice, MenuManager *manager);
void displaySettingsMenu(Menu *self);
void handleSettingsMenu(Menu *self, int choice, MenuManager *manager);
void displayGraphicsMenu(Menu *self);
void handleGraphicsMenu(Menu *self, int choice, MenuManager *manager);

// ---- MAIN MENU CODE ----
void displayMainMenu(Menu *self) {
    printf("\n===================================\n");
    printf("     %s     \n", self->title);
    printf("===================================\n");
    printf("1. Trigger Engine Action\n");
    printf("2. Open Settings Menu (Push Level 1)\n");
    printf("3. Shut Down System\n");
    printf("===================================\n");
}

void handleMainMenu(Menu *self, int choice, MenuManager *manager) {
    switch (choice) {
        case 1:
            printf("\n[Action] Performing calculations on core engine stack...\n");
            break;
        case 2: {
            Menu *settings = Menu_Create("SETTINGS LEVEL 1", displaySettingsMenu, handleSettingsMenu);
            if (settings) Manager_Push(manager, settings);
            break;
        }
        case 3:
            printf("\n[Exit] Powering down engine context.\n");
            manager->isEngineRunning = false;
            break;
        default:
            printf("\n[Error] Choice invalid.\n");
    }
}

// ---- SETTINGS SUB-MENU CODE ----
void displaySettingsMenu(Menu *self) {
    printf("\n-----------------------------------\n");
    printf("     %s     \n", self->title);
    printf("-----------------------------------\n");
    printf("1. Audio Volume (100%%)\n");
    printf("2. Open Graphics Menu (Push Level 2)\n");
    printf("3. Back to Previous Menu (Pop Stack)\n");
    printf("-----------------------------------\n");
}

void handleSettingsMenu(Menu *self, int choice, MenuManager *manager) {
    switch (choice) {
        case 1:
            printf("\n[Audio] Volume verified at max capacity.\n");
            break;
        case 2: {
            Menu *graphics = Menu_Create("GRAPHICS LEVEL 2", displayGraphicsMenu, handleGraphicsMenu);
            if (graphics) Manager_Push(manager, graphics);
            break;
        }
        case 3:
            Manager_Pop(manager);
            break;
        default:
            printf("\n[Error] Choice invalid.\n");
    }
}

// ---- DEEPLY NESTED GRAPHICS MENU CODE ----
void displayGraphicsMenu(Menu *self) {
    printf("\n+++++++++++++++++++++++++++++++++++\n");
    printf("     %s     \n", self->title);
    printf("+++++++++++++++++++++++++++++++++++\n");
    printf("1. Change Resolution (1080p)\n");
    printf("2. Back to Settings (Pop Stack)\n");
    printf("+++++++++++++++++++++++++++++++++++\n");
}

void handleGraphicsMenu(Menu *self, int choice, MenuManager *manager) {
    switch (choice) {
        case 1:
            printf("\n[Config] Screen resolution re-initialized.\n");
            break;
        case 2:
            Manager_Pop(manager);
            break;
        default:
            printf("\n[Error] Choice invalid.\n");
    }
}

// --- 5. Application Execution Context ---
int main() {
    // 1. Setup global menu manager state
    MenuManager manager = { .top = -1, .isEngineRunning = true };

    // 2. Initialize and push our baseline Main Menu object onto stack entry index 0
    Menu *mainMenu = Menu_Create("CG CORE BASE ENGINE", displayMainMenu, handleMainMenu);
    if (!mainMenu) return 1;
    Manager_Push(&manager, mainMenu);

    int choice;

    // 3. The Master Execution Hook using a standard structural do-while loop
    do {
        // Look up the active graphic menu sitting dynamically on the top of our tracking array
        Menu *activeMenu = manager.stack[manager.top];
        
        // Print the active context
        activeMenu->display(activeMenu);
        printf("Current Sub-menu Stack Depth: %d levels deep\n", manager.top + 1);
        printf("Input Selection: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear character noise from input pipeline
            continue;
        }

        // Pass event routing authority directly to the targeted execution method
        activeMenu->handleInput(activeMenu, choice, &manager);

    } while (manager.isEngineRunning && manager.top >= 0);

    // 4. Emergency Cleanup Loop: If application exits, empty remaining slots to guarantee clean memory
    while (manager.top >= 0) {
        Manager_Pop(&manager);
    }

    printf("\n[System] Stack cleared. Game Engine lifecycle finalized safely.\n");
    return 0;
}
