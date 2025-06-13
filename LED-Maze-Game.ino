#include <LedControl.h>


int DIN = 11;
int CLK = 13;
int CS = 7;
LedControl lc = LedControl(DIN, CLK, CS, 0);


int VRx = A0;
int VRy = A1;


int leftThreshold = 190;
int rightThreshold = 810;
int upThreshold = 190;
int downThreshold = 810;


int playerRow = 17;
int playerCol = 17;


int attemptedPlayerRow = 16;
int attemptedPlayerCol = 16;


int counter = 0;


bool VirtualLEDS[19][19];


String Maze[9][9] = {
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "R", "R", "R", "N"}};


int position[2] = {8, 8};


int xOffset;
int yOffset;


void playerMove()
{
  SetVLED(playerCol, playerRow, false);


  if (analogRead(VRx) < leftThreshold) {
    attemptedPlayerCol = max(playerCol - 1, 1);
  }


  if (analogRead(VRx) > rightThreshold) {
    attemptedPlayerCol = min(playerCol + 1, 19);
  }


  if (analogRead(VRy) < upThreshold) {
    attemptedPlayerRow = max(playerRow - 1, 1);
  }


  if (analogRead(VRy) > downThreshold) {
    attemptedPlayerRow = min(playerRow + 1, 19);
  }


  if (VirtualLEDS[attemptedPlayerCol][attemptedPlayerRow] == false) {
    playerRow = attemptedPlayerRow;
    playerCol = attemptedPlayerCol;
  }
  else {
    attemptedPlayerRow = playerRow;
    attemptedPlayerCol = playerCol;
  }


  xOffset = 0;
  yOffset = 0;
  if (playerRow > 6) {
    yOffset = 6;
  }
  if (playerRow > 12) {
    yOffset = 12;
  }
  if (playerCol > 6) {
    xOffset = 6;
  }
  if (playerCol > 12) {
    xOffset = 12;
  }


  SetVLED(playerCol, playerRow, true);
}


void ClearVLEDs()
{
  for (int x = 0; x < 19; x++)
  {
    for (int y = 0; y < 19; y++)
    {
      VirtualLEDS[x][y] = false;
    }
  }
}


void SetVLED(int x, int y, bool state)
{
  if (0 <= x && x < 19)
  {
    if (0 <= y && y < 19)
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
    if (position[0] == 8)
      return;
    Maze[position[1]][position[0]] = "R";
    position[0] += 1;
    Maze[position[1]][position[0]] = "N";
  }


  if (randNum == 2) //Down
  {
    if (position[1] == 8)
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
  ClearVLEDs();
  for (int x = 0; x < 12; x++)
  {
    for (int y = 0; y < 12; y++)
    {
      SetVLED(x*2, y*2, true);
      SetVLED(x*2, y*2+1, true);
      SetVLED(x*2, y*2-1, true);
      SetVLED(x*2+1, y*2, true);
      SetVLED(x*2-1, y*2, true);
    }
  }


  for (int i = 0; i < 19; i++) {
    SetVLED(18, i, true);
    SetVLED(i, 18, true);
  }


  for (int x = 0; x < 9; x++)
  {
    for (int y = 0; y < 9; y++)
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
  //MoveHead();
  RefreshVLEDGrid();
}


void loop()
{
  playerMove();


  if (counter % 2 == 0) {
    SetVLED(1, 1, true);
    SetVLED(playerCol, playerRow, true);
  }
  else {
    SetVLED(1, 1, false);
    SetVLED(playerCol, playerRow, false);
  }
  counter += 1;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      lc.setLed(0, y, x, VirtualLEDS[x + xOffset][y + yOffset]);
    }
  }


  if ((playerRow == 1) && (playerCol == 1)) {
      lc.clearDisplay(0); // Clear the matrix
      delay(1000);
      bool rawMessage[8][80] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
      for (int i = -8; i < 88; i++)
      {
        for (int y = 0; y < 80; y++)
        {
          for (int x = 0; x < 8; x++)
          {
            lc.setLed(0, x, y - i, rawMessage[x][y]);
          }
        }
      }
      delay(1000);
      lc.clearDisplay(0);
      SetVLED(1, 1, false);
      for (int i = 0; i < 1000; i++) {
        MoveHead();
      }
      RefreshVLEDGrid();
      playerRow = 17;
      playerCol = 17;
      attemptedPlayerRow = 17;
      attemptedPlayerCol = 17;
  }
}
