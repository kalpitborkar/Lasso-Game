#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

int number_of_lives = 3;//number of lives for level 4
int level_number;//helps keep record of which level is selected

double coin_speed;
double coin_ay;

//defining the starting position of coin
int coin_start_position = 450;

//to check level failed
int level_failed = 0;

//for back to front page mechanism
int is_back =0;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void resetCoin();

}; // End class Coin

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define COIN_START_POSITION 450
#define COIN_START_POSITION_FOR_LEVEL_3 250

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_SPEED1 140 //defined for level 4
#define COIN_ANGLE_DEG 90
#define COIN_ANGLE_DEG1 70 //defined for parabola level(level 3)

#define LASSO_G 30
#define COIN_G 30
#define COIN_G1 0 //define for level 4

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  int num_coins;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  int getNumCoins() { return num_coins; }

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  coin_start_x = (coin_start_position);
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {


  if(level_number==4){
    coin_speed = COIN_SPEED1;
    }
  else{
    coin_speed = COIN_SPEED;
    }

  double coin_angle_deg = COIN_ANGLE_DEG;
  if(level_number==3){                       //changing angle for level 3 (parabola)
  coin_angle_deg = COIN_ANGLE_DEG1;
  }

  coin_ax = 0;
  if(level_number==4){
    coin_ay = COIN_G1;
    }
  else{
    coin_ay = COIN_G;
    }

  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(0,128,128));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(0,128,128));

  lasso_band.setColor(COLOR(0,128,128));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin();
  }
  the_coin=NULL;
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();

  //checking distance of lasso from bombs for level 5
  double distance_b1;
  distance_b1 = pow(pow(lasso_x-375,2) + pow(lasso_y-280,2),0.5);
  if(distance_b1 <= LASSO_RADIUS){
  level_failed = 1;
  //level failed
  }

  double distance_b2;
  distance_b2 = pow(pow(lasso_x-525,2) + pow(lasso_y-280,2),0.5);
  if(distance_b2 <= LASSO_RADIUS){
  level_failed = 1;
  //level failed
  }

  double distance_b3;
  distance_b3 = pow(pow(lasso_x-375,2) + pow(lasso_y-400,2),0.5);
  if(distance_b3 <= LASSO_RADIUS){
  level_failed = 1;
  //level failed
  }

  double distance_b4;
  distance_b4 = pow(pow(lasso_x-525,2) + pow(lasso_y-400,2),0.5);
  if(distance_b4 <= LASSO_RADIUS){
  level_failed = 1;
  //level failed
  }


  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);


  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);

  }
} // End Lasso::check_for_coin()


