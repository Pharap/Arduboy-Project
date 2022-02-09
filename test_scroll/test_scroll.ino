#include <Arduboy2.h>
Arduboy2 arduboy;

Sprites sprites;

#include "sprites.h"

#define WORLD_WIDTH 17
#define WORLD_HEIGHT 8

uint8_t mapx = 0;
uint8_t level = 0;
uint8_t sky = 0;
uint8_t ground = 11;
uint8_t midBlock = 11;
uint8_t velocity = 0;

uint8_t worldTime = 250;
uint8_t slowRun = 255;
uint8_t slowRunCountdown = 8;
uint8_t levelType = 0;
uint8_t speedChange = 8;
uint8_t levelTimer = 255;
uint8_t coins = 0;
int screenx = 0;
int playerx = 16;
int realplayerx = 0;
int playery = 40;
int gamestate = 0;
uint8_t gravity = 0;
bool canMove = true;
bool isGrounded = true;
bool isFalling = true;
uint8_t plusJump = 0;
uint8_t levelThemeBlocks;

const uint8_t levelInfo []{
  113,
  123,
  113,
  124,
  113,
  105,
  113,
  124,
  123,
  123,
  123,
  124,
  113,
  123,
  113,
  124,
  113,
  113,
  113,
  124,
  123,
  123,
  123,
  124,
  113,
  105,
  113,
  124,
  113,
  113,
  113,
  124,
};

struct Pipe
{
  uint8_t x;
  uint8_t y;
  uint8_t height;
};

const Pipe pipeInfo [6] {
  { 13, 10, 1 },
  { 23, 11, 1 },
  { 31, 12, 1 },
  { 42, 12, 1 },
  { 148, 10, 1 },
  { 165, 10, 1 },
};


const uint8_t gapInfo [3][2] {
  {54,2},
  {71,3},
  {138,2},
};

const uint8_t blockInfo [6][3] {
  //{0,4,14},
  {0,144},
  {5,144},
  //{31,3},
  //{42,3},
  //{148,1},
  //{165,1},
};

const uint8_t stairInfo [6][5] {
  {0,4,14},
  {5,4,14},
  {31,3},
  {42,3},
  {148,1},
  {165,1},
};


//GRAPHICS
const uint8_t logoSuper[] PROGMEM {
// width, height,
32, 8,
0x4c, 0xde, 0xde, 0xf6, 0xf6, 0x64, 0x00, 0x7e, 0xfe, 0xe0, 0xfe, 0x7e, 0x00, 0xfe, 0xfe, 0x06, 0x3e, 0x3e, 0x1c, 0x00, 0x7c, 0xfe, 0xd6, 0xd6, 0xc6, 0x00, 0xfe, 0xfe, 0x02, 0xfe, 0xec, 0x00, 
};

const uint8_t logoMarioBros[] PROGMEM {
// width, height,
32, 16,
//logoMario
0xf8, 0xfc, 0xfc, 0x1c, 0xf8, 0xfc, 0x1c, 0xfc, 0xf8, 0x00, 0xf8, 0xfc, 0x1c, 0xfc, 0xf8, 0x00, 0xfc, 0xfc, 0x0c, 0xfc, 0xfc, 0xf8, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0xf8, 0xfc, 0x0c, 0xfc, 0xf8, 0xff, 0xff, 0xff, 0x00, 0x03, 0x03, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x18, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0xfd, 0xf8, 0x00, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xff, 0xc0, 0xff, 0x7f, 

//logoBros
0xfc, 0xfc, 0xfc, 0x0c, 0xfc, 0xfc, 0x78, 0x00, 0xfc, 0xfc, 0xfc, 0x0c, 0xfc, 0xfc, 0x78, 0x00, 0xf8, 0xfc, 0x0c, 0xfc, 0xf8, 0x00, 0xf8, 0xfc, 0xfc, 0x8c, 0x3c, 0x38, 0x00, 0x00, 0x00, 0x00, 
0xff, 0xff, 0xff, 0x00, 0xff, 0xfe, 0x7c, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xfe, 0xfc, 0x00, 0x7f, 0xff, 0xc0, 0xff, 0x7f, 0x00, 0x78, 0xf9, 0xe3, 0xff, 0xff, 0x7e, 0x00, 0xe0, 0xe0, 0xe0,
};

