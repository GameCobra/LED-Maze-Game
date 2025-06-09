#include <LedControl.h>

int DIN = 11;
int CS = 7;
int   CLK = 13;
LedControl lc=LedControl(DIN, CLK, CS,0);

bool VirtualLEDS[8][8];

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

String Maze[4][4] = {
  {"R", "R", "R", "D"}, 
  {"D", "L", "L", "L"}, 
  {"R", "R", "R", "D"}, 
  {"R", "R", "R", "N"}};

int position[2] = {3, 3};

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

void setup() {
  pinMode(8, OUTPUT);
  ClearVLEDs();
  randomSeed(1);
  //MoveHead();
  
  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
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
  for (int i = 100; i < 1000; i++)
  {
    tone(8, i);
    delay(10);
    //noTone(8);
    //delay(500);
  }
}

void loop() {
  for (int i = 0; i < 10; i++)
    MoveHead();
  RefreshVLEDGrid();
  //delay(250);
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      lc.setLed(0,y,x,VirtualLEDS[x][y]);
    }
  }
  
}