main_program {

  double getXPos();
  char c;

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);

  //bg color
{
  Rectangle bg_color(500,500,1000,1000);
  bg_color.setColor(COLOR(255,153,153));
  bg_color.setFill(true);
  bg_color.imprint();

}

  //game intro
{
  //segment 1
  Circle intro_1(400,25,10);
  intro_1.setColor(COLOR("yellow"));
  intro_1.setFill(true);
  intro_1.imprint();

  int coin_animation_x_1 = 50, coin_animation_y_1 = 0;
  repeat(10){
  intro_1.move(coin_animation_x_1,coin_animation_y_1);
  intro_1.imprint();
  coin_animation_x_1 -= 5;
  coin_animation_y_1 += 5;
  }
}
{
  //segment 2
  Circle intro_2(675,250,10);
  intro_2.setColor(COLOR("yellow"));
  intro_2.setFill(true);
  intro_2.imprint();

  int coin_animation_x_2 = 0, coin_animation_y_2 = 50;
  repeat(10){
  intro_2.move(coin_animation_x_2,coin_animation_y_2);
  intro_2.imprint();
  coin_animation_x_2 -= 5;
  coin_animation_y_2 -= 5;
  }
}
{
  //segment 3
  Circle intro_3(400,525,10);
  intro_3.setColor(COLOR("yellow"));
  intro_3.setFill(true);
  intro_3.imprint();

  int coin_animation_x_3 = -50, coin_animation_y_3 = 0;
  repeat(10){
  intro_3.move(coin_animation_x_3,coin_animation_y_3);
  intro_3.imprint();
  coin_animation_x_3 += 5;
  coin_animation_y_3 -= 5;
  }
}
{
  //segment 4
  Circle intro_4(125,300,10);
  intro_4.setColor(COLOR("yellow"));
  intro_4.setFill(true);
  intro_4.imprint();

  int coin_animation_x_4 = 0, coin_animation_y_4 = -50;
  repeat(10){
  intro_4.move(coin_animation_x_4,coin_animation_y_4);
  intro_4.imprint();
  coin_animation_x_4 += 5;
  coin_animation_y_4 += 5;
  }
}
{
  //segment 5
  Circle intro_5(400,75,10);
  intro_5.setColor(COLOR("yellow"));
  intro_5.setFill(true);
  intro_5.imprint();

  int coin_animation_x_5 = 40, coin_animation_y_5 = 0;
  repeat(10){
  intro_5.move(coin_animation_x_5,coin_animation_y_5);
  intro_5.imprint();
  coin_animation_x_5 -= 4;
  coin_animation_y_5 += 4;
  }
}
{
  //segment 6
  Circle intro_6(620,255,10);
  intro_6.setColor(COLOR("yellow"));
  intro_6.setFill(true);
  intro_6.imprint();

  int coin_animation_x_6 = 0, coin_animation_y_6 = 40;
  repeat(10){
  intro_6.move(coin_animation_x_6,coin_animation_y_6);
  intro_6.imprint();
  coin_animation_x_6 -= 4;
  coin_animation_y_6 -= 4;
  }
}
{
  //segment 7
  Circle intro_7(400,475,10);
  intro_7.setColor(COLOR("yellow"));
  intro_7.setFill(true);
  intro_7.imprint();

  int coin_animation_x_7 = -40, coin_animation_y_7 = 0;
  repeat(10){
  intro_7.move(coin_animation_x_7,coin_animation_y_7);
  intro_7.imprint();
  coin_animation_x_7 += 4;
  coin_animation_y_7 -= 4;
  }
}
{
  //segment 8
  Circle intro_8(180,295,10);
  intro_8.setColor(COLOR("yellow"));
  intro_8.setFill(true);
  intro_8.imprint();

  int coin_animation_x_8 = 0, coin_animation_y_8 = -40;
  repeat(10){
  intro_8.move(coin_animation_x_8,coin_animation_y_8);
  intro_8.imprint();
  coin_animation_x_8 += 4;
  coin_animation_y_8 += 4;
  }
}

{
  {
  settextstyle(BOLD_FONT ,HORIZ_DIR,7);
  Text intro_lasso_1(400,200,"LASSO!");
  intro_lasso_1.setColor(COLOR(0,51,102));
  intro_lasso_1.imprint();
  }

  settextstyle(BOLD_FONT ,HORIZ_DIR,3);
  Text intro_lasso_2(400,300,"PRESS ANY BUTTON TO CONTINUE");
  intro_lasso_2.setColor(COLOR(0,51,102));
  intro_lasso_2.imprint();
}
 intro_end:

    if(is_back==1){
            {
            Rectangle clr_scr_1(500,500,1000,1000);
                clr_scr_1.setColor(COLOR("white"));
                clr_scr_1.setFill(true);
                clr_scr_1.imprint();
            }
            Text text_press_any_button(400,300,"PRESS 'c' TO CONTINUE");
            text_press_any_button.imprint();
        }

        XEvent e2;
      nextEvent(e2);