const uint8_t tiles[] PROGMEM {
// width, height,
8, 8,

//grey(0)
0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,

//white(1)
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

//black(2)
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

//groundBlock (3)
0xbe, 0x7e, 0x7e, 0xbe, 0x88, 0xf6, 0xf6, 0x00, 

//castleBlock (4)
0x0e, 0xe0, 0xee, 0xee, 0x0e, 0xe0, 0xee, 0xee, 

//underwaterBlock (5)
0xad, 0xc1, 0x65, 0x6e, 0x9c, 0x51, 0x6d, 0x1d, 

//star(6)
0x7B, 0x93, 0x83, 0xC0, 0xC0, 0x83, 0x93, 0x7B,

//pipe1 (7)
0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff,  

//pipe2 (8)
0x00, 0xff, 0xff, 0x55, 0xaa, 0x55, 0x00, 0xff, 

//pipe3 (9)
0x00, 0x02, 0x7e, 0x00, 0x00, 0x00, 0x7e, 0x02, 

//pipe4 (10)
0x7e, 0x7e, 0x7e, 0x7e, 0x56, 0x2a, 0x56, 0x00, 

//coin (11)
0xff, 0x81, 0x3c, 0x42, 0x3c, 0x00, 0x81, 0xff, 

//squareBlock (12)
0xAA, 0x7F, 0x3E, 0x3F, 0x3E, 0x3F, 0x01, 0x00, 

//brickBlock (13)
0x61, 0x6C, 0x0D, 0x6D, 0x61, 0x6C, 0x0D, 0x6D,

//questionMarkBlock (14)
0x00, 0x7E, 0x76, 0x7A, 0x2A, 0x76, 0x7E, 0x00, 

//questionMarkUsedBlock (15)
0x81, 0x42, 0x00, 0x00, 0x00, 0x00, 0x42, 0x81, 

//mushroom (16)
0xF3, 0xE1, 0x8C, 0x6C, 0x62, 0x80, 0xE9, 0xF3,

//flower(17)
0xED, 0x8A, 0x9A, 0x04, 0x04, 0x9A, 0x8A, 0xED,
};

const uint8_t CoinMask[] PROGMEM = {
  0x00, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x00,
  
};

const uint8_t playerBig[] PROGMEM =
{
  // Width, Height,
  8, 16,
  
  // Frame 0 (Idle)
  0x93, 0x41, 0xf1, 0xf9, 0xa1, 0x99, 0x33, 0xbb, 0x63, 0x10, 0x04, 0xc0, 0xc0, 0x04, 0x11, 0x63, 
};


//WORLD ARRAY
uint8_t world[WORLD_HEIGHT][WORLD_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1},
  {1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1},
  {1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1},
  {1,1,1,2,2,2,2,2,2,2,2,2,2,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
};

/*
uint8_t nextColumn[WORLD_HEIGHT][1] = {
  {0},
  {0},
  {3},
  {0},
  {0},
  {0},
  {0},
  {0},
};
*/

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(60);
}

void drawGround(){  

if(mapx == 0){
  for(uint8_t i = 0; i < 16;i++){
      world[7][i] = levelInfo[level]%10;
      }
}

  //draws ground until spacePosition is reached (after the first 16 blocks)
  if(mapx>0){
   // for(int i = mapx; i < spacePosition;i++){
      world[7][16] = levelInfo[level]%10;
   //   }
     
  }
  
   // return;
}

void drawGaps(uint8_t spacePosition, uint8_t spaceWidth){ 
if(mapx >= spacePosition){
  if(mapx - spacePosition < spaceWidth){
    world[7][16] = sky; 
  }
  
}


}

void buildCastle(uint8_t xSpot){  
  //builds castle
  buildBlocks(xSpot,2,5,3,13);
  buildBlocks(xSpot + 1,4,3,2,13); 
}

