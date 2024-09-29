//MD AKASH HOSSAIN 
// 2021521460115


#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

#define BOARD_COUNT 6
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 600
#define BOARD_WIDTH 100
#define BOARD_HEIGHT 10
#define CHAR_SIZE 20
#define SPIKE_HEIGHT 30
#define GIFT_BAG_WIDTH 30
#define GIFT_BAG_HEIGHT 30

struct Board {
    int x, y, width, speed;
};

struct GiftBag {
    int x, y;
    bool active; // Status of the gift bag
};

Board boards[BOARD_COUNT];
GiftBag giftBag;
int health = 3;
int charX, charY; 
int charSpeed = 10;
int speed = 5; 
bool onBoard = false;


void initGame() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < BOARD_COUNT; i++) {
        boards[i].x = rand() % (SCREEN_WIDTH - BOARD_WIDTH);
        boards[i].y = SCREEN_HEIGHT - i * (SCREEN_HEIGHT / BOARD_COUNT);
        boards[i].width = BOARD_WIDTH;
        boards[i].speed = speed; 
    }

   
    int randomBoardIndex = rand() % BOARD_COUNT;
    charX = boards[randomBoardIndex].x + (BOARD_WIDTH / 2); 
    charY = boards[randomBoardIndex].y - CHAR_SIZE; 

    // Initialize the gift bag
    giftBag.active = false;
}


void displayStartScreen() {
    setbkcolor(RGB(0, 0, 255)); 
    cleardevice(); 

    
    setcolor(RGB(255, 255, 255)); 

    // Display Game Over message
    settextcolor(RED);
    setbkmode(TRANSPARENT); 

    outtextxy(200, 300, _T("Game Successfully Start!"));

    settextcolor(BLACK);
    outtextxy(180, 350, _T("To press Enter."));

    FlushBatchDraw(); 
    Sleep(3000); // Delay for 3 seconds before starting the game
}

// Function to draw spikes at the top
void drawSpikes() {
    setlinecolor(RGB(0, 0, 0)); 
    for (int i = 0; i < 20; i++) {
        line(30 * i, 0, 30 * i + 15, SPIKE_HEIGHT);
        line(30 * i + 15, SPIKE_HEIGHT, 30 * (i + 1), 0);
    }
}

// Function to move and draw boards
void moveAndDrawBoards() {
    int boardColors[BOARD_COUNT] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 0, 255), RGB(0, 255, 255) };

    for (int i = 0; i < BOARD_COUNT; i++) {
        // Move the boards upwards
        boards[i].y -= boards[i].speed;
        if (boards[i].y < 0) {
            boards[i].y = SCREEN_HEIGHT - BOARD_HEIGHT;
            boards[i].x = rand() % (SCREEN_WIDTH - BOARD_WIDTH);
        }
       
        setfillcolor(boardColors[i]); 
        fillrectangle(boards[i].x, boards[i].y, boards[i].x + boards[i].width, boards[i].y + BOARD_HEIGHT);
    }
}


void placeCharacter() {
    setfillcolor(RGB(0, 0, 0)); 
    fillcircle(charX, charY, CHAR_SIZE);
}

// Function to draw the gift bag
void drawGiftBag() {
    if (giftBag.active) {
        setfillcolor(RGB(0, 0, 0)); // Set gift bag color to black
        fillrectangle(giftBag.x, giftBag.y, giftBag.x + GIFT_BAG_WIDTH, giftBag.y + GIFT_BAG_HEIGHT);
    }
}

// Function to move the character
void moveCharacter() {
    // Move left
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) charX -= charSpeed;
    // Move right
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) charX += charSpeed;
    // Move up
    if (GetAsyncKeyState(VK_UP) & 0x8000) charY -= charSpeed;
    // Move down
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) charY += charSpeed;

    // Ensure character stays within screen bounds
    if (charX < 0) charX = 0;
    if (charX > SCREEN_WIDTH - CHAR_SIZE) charX = SCREEN_WIDTH - CHAR_SIZE;
    if (charY < 0) charY = 0; 
    if (charY > SCREEN_HEIGHT - CHAR_SIZE) charY = SCREEN_HEIGHT - CHAR_SIZE; 

    
    if (onBoard) {
        charY -= speed; 
    }
    else {
        charY += speed; 
    }
}


void detectCollision() {
    onBoard = false; 
    for (int i = 0; i < BOARD_COUNT; i++) {
        if (charY + CHAR_SIZE >= boards[i].y && charY + CHAR_SIZE <= boards[i].y + BOARD_HEIGHT &&
            charX + CHAR_SIZE >= boards[i].x && charX - CHAR_SIZE <= boards[i].x + boards[i].width) {
           
            charY = boards[i].y - CHAR_SIZE; 
            onBoard = true; 
            break; 
        }
    }
}


void generateGiftBag() {
    if (!giftBag.active && rand() % 100 < 5) { // 5% chance to generate a gift bag
        giftBag.x = rand() % (SCREEN_WIDTH - GIFT_BAG_WIDTH);
        giftBag.y = rand() % 300 + 100; 
        giftBag.active = true; // Activate the gift bag
    }
}

// Function to check if the character touches the spikes or falls down
void checkGameOverConditions() {
    // Check if character touches the spikes (top of the screen)
    if (charY <= SPIKE_HEIGHT) {
        printf("Game Over! You hit the spikes.\n"); // Print Game Over message
        health = 0; // Set health to zero to trigger game over
    }
   
    if (charY >= SCREEN_HEIGHT) {
        printf("Game Over! You fell off the screen.\n"); // Print Game Over message
        health = 0; // Set health to zero to trigger game over
    }

    // Check if the character collects the gift bag
    if (giftBag.active && charX < giftBag.x + GIFT_BAG_WIDTH && charX + CHAR_SIZE > giftBag.x &&
        charY < giftBag.y + GIFT_BAG_HEIGHT && charY + CHAR_SIZE > giftBag.y) {
        giftBag.active = false; // Collect the gift bag
        health++; 
    }
}

int main() {
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
    displayStartScreen(); // Show the start screen
    initGame();
    while (1) {
        cleardevice(); 
        drawSpikes(); 
        moveAndDrawBoards(); 
        placeCharacter(); 
        moveCharacter(); 
        detectCollision(); 
        generateGiftBag(); 
        drawGiftBag(); 
        checkGameOverConditions(); 
        if (health <= 0) {
            closegraph(); 
            _getch(); 
            break; 
        }
        FlushBatchDraw(); 
        Sleep(30); 
    }
    closegraph(); 
    return 0;
}