{
Rectangle clr_scr_1(500,500,1000,1000);
    clr_scr_1.setColor(COLOR("white"));
    clr_scr_1.setFill(true);
    clr_scr_1.imprint();
}

  //front page for the game
  //contains various options
  {
  front_page:

  number_of_lives = 3;// keeps tracks of lives in level 4

  if(level_failed ==1){
  Rectangle clr_scr(500,500,1000,1000);
  clr_scr.setColor(COLOR("white"));
  clr_scr.setFill(true);
  clr_scr.imprint();
  Text text_level_failed(400,200,"LEVEL FAILED");
  Text text_try_again(400,300,"TRY AGAIN");
  Text text_back_to_front_page(400,350,"CLICK ANYWHERE TO CONTINUE");
  getClick();
    }

  //resetting level_failed =0
  level_failed = 0;

  //clear bombs for level 5
  Circle bomb_1(525,280,10);
  bomb_1.setColor(COLOR("white"));
  bomb_1.setFill(true);
  bomb_1.imprint();

  Circle bomb_2(375,280,10);
  bomb_2.setColor(COLOR("white"));
  bomb_2.setFill(true);
  bomb_2.imprint();

  Circle bomb_3(525,400,10);
  bomb_3.setColor(COLOR("white"));
  bomb_3.setFill(true);
  bomb_3.imprint();

  Circle bomb_4(375,400,10);
  bomb_4.setColor(COLOR("white"));
  bomb_4.setFill(true);
  bomb_4.imprint();


// front page
{
Rectangle clr_scr(500,500,1000,1000);
    clr_scr.setColor(COLOR("white"));
    clr_scr.setFill(true);
    clr_scr.imprint();
    }
{
settextstyle(BOLD_FONT ,HORIZ_DIR,6);
Text game_title(400,50,"L A S S O !");
game_title.imprint();

}
//GUI buttons
{
//play button
settextstyle(BOLD_FONT ,HORIZ_DIR,3);
Rectangle rect_play1(400,250,200,100);
Text play(400,250,"P L A Y !");
rect_play1.imprint();
play.imprint();

//control button
Rectangle rect_controls1(200,400,120,100);
Text help(200,400,"CONTROLS");
rect_controls1.imprint();
help.imprint();

//info button
Rectangle rect_info1(400,475,200,100);
Text settings(400,475,"HOW TO PLAY");
rect_info1.imprint();
settings.imprint();

//quit button
Rectangle rect_quit1(600,400,120,100);
Text quit(600,400,"QUIT");
rect_quit1.imprint();
quit.imprint();


}

{
  get_click_again_1://to loop the click, if not clicked on any button
  int w = getClick();
  int x = w/65536;
  int y = w%65536;

// click on controls
if(x>100 && x<300 && y>300 && y<500){
   //clr scr
  {
  Rectangle clr_scr(500,500,1000,1000);
  clr_scr.setColor(COLOR("white"));
  clr_scr.setFill(true);
  clr_scr.imprint();
  }

  Text t_control1(400,100,"PRESS 't' TO THROW LASSO");
  Text t_control2(400,150,"PRESS 'y' TO YANK LASSO");
  Text t_control3(400,200,"PRESS 'l' TO LOOP LASSO");
  Text t_control4(400,450,"PRESS 'q' TO QUIT");
  Text t_control5(400,250,"PRESS ']' TO INCREASE ANGLE");
  Text t_control6(400,300,"PRESS '[' TO DEACREASE ANGLE");
  Text t_control7(400,350,"PRESS '=' TO INCREASE SPEED");
  Text t_control8(400,400,"PRESS '-' TO DESCREASE SPEED");
  Text click_goback(400,550,"CLICK ANYWHERE TO GO BACK");

  Line divide1(0,500,800,500);
  getClick();
  goto front_page;
  }

//click on how to play
if(x>300 && x<500 && y>375 && y<575){
    //clr scr
    {Rectangle clr_scr(500,500,1000,1000);
    clr_scr.setColor(COLOR("white"));
    clr_scr.setFill(true);
    clr_scr.imprint();}

    Text how_to_play1(400,50,">THROW THE LASSO TOWARDS THE COIN");
    Text how_to_play2(400,100,">CATCH THE COIN WITH THE LASSO");
    Text how_to_play3(400,150,">PULL IT BACK");
    Text how_to_play4(400,200,">COLLECT AS MANY COINS AS POSSIBLE");
    Text how_to_play5(400,550,"CLICK ANYWHERE TO GO BACK");

    Line divide1(0,225,800,225);
    Line divide2(0,500,800,500);

    Text how_to_play6(400,250,">LEVEL 1 : BASIC LEVEL");
    Text how_to_play7(400,300,">LEVEL 2 : TIMED LEVEL");
    Text how_to_play8(400,350,">LEVEL 3 : COIN IN TRAJECTORY");
    Text how_to_play9(400,400,">LEVEL 4 : LIMITED LIVES");
    Text how_to_play10(400,450,">LEVEL 5 : BOMBS");


    getClick();
    goto front_page;

    }

//click on quit
if(x>500 && x<700 && y>300 && y<500){
    exit(0);
    }

//click on play
if(x>200 && x<600 && y>200 && y<300){
    //clr scr
        {Rectangle clr_scr(500,500,1000,1000);
    clr_scr.setColor(COLOR("white"));
    clr_scr.setFill(true);
    clr_scr.imprint();}

    Rectangle level1(400,100,125,50);
    Text text_level1(400,100,"LEVEL 1");

    Rectangle level2(400,200,125,50);
    Text text_level2(400,200,"LEVEL 2");

    Rectangle level3(400,300,125,50);
    Text text_level3(400,300,"LEVEL 3");

    Rectangle level4(400,400,125,50);
    Text text_level4(400,400,"LEVEL 4");

    Rectangle level5(400,500,125,50);
    Text text_level5(400,500,"LEVEL 5");

    get_click_again:

    int w = getClick();
    int x = w/65536;
    int y = w%65536;

    if(x>300 && x<500 && y>75 && y<125){
        level_number=1;
        goto start_level_page;
        }

    if(x>300 && x<500 && y>175 && y<225){
        level_number=2;
        goto start_level_page;
        }

    if(x>300 && x<500 && y>275 && y<325){
        level_number=3;
        goto start_level_page;
        }

    if(x>300 && x<500 && y>375 && y<425){
        level_number=4;
        goto start_level_page;
        }

    if(x>300 && x<500 && y>475 && y<525){
        level_number=5;
        goto start_level_page;
        }

    else goto get_click_again;

    }
    else goto get_click_again_1;

    }

  }