/*
void buildStairs(int xSpot, int ySpot, int blockWidth, int blockType){  
    //builds a square section of blocks out of x,y,w,h,and type
    for(int i = blockWidth; i > 0; i--){ //horizontal
      for(int j = 0; j < i; j++){ //vertical
        world[i + ySpot][j + xSpot] = blockType;
        }
    }
    
    return;
}

void buildStairs(int xSpot, int ySpot, int blockWidth, int blockType, bool faceRight){  
    //builds a square section of blocks out of x,y,w,h,and type
    if(mapx >= xSpot-1){
    for(int i = blockWidth; i > 0; i--){ //horizontal
      for(int j = 0; j < i; j++){ //vertical
        if(faceRight == true){
          world[i + ySpot][4-j + xSpot] = blockType;
          }
        else{
          world[i + ySpot][j + xSpot] = blockType;
          }
        } 
    }
    /*
    for(int j = 0; j < 8; j++){ //vertical
          world[j][16] = 0;
        }
        
    }
}



void buildStairs(int xSpot, int ySpot, int blockWidth, int blockType, bool faceRight){  
    //builds a square section of blocks out of x,y,w,h,and type
    if(mapx >= xSpot-1){
      if(blockWidth > mapx-xSpot){
    for(int i = blockWidth; i > 0; i--){ //vertical columns
        if(faceRight == true){
          world[i + ySpot][16] = blockType;
          }
        else{
          world[i + ySpot][16] = blockType;
          }
        } 
    }
    
    for(int j = 0; j < 8; j++){ //vertical
          world[j][16] = 0;
        }
        
    }
}
*/

void buildStairs(int xSpot, int ySpot, int blockWidth, int blockType, bool faceRight){
    //can probably limit this int
    int z = mapx - xSpot+1;
    int w = xSpot - mapx;
  if(mapx >= xSpot-1){
      //horizontal
    if(faceRight == true){
      for(int j = 0; j < z; j++){ //vertical columns
        if(z <= blockWidth){
        world[8-ySpot - j][16] = blockType;
        }
      }
    }
    else if(faceRight == false){
      for(int j = blockWidth; j > 0; j--){ //vertical columns
        if(z <= blockWidth){
        world[8-ySpot - j][16] = blockType;
        }
      }
    }
      //this can be cleaned up a lot...this if statement used to be an else if inside of the above for loop
      if(z==blockWidth+1){
        for(int i = 0; i < 8; i++){
          world[8-ySpot - i][16] = sky;
        }
      }
    }
}

void buildBlocks(uint8_t xSpot, uint8_t ySpot, uint8_t blockWidth, uint8_t blockHeight, uint8_t blockType){  
    //builds a square section of blocks out of x,y,w,h,and type
  if(mapx >= xSpot-1){
    for(uint8_t i = 0; i < blockHeight; i++){  //vertical
      for(uint8_t j = 0; j < blockWidth; j++){  //horizontal
        if(mapx< xSpot - 1 + blockWidth){
          world[6-ySpot + i][16] = blockType;
        }
        else{
          world[6-ySpot + i][16] = sky;
          break;
        }
      }
    }
  }
}


void buildBlock(uint8_t xSpot, uint8_t blockTypeySpot){
    //builds a square section of blocks out of x,y,w,h,and type
    
  if(mapx == xSpot){
          world[8-blockTypeySpot%10][16] = blockTypeySpot/10;
  }
  else if(mapx == xSpot+1){
          world[8-blockTypeySpot%10][16] = sky;
  }
}

void newLevel(){
  sky = levelInfo[level]/10%10;
  levelTimer = 250;
  mapx = 0;
  buildSky();
  
  
  
  
}

/*
void buildBlock(uint8_t xSpot, uint8_t ySpot, uint8_t blockType){
    //builds a square section of blocks out of x,y,w,h,and type
  if(mapx == xSpot){
          nextColumn[8-ySpot][0] = blockType;
  }
  
}
*/

