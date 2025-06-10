#include <LedControl.h>


int DIN = 11;
int CLK = 13;
int CS = 7;
LedControl lc = LedControl(DIN, CLK, CS, 0);


int VRx = A0;
int VRy = A1;


int leftThreshold = 150;
int rightThreshold = 850;
int upThreshold = 150;
int downThreshold = 850;


int playerRow = 7;
int playerCol = 7;


int attemptedPlayerRow = 7;
int attemptedPlayerCol = 7;


bool VirtualLEDS[8][8];


String Maze[4][4] = {
  {"R", "R", "R", "D"},
  {"D", "L", "L", "L"},
  {"R", "R", "R", "D"},
  {"R", "R", "R", "N"}};


int position[2] = {3, 3};


void playerMove()
{
  SetVLED(playerCol, playerRow, false);


  if (analogRead(VRx) < leftThreshold) {
    attemptedPlayerCol = max(playerCol - 1, 1);
  }


  if (analogRead(VRx) > rightThreshold) {
    attemptedPlayerCol = min(playerCol + 1, 7);
  }


  if (analogRead(VRy) < upThreshold) {
    attemptedPlayerRow = max(playerRow - 1, 1);
  }


  if (analogRead(VRy) > downThreshold) {
    attemptedPlayerRow = min(playerRow + 1, 7);
  }


  if (VirtualLEDS[attemptedPlayerCol][attemptedPlayerRow] == false) {
    playerRow = attemptedPlayerRow;
    playerCol = attemptedPlayerCol;
  }
  else {
    attemptedPlayerRow = playerRow;
    attemptedPlayerCol = playerCol;
  }


  SetVLED(playerCol, playerRow, true);
}


void ClearVLEDs()
{
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      VirtualLEDS[x][y] = false;
    }
  }
}


void SetVLED(int x, int y, bool state)
{
  if (0 <= x && x <= 7)
  {
    if (0 <= y && y <= 7)
    {
      VirtualLEDS[x][y] = state;
    }
  }
}


void MoveHead()
{
  int randNum = random(0, 4);
  if (randNum == 0) //Up
  {
    if (position[1] == 0)
      return;
    Maze[position[1]][position[0]] = "U";
    position[1] -= 1;
    Maze[position[1]][position[0]] = "N";
  }


  if (randNum == 1) //Right
  {
    if (position[0] == 3)
      return;
    Maze[position[1]][position[0]] = "R";
    position[0] += 1;
    Maze[position[1]][position[0]] = "N";
  }


  if (randNum == 2) //Down
  {
    if (position[1] == 3)
      return;
    Maze[position[1]][position[0]] = "D";
    position[1] += 1;
    Maze[position[1]][position[0]] = "N";
  }


  if (randNum == 3) //Left
  {
    if (position[0] == 0)
      return;
    Maze[position[1]][position[0]] = "L";
    position[0] -= 1;
    Maze[position[1]][position[0]] = "N";
  }
}


void RefreshVLEDGrid()
{
  for (int x = 0; x < 5; x++)
  {
    for (int y = 0; y < 5; y++)
    {
      SetVLED(x*2,y*2,true);
      SetVLED(x*2,y*2+1,true);
      SetVLED(x*2,y*2-1,true);
      SetVLED(x*2+1,y*2,true);
      SetVLED(x*2-1,y*2,true);
    }
  }
  for (int x = 0; x < 4; x++)
  {
    for (int y = 0; y < 4; y++)
    {
      if (Maze[y][x] == "R")
        SetVLED(x*2+1+1,y*2+1,false);
      if (Maze[y][x] == "L")
        SetVLED(x*2+1-1,y*2+1,false);
      if (Maze[y][x] == "U")
        SetVLED(x*2+1,y*2+1-1,false);
      if (Maze[y][x] == "D")
        SetVLED(x*2+1,y*2+1+1,false);
    }
  }
}


void setup()
{
  lc.shutdown(0, false); // Wake up the 7219
  lc.setIntensity(0, 15); // Full brightness
  lc.clearDisplay(0); // Clear the matrix


  ClearVLEDs();
  randomSeed(1);
  MoveHead();
  RefreshVLEDGrid();
}


void loop()
{
  playerMove();
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      lc.setLed(0, y, x, VirtualLEDS[x][y]);
    }
  }


  delay(100);
  lc.setLed(0, playerRow, playerCol, false);
  delay(100);


  if ((playerRow == 1) && (playerCol == 1)) {
      lc.clearDisplay(0); // Clear the matrix
      delay(2000);
      SetVLED(1, 1, false);
      for (int i = 0; i < 10; i++) {
        MoveHead();
      }
      RefreshVLEDGrid();
      playerRow = 7;
      playerCol = 7;
      attemptedPlayerRow = 7;
      attemptedPlayerCol = 7;
  }
}