start_level_page:

  //page after selecting the level
  //shows istructions for the selected level
    {
  Text starting(WINDOW_X/2,100,"STARTING  LEVEL : ");
  Line underline1(260,120,565,120);
  Text starting_level_number(WINDOW_X/2 + 150,100,level_number);
  Line divide1(0,450,800,450);
  Text instructions(WINDOW_X/2,200,"INTRUCTIONS");

  //level 1 info (basic)
  if(level_number==1){
  Text level1_instruction_1(WINDOW_X/2,250,">BASIC LEVEL");
  Text level1_instruction_2(WINDOW_X/2,300,">COLLECT AS MANY COINS AS POSSIBLE");
  Text level1_instruction_3(WINDOW_X/2,500,"CLICK ANYWHERE TO CONTINUE");

  getClick();

  }

  //level 2 info (timer)
  if(level_number==2){
  Text level2_instruction_1(WINDOW_X/2,250,">TIMED LEVEL");
  Text level2_instruction_2(WINDOW_X/2,300,">COLLECT COINS BEFORE THE TIMER ENDS");
  Text level2_instruction_3(WINDOW_X/2,350,">COLLECT AS MANY COINS AS POSSIBLE");
  Text level2_instruction_4(WINDOW_X/2,500,"CLICK ANYWHERE TO CONTINUE");
  getClick();

  }

  //level 3 info (parabola)
  if(level_number==3){
  Text level3_instruction_1(WINDOW_X/2,250,">PARABOLA LEVEL");
  Text level3_instruction_2(WINDOW_X/2,300,">COIN MOVES IN A PARABOLA");
  Text level3_instruction_3(WINDOW_X/2,350,">COLLECT AS MANY COINS AS POSSIBLE");
  Text level3_instruction_4(WINDOW_X/2,500,"CLICK ANYWHERE TO CONTINUE");
  getClick();

  }

  //level 4 info (lives)
  if(level_number==4){
  Text level4_instruction_1(WINDOW_X/2,250,">IF THE COIN TOUCHES THE LASER, YOU LOSE A LIFE");
  Text level4_instruction_2(WINDOW_X/2,300,">ONLY 3 LIVES");
  Text level4_instruction_3(WINDOW_X/2,350,">COLLECT AS MANY COINS AS POSSIBLE");
  Text level4_instruction_4(WINDOW_X/2,500,"CLICK ANYWHERE TO CONTINUE");
  getClick();
}

  //level 5 info (bomb)
  if(level_number==5){
  Text level5_instruction_1(WINDOW_X/2,250,">OBSTACLE LEVEL");
  Text level5_instruction_2(WINDOW_X/2,300,">AVOID COLLECTING THE BOMBS");
  Text level5_instruction_3(WINDOW_X/2,350,">COLLECT AS MANY COINS AS POSSIBLE");
  Text level5_instruction_4(WINDOW_X/2,500,"CLICK ANYWHERE TO CONTINUE");
  getClick();
}

  }

  if(level_number==3){
  //sets coin for level 3
  coin_start_position = 250;
  }
  else{
  coin_start_position = 450;
  }

  //bombs for level 5
  if(level_number==5){
  //bomb 1
  Circle bomb_1(525,280,10);
  bomb_1.setColor(COLOR(96,96,96));
  bomb_1.setFill(true);
  bomb_1.imprint();

  Line line_bomb_1(525,280,525,265);
  line_bomb_1.setColor(COLOR("red"));
  line_bomb_1.imprint();

  Circle bomb_1_detail(525,280,5);
  bomb_1_detail.setColor(COLOR("red"));
  bomb_1_detail.setFill(true);
  bomb_1_detail.imprint();

  //bomb 2
  Circle bomb_2(375,280,10);
  bomb_2.setColor(COLOR(96,96,96));
  bomb_2.setFill(true);
  bomb_2.imprint();

  Line line_bomb_2(375,280,375,265);
  line_bomb_2.setColor(COLOR("red"));
  line_bomb_2.imprint();

  Circle bomb_2_detail(375,280,5);
  bomb_2_detail.setColor(COLOR("red"));
  bomb_2_detail.setFill(true);
  bomb_2_detail.imprint();

  //bomb 3
  Circle bomb_3(525,400,10);
  bomb_3.setColor(COLOR(96,96,96));
  bomb_3.setFill(true);
  bomb_3.imprint();

  Line line_bomb_3(525,400,525,385);
  line_bomb_3.setColor(COLOR("red"));
  line_bomb_3.imprint();

  Circle bomb_3_detail(525,400,5);
  bomb_3_detail.setColor(COLOR("red"));
  bomb_3_detail.setFill(true);
  bomb_3_detail.imprint();

  //bomb 4
  Circle bomb_4(375,400,10);
  bomb_4.setColor(COLOR(96,96,96));
  bomb_4.setFill(true);
  bomb_4.imprint();

  Line line_bomb_4(375,400,375,385);
  line_bomb_4.setColor(COLOR("red"));
  line_bomb_4.imprint();

  Circle bomb_4_detail(375,400,5);
  bomb_4_detail.setColor(COLOR("red"));
  bomb_4_detail.setFill(true);
  bomb_4_detail.imprint();
  }

  if(level_number==4){
  //creating number of lives logo
  Circle life_logo_1(330,50,10);
  life_logo_1.setColor(COLOR("yellow"));
  life_logo_1.setFill(true);
  life_logo_1.imprint();

  Circle life_logo_2(360,50,10);
  life_logo_2.setColor(COLOR("yellow"));
  life_logo_2.setFill(true);
  life_logo_2.imprint();

  Circle life_logo_3(390,50,10);
  life_logo_3.setColor(COLOR("yellow"));
  life_logo_3.setFill(true);
  life_logo_3.imprint();
  }

  lost_life_lvl4:

  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  //setting time limit for level 2(timer)
  if(level_number==2){
    runTime=10;
    }

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));


  Text text_go_back_to_front_page(500,525,">PRESS 'b' TO GO BACK TO FRONT PAGE");
  Text text_pause(500,550,">PRESS 'p' TO PAUSE");

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  paused = true; rtheta = true;

  if(level_number==4){
    coin_speed = COIN_SPEED1;//setting coin speed for level 4
    }
  else{
    coin_speed = COIN_SPEED;
    }

  double coin_angle_deg = COIN_ANGLE_DEG;
  if(level_number==3){                       //changing angle for level 3 (parabola)
    coin_angle_deg = COIN_ANGLE_DEG1;
    }
  double coin_ax = 0;
  if(level_number==4){
    coin_ay = COIN_G1;
    }
  else{
    coin_ay = COIN_G;
    }

  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  //graphics for the timer in level 2 (timer)
  int timer_x_coordinate=1000;
  if(level_number==2 ){
  timer_x_coordinate=700;
  }

  Circle timer_circle(timer_x_coordinate,100,30);
  timer_circle.setColor(COLOR("red"));

  for(;;) {


    if((runTime > 0) && (currTime > runTime)) { break; }

    int round_off_currTime = 10*(currTime);
    double round_off_currTime1 = round_off_currTime/10.0;  //roundoff time
    Text t_timer(timer_x_coordinate,100,10-round_off_currTime1); //shows the time in level 2

      if(level_number==4){
  //updating number of lives logo


  //written text for number of lives
  Text text_lives_1(230,50,"LIVES : ");
  Text text_lives_2(300,50,number_of_lives);
  text_lives_1.imprint();
  text_lives_2.imprint();

  //graphic for laser
  Line laser(0,100,800,100);
  laser.setColor(COLOR("red"));
  laser.imprint();

  if(coin.getYPos()<110) {
    //lose a life
        number_of_lives --;

  //keep track of number of lives
  if(number_of_lives==2){
  Circle deduct_life(390,50,10);
  deduct_life.setColor(COLOR("white"));
  deduct_life.setFill(true);
  deduct_life.imprint();
  }

  if(number_of_lives==1){
  Circle deduct_life(360,50,10);
  deduct_life.setColor(COLOR("white"));
  deduct_life.setFill(true);
  deduct_life.imprint();
  }

  if(number_of_lives==0){
  Circle deduct_life(330,50,10);
  deduct_life.setColor(COLOR("white"));
  deduct_life.setFill(true);
  deduct_life.imprint();
  }
  goto lost_life_lvl4;
  }
  }


    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {

      case 'p'://pause the game
      {
      Rectangle paused(400,125,400,125);
      Text game_is_paused(400,100,"GAME IS PAUSED");
      Text press_r_to_resume(400,150,"PRESS 'r' TO RESUME");
      paused.imprint();
      game_is_paused.imprint();
      press_r_to_resume.imprint();
      }

      //resume the game
      while(true){
      XEvent e1;
      nextEvent(e1);
      char c1 = charFromEvent(e1);
      if(c1=='r') break;
      }

      //graphics for paused
      {
      Rectangle paused(400,125,400,125);
      paused.setColor(COLOR("white"));
      paused.setFill(true);
      paused.imprint();
      }

      break;

      case 'b':
      is_back=1;
      goto intro_end;

      case 't':
    lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);

    if(number_of_lives==0){
        level_failed = 1; // end level if number of lives =0
        }


  if(level_failed == 1){
    Rectangle clr_scr(500,500,1000,1000);
    clr_scr.setColor(COLOR("white"));
    clr_scr.setFill(true);
    clr_scr.imprint();
    Text text_level_failed(400,200,"LEVEL FAILED");
    Text text_try_again(400,300,"TRY AGAIN!");
    Text text_back_to_front_page(400,350,"CLICK ANYWHERE TO CONTINUE");
    goto front_page;

    }


  } // End for(;;)

  if(level_number == 5){
    goto front_page;
    }

    Rectangle clr_scr(500,500,1000,1000);
    clr_scr.setColor(COLOR("white"));
    clr_scr.setFill(true);
    clr_scr.imprint();
    Text text_level_failed(400,200,"LEVEL FAILED");
    Text text_try_again(400,300,"TRY AGAIN!");
    Text text_back_to_front_page(400,350,"CLICK ANYWHERE TO CONTINUE");
    getClick();
goto front_page;

  wait(3);
} // End main_program