void buildPipe(const Pipe & pipe) {
  if(mapx == pipe.x - 1){
    //builds a pipe, input x position and height of pipe
    for(uint8_t i = 0; i < pipe.height + 2; i++){
      world[7 - i - pipe.y][16] = 7;
    }
    world[7 - pipe.height-1 - pipe.y][16] = 9;
  }
  
  else if(mapx == pipe.x){
    for(int i = 0; i < pipe.height + 2; i++){
        world[7 - i - pipe.y][16] = 8; 
        world[7 - pipe.height-1 - pipe.y][16] = 10;
    }
  }
  else if(mapx==pipe.x + 1){
    for(int i = 0; i < pipe.height + 1; i++){
        world[7 - i - pipe.y][16] = levelInfo[level]/10%10;
        world[7 - pipe.height-1 - pipe.y][16] = levelInfo[level]/10%10;
    }
  }
}

void piper(){
   //gaps
    //for(uint8_t y = 0; y<=3;y++){
   //   drawGaps(gapInfo[y][0],gapInfo[y][1]);
    //}
    //single block
    for(uint8_t y = 0; y<=5;y++){
      buildBlock(blockInfo[y][0],blockInfo[y][1]);
    }
  //pipes
    for(uint8_t y = 0; y<=5;y++){
      buildPipe(pipeInfo[y]);
    }
  
 
  
  
}




void scrollLeft() {
  /*
    for(int i = 0;i<8;i++){
      world[i][16] = nextColumn[i][0];
    }
    */
    if(screenx % speedChange == 0){
    mapx++;
    for(size_t x = 1; x < WORLD_WIDTH; ++x){
      for(size_t y = 0; y < WORLD_HEIGHT; ++y){
        world[y][x - 1] = world[y][x];  
      } 
    }
  }     
}


void controls() {
  
  if(arduboy.pressed(RIGHT_BUTTON)){
    if(canMove == true){
      if(worldTime % 3 == 0){
        if(slowRunCountdown > 1){
          slowRunCountdown--;
        } 
      }
     

      

    
      if(slowRun%slowRunCountdown == 0){
        playerx++;
      }
      
      
   //   if(playerx-screenx < 56){
   //   }
      if(playerx-screenx >= 56){
        screenx++;
        scrollLeft();
      }
    }   
  }
  else{
    if(slowRunCountdown < 8){
          slowRunCountdown++;

    }
  }

  if(arduboy.pressed(LEFT_BUTTON)){
    if(canMove == true){
      if(playerx-screenx > 0){
              playerx--;
      }
    }   
  }
  

  if(arduboy.justPressed(UP_BUTTON)){
    
    level++;
    newLevel();

    //resets from level 32 (31) to 1 (0)
    if(level > 31){
      level = 0;
    }
      /*for(size_t y = 0; y < WORLD_HEIGHT; ++y){
      world[y][16] = 1;
      }
*/
      
  }

  if(arduboy.justPressed(DOWN_BUTTON)){
    if(level == 0){
      level = 32;
    }
    level--;
    newLevel();
      //for(size_t y = 0; y < WORLD_HEIGHT; ++y){
      //world[y][16] = 0;
      //}
  }

  

  if(arduboy.pressed(B_BUTTON)){ 
  
    
    
    
    
    
      playery -= 3;
    
    
    
  }
  //else{
   // playery-=2;
  //}
}

void gravityCount(){
  if(plusJump<5){
    gravity++;
    if(gravity % 10 == 0){
      plusJump++;
    }
  }
    
    playery += plusJump;
  }


void collision(){
  //constant clock for the game world
  worldTime--;
  
//level timer
  if(worldTime%25 == 0){
    levelTimer--;
  }

  if(levelTimer == 0){
    levelTimer = 250;
  }

  
  //yoooo make one of these into its own function so you can call it and replace block to check
  if(world[playery / 8+2][(playerx-screenx) / 8] == sky){
    isGrounded = false;
    playery++;
    gravityCount();
  }
  else if(playery>=48){
    playery++;
  }

  if(world[playery / 8+2][(playerx-screenx) / 8] != sky){
    if(playery > 0){
      isGrounded = true;
    gravity = 0;
    plusJump = 0;
    }
    
  }
  
  //for being able to move right
  if(world[playery / 8+1][(playerx-screenx) / 8+1] != sky){
    if(playery>0){
      canMove = false;
    }
    
  }
  
  else{
    canMove = true;
  }
  if(world[playery / 8][(playerx-screenx) / 8] == 11){
    coins++;
    world[playery / 8][(playerx-screenx) / 8] = sky;
  }
  if(playery == 80){
    mapx = 0;
    playery = 0;
  }

  if(plusJump){
    isFalling = true;
  }
  else{
    isFalling = false;
  }
}

