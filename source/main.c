#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define clear() printf("\e[2J\e[H");
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

u16 number;
int i = 0;
int hold = 0;
u16 guess = 0;
u16 prevguess = 0;
u32 kDown;
u32 kHeld;
static SwkbdState swkbd;
static char mybuf[60];
SwkbdButton button = SWKBD_BUTTON_NONE;
bool redraw = true;

int draw(void) 
{
  clear();
  printf("Guess the number between 0 and 10000\n");
  printf("%sTries: %d%s\n\n", YEL,i,RESET);
  printf("Guess: %d\n", guess);
  redraw = false;
  return 0;
}

int run(void) 
{
  if (!kHeld) hold = 0;
  if (redraw) draw();
  
  if (kDown & KEY_RIGHT) {
    guess++;
    if (guess > 60000) guess = 10000;
    if (guess > 10000) guess = 0;
    printf("\x1b[4;8H%d      \n", guess);
  }
  if (kDown & KEY_LEFT) {
    guess--;
    if (guess > 60000) guess = 10000;
    if (guess > 10000) guess = 0;
    printf("\x1b[4;8H%d      \n", guess);
  }
  if (kDown & KEY_UP) {
    guess += 10;
    if (guess > 60000) guess = 10000;
    if (guess > 10000) guess = 0;
    printf("\x1b[4;8H%d      \n", guess);
  }
  if (kDown & KEY_DOWN) {
    guess -= 10;
    if (guess > 60000) guess = 10000;
    if (guess > 10000) guess = 0;
    printf("\x1b[4;8H%d      \n", guess);
  }
  if (kDown & KEY_R) {
    guess += 100;
    if (guess > 60000) guess = 10000;
    if (guess > 10000) guess = 0;
    printf("\x1b[4;8H%d      \n", guess);
  }
  if (kDown & KEY_L) {
    guess -= 100;
    if (guess > 60000) guess = 10000;
    if (guess > 10000) guess = 0;
    printf("\x1b[4;8H%d      \n", guess);
  }
  if (kHeld & KEY_RIGHT) {
    hold++;
    if (hold > 60) {
      guess ++;
      if (guess > 60000) guess = 10000;
      if (guess > 10000) guess = 0;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  if (kHeld & KEY_LEFT) {
    hold++;
    if (hold > 60) {
      guess--;
      if (guess > 60000) guess = 10000;
      if (guess > 10000) guess = 0;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  if (kHeld & KEY_UP) {
    hold++;
    if (hold > 60) {
      guess += 10;
      if (guess > 60000) guess = 10000;
      if (guess > 10000) guess = 0;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  if (kHeld & KEY_DOWN) {
    hold++;
    if (hold > 60) {
      guess -= 10;
      if (guess > 60000) guess = 10000;
      if (guess > 10000) guess = 0;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  if (kHeld & KEY_R) {
    hold++;
    if (hold > 60) {
      guess += 100;
      if (guess > 60000) guess = 10000;
      if (guess > 10000) guess = 0;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  if (kHeld & KEY_L) {
    hold++;
    if (hold > 60) {
      guess -= 100;
      if (guess > 60000) guess = 10000;
      if (guess > 10000) guess = 0;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  

  if (kDown & KEY_X) 
  {
    swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 8);
		swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
		swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
		swkbdSetNumpadKeys(&swkbd, '#', '*');
		button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
    if (button != SWKBD_BUTTON_NONE) {
      guess = atoi(mybuf);
      if (guess > 60000) guess = 0;
      if (guess > 10000) guess = 10000;
      printf("\x1b[4;8H%d      \n", guess);
    }
  }
  if (kDown & KEY_A) {
    i++;
    prevguess = guess;
    draw();
    printf("%sYou guessed: %d%s\n\n", CYN,guess,RESET);
    if (guess == number) {
      printf("%sYou guessed the number!%s\n",GRN,RESET);
      printf("It took you %s%d tries%s\n\n", YEL,i,RESET);
      printf("Press A to play again\n");
      printf("Press START to exit\n");
      i = 0;
      number = rand() % 10000;
    } else if (guess > number) {
      printf("Lower !\n");
    } else if (guess < number) {
      printf("Higher !\n");
    }
  }  
  return 0;
}

int main(int argc, char **argv)
{
  PrintConsole topScreen, bottomScreen;
	gfxInitDefault();
  srand(time(NULL));
  number = rand() % 10000;
	
	consoleInit(GFX_TOP, &topScreen);
  consoleInit(GFX_BOTTOM, &bottomScreen);
  consoleSelect(&bottomScreen);
  printf("%sDPAD Right/Left:%s Increment the selected number by 1\n\n", MAG,RESET);
  printf("%sDPAD Up/Down:%s Increment the selected\nnumber by 10\n\n", MAG,RESET);
  printf("%sL/R:%s Increment the selected number \nby 100\n\n", MAG,RESET);
  printf("%sA:%s Guess the number\n\n", MAG,RESET);
  printf("%sX:%s Enter a number using the \nsoftware keyboard\n\n", MAG,RESET);
  printf("%sB/Start:%s Exit the game\n", MAG,RESET);
	consoleSelect(&topScreen);

	while (aptMainLoop())
	{
		hidScanInput();
		kDown = hidKeysDown();
    kHeld = hidKeysHeld();
		if (kDown & KEY_START || kDown & KEY_B) break;
    run();
		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}

