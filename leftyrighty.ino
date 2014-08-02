#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;

// PROGMEM STUFF

const byte logo[] PROGMEM = {
 64,30,
 B00000000,B00000000,B00000001,B11000000,B00000000,B00000000,B00000000,B00000000,
 B00110000,B00000000,B00000011,B00001000,B00000000,B00000000,B00000000,B00000000,
 B00110000,B00000000,B00000011,B00011000,B00000000,B00000000,B00000000,B00000000,
 B00110000,B00000011,B11000111,B11111111,B10000011,B00000000,B00111100,B00000000,
 B00110000,B00000110,B01100011,B00011001,B10000011,B00000000,B01101010,B00000000,
 B00110000,B00001100,B00110011,B00011000,B10000010,B00000000,B11110101,B00000000,
 B00110000,B00001100,B00110011,B00011000,B11000110,B00000000,B11111010,B00000000,
 B00110000,B00001111,B11110011,B00011000,B01000100,B00000000,B11111101,B00000000,

 B00110000,B00001100,B00000011,B00011000,B01100100,B00000000,B11111011,B00000000,
 B00110000,B00001100,B00000011,B00011000,B01101100,B00000000,B01111110,B00000000,
 B00110000,B00001100,B00110011,B00011000,B00101000,B00000000,B00111100,B00000000,
 B00110000,B00000110,B01100011,B00011000,B00111000,B00000000,B00000000,B00000000,
 B00111111,B11000011,B11000011,B01101110,B00010000,B00000000,B00000000,B00000000,
 B00000000,B00001111,B11111000,B01100000,B00110001,B10000000,B00100000,B00000000,
 B00000000,B00001100,B00001100,B00000000,B01100001,B10000000,B01100000,B00000000,
 B00000000,B00001100,B00000110,B01100001,B11011001,B10111100,B11111110,B00001100,

 B00001111,B00001100,B00000110,B01100011,B00111001,B11001110,B01100110,B00001100,
 B00011010,B10001100,B00000110,B01100110,B00011001,B10000110,B01100010,B00001000,
 B00111101,B01001100,B00001100,B01100110,B00011001,B10000110,B01100011,B00011000,
 B00111110,B10001111,B11111000,B01100110,B00011001,B10000110,B01100001,B00010000,
 B00111111,B01001100,B00110000,B01100110,B00011001,B10000110,B01100001,B10010000,
 B00111110,B11001100,B00011000,B01100110,B00011001,B10000110,B01100001,B10110000,
 B00011111,B10001100,B00001000,B01100110,B00011001,B10000110,B01100000,B10100000,
 B00001111,B00001100,B00001100,B01100011,B00111001,B10000110,B01100000,B11100000,

 B00000000,B00001100,B00000110,B01100011,B11011001,B10000110,B00111000,B01000000,
 B00000000,B00000000,B00000000,B00000000,B00011000,B00000000,B00000000,B01000000,
 B00000001,B00101111,B01001011,B11000000,B00011000,B00000000,B00000000,B11000000,
 B00000001,B10101001,B01101010,B01000111,B00110000,B00000000,B00000001,B10000000,
 B00000001,B01101111,B01011010,B01000001,B11100000,B00000000,B00000111,B00000000,
 B00000001,B00101001,B01001011,B11000000,B00000000,B00000000,B00000000,B00000000,
};

const byte ball[] PROGMEM = {
 8,8,
 B00111100,
 B01101010,
 B11110101,
 B11111010,
 B11111101,
 B11111011,
 B01111110,
 B00111100,

};

// END PROGMEM STUFF

int ball_size = 8;
int ball_left = ((LCDWIDTH/2)/2)-(ball_size/2); //ball on the left
int ball_right = (LCDWIDTH-ball_left)-(ball_size); //ball on the right
int ball_start = -9; // y start pos
int ball_end = LCDHEIGHT+8; // y end pos
int ball_speed = 1;
int max_speed = 6;
int ball_x = ball_left;
int ball_y = ball_start;
int score = 0;

// label the different views/screens
int TITLESCREEN = 0;
int MAINSCREEN = 1;
int GAMEOVER = 2;