void buildSky(){
   //builds sky
  //sky = levelInfo[1];

  
  for (uint8_t y = 0; y < 8; y++) {
    for (uint8_t x = 0; x < 17; x++) {
      //world[y][x] = levelInfo[level] / 100;
      world[y][x] = sky;
      
    }
  }
}


void graphics() {

 
  
  const int tileswide = WIDTH / 8 + 1;
  const int tilestall = HEIGHT / 8 + 1;

  for (uint8_t y = 0; y < tilestall; y++) {
    for (uint8_t x = 0; x < tileswide; x++) {
      Sprites::drawOverwrite(x * 8 - screenx % 8, y * 8, tiles, world[y][x]);
    }
  }

  //prints timer
  arduboy.setCursor(100,0);
  arduboy.print(levelTimer);
  
  //prints player
  Sprites::drawOverwrite(playerx-screenx,playery,playerBig,0);



  arduboy.setCursor(0,0);
  arduboy.print("MAP x = ");
  arduboy.print(mapx);

  arduboy.setCursor(64,0);
 // arduboy.print("Level: ");
  //arduboy.print(level + 1);
  //arduboy.print(plusJump);
  arduboy.print(isGrounded);
  //arduboy.print(playery);
  arduboy.print(isFalling);

  //arduboy.print(worldTime);
  
  //arduboy.print(coins);
  //arduboy.print((playerx-screenx)/8);
  //arduboy.print(levelInfo[level]/10%10);
    //arduboy.print(levelInfo[level]);

  
 //arduboy.print(678/10%10);
 
}

void titleScreen(){
  for (uint8_t y = 0; y < 8; y++) {
    for (uint8_t x = 0; x < 16; x++) {
      Sprites::drawOverwrite(x * 8, y * 8, tiles, world[y][x]);
    }
  }

  arduboy.drawPixel(26,10,WHITE);
  arduboy.drawPixel(101,10,WHITE);
  arduboy.drawPixel(26,37,WHITE);
  arduboy.drawPixel(101,37,WHITE);
  
  Sprites::drawOverwrite(30, 10, logoSuper, 0);
  Sprites::drawOverwrite(30, 20, logoMarioBros, 0);
  Sprites::drawOverwrite(68, 20, logoMarioBros, 1);

  
  if(arduboy.justPressed(B_BUTTON)){
        newLevel();
        gamestate = 2;
      }
}

void gameplay(){
  //buildSky();
  controls();
  collision();
  graphics();
  drawGround();
  //buildCastle(15);
  buildBlocks(5,2,5,1,13);
  buildStairs(119,2,4,12,true);
  buildStairs(133,2,5,12,true);
 
  buildStairs(167,2,6,12,true);
  buildCastle(186);

 
  //buildBlock(0,4,14);
  piper();
  
  drawGaps(gapInfo[0][0],gapInfo[0][1]);
  drawGaps(gapInfo[1][0],gapInfo[1][1]);
  drawGaps(gapInfo[2][0],gapInfo[2][1]);
  //buildBlock(0,144);
  buildBlocks(25,4,5,1,11);

  Sprites::drawOverwrite(24, 24, tiles, 11);
  Sprites::drawExternalMask(24, 24, tiles, CoinMask, 11, 0);
  
  
  }

void gameloop() {
  switch(gamestate){
  case 0:
    titleScreen();
    break;

  case 1:
    //midScreen();
    break;

  case 2:
    gameplay();
    break;
    
  case 3:
  //pause screen
    //pauseScreen();
    break;
 
  }
}

void loop() {
  // put your main code here, to run repeatedly:
    if(!(arduboy.nextFrame())) {
    return;
  }
 // arduboy.clear();
  arduboy.pollButtons();
  gameloop();

  arduboy.display();


}
