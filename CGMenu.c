#include <stdio.h>
#include <stdbool.h>

// --- 1. Forward Declarations & Function Pointers ---
typedef struct MenuObject Menu;
typedef void (*MenuAction)();

// --- 2. Definition of the CG Menu Object ---
struct MenuObject {
    const char *title;
    bool isRunning;
    void (*display)(Menu *self);
    void (*executeSubMenu)(Menu *self);
};

// --- 3. Feature Functions (Sub-Menus & Actions) ---
void openGraphicsSettings() {
    int choice;
    do {
        printf("\n--- GRAPHICS SETTINGS ---\n");
        printf("1. Resolution: 1920x1080\n");
        printf("2. V-Sync: Enabled\n");
        printf("3. Back to Settings\n");
        printf("Enter choice (1-3): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: printf("\n[Config] Resolution set to 1080p.\n"); break;
            case 2: printf("\n[Config] V-Sync toggled.\n"); break;
            case 3: printf("\nReturning to Settings Menu...\n"); break;
            default: printf("\n[Error] Invalid choice.\n");
        }
    } while(choice != 3);
}

void openSettingsMenu() {
    int choice;
    do {
        printf("\n===== SETTINGS SUB-MENU =====\n");
        printf("1. Audio Settings\n");
        printf("2. Graphics Settings (Nested Sub-Menu)\n");
        printf("3. Back to Main Menu\n");
        printf("=============================\n");
        printf("Enter choice (1-3): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: printf("\n[Audio] Volume set to 100%%.\n"); break;
            case 2: openGraphicsSettings(); break; // Deeply nested sub-menu
            case 3: printf("\nReturning to Main Menu...\n"); break;
            default: printf("\n[Error] Invalid choice.\n");
        }
    } while(choice != 3);
}

// --- 4. Object Behavior Implementation ---
void displayMainMenu(Menu *self) {
    printf("\n===================================\n");
    printf("     %s     \n", self->title);
    printf("===================================\n");
    printf("1. Start Application / Game\n");
    printf("2. Open Settings (Sub-Menu)\n");
    printf("3. View Help & Credits\n");
    printf("4. Exit Program\n");
    printf("===================================\n");
    printf("Select an option (1-4): ");
}

void executeMainMenu(Menu *self) {
    int choice;
    
    // Explicit do-while loop checking the object's active state
    do {
        self->display(self);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n[Action] Initializing engine core...\n");
                break;
            case 2:
                openSettingsMenu(); // Launches the Sub-Menu loop
                break;
            case 3:
                printf("\n[Credits] Created by AI CG-Menu Engine v2.0.\n");
                break;
            case 4:
                printf("\nExiting. Destroying Menu Object context...\n");
                self->isRunning = false; // Graceful termination condition
                break;
            default:
                printf("\n[Error] Selection out of bounds. Try again.\n");
        }
    } while (self->isRunning); 
}

// --- 5. Main Runtime Context ---
int main() {
    // Instantiate and initialize our CG Menu Object
    Menu mainMenu = {
        .title = "CG GRAPHICS ENGINE MAIN MENU",
        .isRunning = true,
        .display = displayMainMenu,
        .executeSubMenu = executeMainMenu
    };

    // Trigger the object's logic loop
    mainMenu.executeSubMenu(&mainMenu);

    return 0;
}