int CURRENTSCREEN = 0;


// set the ball at either the left or right side of the screen
int leftright() {
  int x;
  if(random(0, 100)< 50) {
    x = ball_left;
  } else {
    x = ball_right; 
  }
  return x;
}

// Woo. You scored a point!
void scored() {
  ball_y = ball_start;
  ball_x = leftright();
  score = score+1;
  if(((score % 10) == 0) && score > 0) {
      if(ball_speed < max_speed){
        gb.popup(F("     SPEED UP!"),20);
        ball_speed = ball_speed + 1;
      } else {
        // don't go faster than 6px, the screen is too small for that!
        gb.popup(F("    GO! GO! GO!"),20);
      }
      gb.sound.playOK();
  } else {
    gb.sound.playTick();
  }
}

// back to the start
void resetgame() {
 gb.pickRandomSeed();
 score = 0;
 ball_speed = 1;
 ball_y = ball_start;
 leftright();
}

// Boooooo!
void lose(){
  CURRENTSCREEN = GAMEOVER;
  gb.sound.playCancel();
}

// Let's get going.
void setup(){
  gb.pickRandomSeed();
  gb.begin();
  gb.titleScreen(F("PixelGuff Presents:"),logo);
}

// GAME LOOP
void loop(){

  if(gb.update()){
    
    // instructions
    if (CURRENTSCREEN == TITLESCREEN) {
      
      gb.display.print("\n INSTRUCTIONS:\n");
      gb.display.print(" Press left for left.\n");  
      gb.display.print(" B for right.\n");
      gb.display.print(" A to start!\n");
      gb.display.print(" DON'T GET IT WRONG!\n");
      gb.display.print(" DON'T LET THEM DROP!");
      gb.display.drawRoundRect(0, 0, LCDWIDTH, LCDHEIGHT, 3);
      if(gb.buttons.pressed(BTN_A)){
        CURRENTSCREEN = MAINSCREEN;
      }
    }
    
    // the game itself
    if (CURRENTSCREEN == MAINSCREEN) {
      ball_y = ball_y + ball_speed;
    
      if (ball_y > ball_end) {
        ball_y = ball_start;
        lose(); 
      }
      
      
      if(gb.buttons.pressed(BTN_LEFT) && (ball_x == ball_left)) {
        scored();
        gb.display.fillRect(0, 0, LCDWIDTH/2, LCDHEIGHT);
      }
      
      if(gb.buttons.pressed(BTN_LEFT) && (ball_x != ball_left)) {
        if(ball_y >= 0){
          lose();
        }
      }
      
      if(gb.buttons.pressed(BTN_B) && (ball_x == ball_right)) {
        scored();
        gb.display.fillRect(LCDWIDTH/2, 0, LCDWIDTH/2, LCDHEIGHT);
      }
      
      if(gb.buttons.pressed(BTN_B) && (ball_x != ball_right)) {
        if(ball_y >= 0){
          lose(); 
        }
      }
      
      
      //draw the ball on the screen
      gb.display.drawRoundRect(0, 0, LCDWIDTH, LCDHEIGHT, 3);
      gb.display.drawFastVLine(LCDWIDTH/2, 0, LCDHEIGHT);
      gb.display.drawBitmap(ball_x, ball_y, ball);
      gb.display.print(score);
      }
    
    // YOU LOSE
    if(CURRENTSCREEN == GAMEOVER) {
      gb.display.print("\n OOPS! GAME OVER!");
      gb.display.print("\n YOU SCORED: ");
      gb.display.print(score);
      gb.display.print("\n\n\n Press A to reset.");
      gb.display.drawRoundRect(0, 0, LCDWIDTH, LCDHEIGHT, 3);
      if(gb.buttons.pressed(BTN_A)){
        CURRENTSCREEN = TITLESCREEN;
        resetgame();
      }
    }
    
    // back to the title screen on Button C.  
    if(gb.buttons.pressed(BTN_C)){
      gb.pickRandomSeed();
      gb.titleScreen(F("PixelGuff Presents:"),logo);
      resetgame();
    }
    
  }
}

