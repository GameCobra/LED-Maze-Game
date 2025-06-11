#include <LedControl.h>


int shiftX = 0;
int shiftY = 0;

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


int playerRow = 5;
int playerCol = 5;


int attemptedPlayerRow = 5;
int attemptedPlayerCol = 5;


bool VirtualLEDS[14][14];


String Maze[7][7] = {
  {"R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "D"},
  {"R", "R", "R", "R", "R", "N"}};


int position[2] = {5, 5};


void playerMove()
{
  SetVLED(playerCol, playerRow, false);


  if (analogRead(VRx) < leftThreshold) {
    attemptedPlayerCol = playerCol - 1;
  }


  if (analogRead(VRx) > rightThreshold) {
    attemptedPlayerCol = playerCol + 1;
  }


  if (analogRead(VRy) < upThreshold) {
    attemptedPlayerRow = playerRow - 1;
  }


  if (analogRead(VRy) > downThreshold) {
    attemptedPlayerRow = playerRow + 1;
  }
/*
  if (shiftX == 0 && attemptedPlayerCol == 7)
  {
    attemptedPlayerRow = playerRow;
    attemptedPlayerCol = playerCol;
    shiftX = 5;
  }*/

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
  for (int x = 0; x < 14; x++)
  {
    for (int y = 0; y < 14; y++)
    {
      VirtualLEDS[x][y] = false;
    }
  }
}


void SetVLED(int x, int y, bool state)
{
  if (0 <= x && x < 14)
  {
    if (0 <= y && y < 14)
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
    if (position[0] == 5)
      return;
    Maze[position[1]][position[0]] = "R";
    position[0] += 1;
    Maze[position[1]][position[0]] = "N";
  }


  if (randNum == 2) //Down
  {
    if (position[1] == 5)
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
  for (int x = 0; x < 14; x++)
  {
    for (int y = 0; y < 14; y++)
    {
      SetVLED(x*2,y*2,true);
      SetVLED(x*2,y*2+1,true);
      SetVLED(x*2,y*2-1,true);
      SetVLED(x*2+1,y*2,true);
      SetVLED(x*2-1,y*2,true);
    }
  }
  for (int x = 0; x < 7; x++)
  {
    for (int y = 0; y < 7; y++)
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

bool GetVLEDValue(int x, int y)
{
  if (0 <= x && x < 14 && 0 <= y && y < 14 )
    return VirtualLEDS[x][y];
  else
    return false;
}

void setup()
{
  Serial.begin(9600);
  lc.shutdown(0, false); // Wake up the 7219
  lc.setIntensity(0, 15); // Full brightness
  lc.clearDisplay(0); // Clear the matrix


  ClearVLEDs();
  randomSeed(1);
  //MoveHead();
  RefreshVLEDGrid();
  for (int x = 0; x < 14; x++)
  {
    for (int y = 0; y < 14; y++)
    {
      Serial.print(GetVLEDValue(x, y) == true ? "1" : "0");
    }
    Serial.print("\n");
  }
}

void RefreshScreen()
{
  //lc.clearDisplay(0); // Clear the matrix
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      lc.setLed(0, y, x, GetVLEDValue(x + shiftX, y + shiftY));
    }
  }
}

void loop()
{
  playerMove();
  RefreshScreen();

  delay(100);
  lc.setLed(0, playerRow - shiftY, playerCol - shiftX, false);
  delay(100);

  Serial.print(playerCol);
  Serial.print(playerRow);
  if (playerCol >= 7)
  {
    /*for (int i = 0; i < 7; i++)
    {
      shiftX = i;
      //delay(25);
      RefreshScreen();
    }*/
    shiftX = 6;
  }
  if (playerCol <= 6)
  {
    shiftX = 0;
  }
  if (playerRow >= 7)
  {
    shiftY = 6;
  }
  if (playerRow <= 6)
  {
    shiftY = 0;
  }

  //shiftX += 1;
  //shiftY += 1;


  if ((playerRow == 1) && (playerCol == 1)) {
      lc.clearDisplay(0); // Clear the matrix
      delay(2000);
      SetVLED(1, 1, false);
      for (int i = 0; i < 10; i++) {
        MoveHead();
      }
      RefreshVLEDGrid();
      playerRow = 5;
      playerCol = 5;
      attemptedPlayerRow = 5;
      attemptedPlayerCol = 5;
  }
}

