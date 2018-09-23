#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <vector>

#define clk PB13
#define din PB15
#define dc  PB14
#define cs  PB12
#define rst PA15
// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(clk, din, dc, cs, rst);

//speed in ms
#define SPEED 60

#define SNAKE_INIT_LENGTH 80 //Initial length of snake
#define SNAKE_START_X 42 //Initial x position
#define SNAKE_START_Y 20 //Initial y position

//Pin setup according to the board
#define left_b PA1
#define right_b PB8
#define up_b PA0
#define down_b PB9
#define center_b PC13
#define left_led PB6
#define right_led PB7

enum dir {right, left, up, down};

//Store the coordinates in a structure
struct coordinates
{
  int8_t x;
  int8_t y;
  int8_t x2;
  int8_t y2;
};

std::vector<coordinates> snake; //store the snake points
coordinates snake_last_piece; //store the last point | needed for visualisation

coordinates food; //food coordinates

bool leds = true; //for LED blink


dir direction;// = right;
int last_direction = 0;

int points = 0;


void start_timer()
{
  Timer2.setChannel1Mode(TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(SPEED*1000); // period in microseconds
  Timer2.setCompare1(1); 
  Timer2.attachCompare1Interrupt(TimerCallbackFunc); //callback function periodically called witt interrupt
}

void stop_timer()
{
  Timer2.setChannel1Mode(TIMER_DISABLED);
}


void setup() {
  // put your setup code here, to run once:
  //Serial port
  Serial.begin(9600);

  //Set inputs and outputs
  pinMode(left_b, INPUT_PULLUP);
  pinMode(right_b, INPUT_PULLUP);
  pinMode(up_b, INPUT_PULLUP);
  pinMode(down_b, INPUT_PULLUP);
  pinMode(center_b, INPUT_PULLUP);
  pinMode(left_led, OUTPUT);
  pinMode(right_led, OUTPUT);

  display.begin(65,4);  //contrast, bias
  
  display.display(); //show splashscreen
  delay(2000);
  display.clearDisplay();

  init_game();

  start_timer();

 // timer.setInterval(SPEED, TimerCallbackFunc); //Timer for moving the snake
}

void init_game()
{
  /*
   * This function initializes the snake, clears the points and direction, and generate a new food.
   * Can be used to reset the game.
   */
  
  //init the snake
  direction = right;

  points = 0;
  
  snake.clear(); //clear the vector
  for(int i = 0; i < SNAKE_INIT_LENGTH; i++)
  {
    coordinates helper;
    helper.x = SNAKE_START_X - i;
    helper.y = SNAKE_START_Y;
    helper.x2 = SNAKE_START_X - i;
    helper.y2 = SNAKE_START_Y+1;
    snake.push_back(helper);
  }

  snake_last_piece.x = 0;
  snake_last_piece.y = 0;
  snake_last_piece.x2 = 0;
  snake_last_piece.y2 = 0;

  generate_food();
}

void TimerCallbackFunc(void)
{
  //blinks the 2 LED
  digitalWrite(left_led, leds);
  digitalWrite(right_led, !leds);
  leds = !leds;

  //Moving in the given direction
  snake_move(direction);
}

void draw_food(uint8_t color)
{
  display.drawPixel(food.x+1, food.y ,   color);
  display.drawPixel(food.x+1, food.y+2 , color);
  display.drawPixel(food.x,   food.y+1 , color);
  display.drawPixel(food.x+2, food.y+1 , color);
}

bool check_food_colision(coordinates point)
{
  if(((food.x+1 == point.x) && (food.y == point.y)) || ((food.x+1 == point.x2) && (food.y == point.y2) ))
    return true;
    
  if(((food.x+1 == point.x) && (food.y+2 == point.y)) || ((food.x+1 == point.x2) && (food.y+2 == point.y2) ))
    return true;

  if(((food.x == point.x) && (food.y+1 == point.y)) || ((food.x == point.x2) && (food.y+1 == point.y2) ))
    return true;

  if(((food.x+2 == point.x) && (food.y+1 == point.y)) || ((food.x+2 == point.x2) && (food.y+1 == point.y2) ))
    return true;

  return false;
}

void generate_food()
{
  /*
   * Generates random x and y points until it's out of the snake's points.
   */
  draw_food(WHITE);  //clear last food remained pieces
  bool equal = true;
  while(equal)
  {
    equal = false;
    food.x = random(0, 83-2);  //minus food size
    food.y = random(0,47-2); 
    for(int i = 0; i < snake.size(); i++)
    {
     // if(snake[i].x == food.x && snake[i].y == food.y || snake[i].x2 == food.x && snake[i].y2 == food.y)
      if(check_food_colision(snake[i]))
      {
        equal = true;
        break;  //if colision happened terminate loop because unnecessary to check remaining points
      }
    }
  }
  draw_food(BLACK);
  display.display();
}


void draw_head(coordinates point, dir direction)
{
  if(direction == right)
  {
    display.fillRect(point.x, point.y, 4, 2, BLACK);
    display.drawPixel(point.x,point.y,WHITE);
    display.drawPixel(point.x-1,point.y,WHITE);
    display.drawPixel(point.x,point.y-1,BLACK);
    display.drawPixel(point.x-1,point.y-1,BLACK);
  }

  if(direction == left)
  {
    display.fillRect(point.x-2, point.y-1, 3, 2, BLACK);
    display.drawPixel(point.x,point.y-1,WHITE);
    display.drawPixel(point.x+1,point.y-1,WHITE);
    display.drawPixel(point.x,point.y-2,BLACK);
    display.drawPixel(point.x+1,point.y-2,BLACK);
    
  }

  if(direction == up)
  {
    display.fillRect(point.x, point.y-3, 2, 3, BLACK);
    display.drawPixel(point.x,point.y,WHITE);
    display.drawPixel(point.x,point.y-1,WHITE);
    display.drawPixel(point.x-1,point.y,BLACK);
    display.drawPixel(point.x-1,point.y-1,BLACK);
    
  }

  if(direction == down)
  {
    display.fillRect(point.x-1, point.y+1, 2, 3, BLACK);
    display.drawPixel(point.x-1,point.y,WHITE);
    display.drawPixel(point.x-1,point.y+1,WHITE);
    display.drawPixel(point.x-2,point.y,BLACK);
    display.drawPixel(point.x-2,point.y+1,BLACK);
    
  }
}



void draw_snake()
{
  /*
   * Draw the snake points.
   */
  display.clearDisplay();
  
  for(int i = 0; i < snake.size(); i++)
  {
    if(i%4 != 2)
   {
    display.drawPixel(snake[i].x, snake[i].y, BLACK);
   }
    if((i+1)%4 != 2)
   {
    display.drawPixel(snake[i].x2, snake[i].y2, BLACK);
   }  
  }
  draw_head(snake[0], direction);
}

bool check_crash()
{
  /*
   * Returns true if the snake crashing into itself.
   */
  for(int i = 1; i < snake.size(); i++)
  {
    if( (snake[0].x == snake[i].x && snake[0].y == snake[i].y)  )   //|| (snake[0].x2 == snake[i].x2 && snake[0].y2 == snake[i].y2)|| (snake[0].x == snake[i].x2 && snake[0].y == snake[i].y2)
    {
      return true;
    }
  }
  return false;
}

void snake_move(dir direction)  //TODO use dir enum
{
  /*
   * Handling the moving directions and next points.
   */
  last_direction = direction;
  
  snake_last_piece = snake[snake.size()-1];
  coordinates helper;
  
  if(direction == right) 
  {
    helper = snake[0];
    helper.x++;
    helper.x2 =  helper.x;
    helper.y2 = helper.y + 1;
    if(helper.x > 83 || helper.x2 > 83)
    {
      helper.x  = 0;
      helper.x2 = 0;
    }
  }
  else if(direction == left) 
  {
    helper = snake[0];
    helper.x--;
    helper.x2 =  helper.x;
    helper.y2 = helper.y - 1;
    if(helper.x < 0 || helper.x2 < 0)
    {
      helper.x  = 83;
      helper.x2 = 83;
    }
  }
  else if(direction == up) 
  {
    helper = snake[0];
    helper.y--;
    helper.y2 = helper.y;
    helper.x2 = helper.x + 1;
    
    if(helper.y < 0 || helper.y2 < 0)
    {
      helper.y  = 47;
      helper.y2 = 47;
    }
  }
  else if(direction == down) 
  {
    helper = snake[0];
    helper.y++;
    helper.y2 = helper.y;
    helper.x2 = helper.x - 1;
    if(helper.y > 47 || helper.y2 > 47)
    {
      helper.y  = 0;
      helper.y2 = 0;
    }
  }

  //move forward every coordinate and replace the first with the new one
  for(int i = snake.size(); i > 0; i--)
  {
    snake[i] = snake [i-1];
  }
  snake[0] = helper;
  draw_snake();
  draw_food(BLACK);
  display.display();

  if(check_food_colision(snake[0]))
  {
    increment_snake();
    generate_food();
  }
}

void increment_snake()
{
  snake.push_back(snake[snake.size()]);
  points++;
}

void loop()
{
  // put your main code here, to run repeatedly:
 // timer.run();
/*
  if(update)
  {
    update = false;
    TimerCallbackFunc();
  }
*/

 
  if(digitalRead(left_b) == 0)
  {
    Serial.print("Left button pressed");
    if(last_direction != right)
    {
      direction = left;
    }
  }

  if(digitalRead(right_b) == 0)
  {
    Serial.print("Right button pressed");
    if(last_direction != left)
    {
      direction = right;
    }
  }

  if(digitalRead(up_b) == 0)
  {
    Serial.print("Up button pressed");
    if(last_direction != down )
    {
      direction = up;
    }
  }

  if(digitalRead(down_b) == 0)
  {
    Serial.print("Down button pressed");
    if(last_direction != up)
    {
      direction = down;
    }
  }

  if(digitalRead(center_b) == 0)
  {
    noInterrupts();
    Serial.print("Center button pressed");
    display.drawPixel(food.x, food.y, BLACK);
    display.display();
    interrupts();
  }

  // if draw to screen or access game logic variables from outside timer callback( eg loop or functions called from loop)
  // guard code block with  noInterrupts()  interrupts() otherwise display driver crash, game bug will happen
  // in guarded block don't use delay or long loops, otherwise usb connection will fail, maybe millis() freez  in guarded block???
  noInterrupts();
  bool crash = check_crash();
  interrupts();
  
  if(crash)
  {
    stop_timer(); 
     interrupts();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.println("Game over!");
    //display.setCursor(0,10);
    display.println("Score: ");
    display.print(points, DEC);
    display.display();
    
    while(digitalRead(center_b) != 0)
    {
    }
    display.clearDisplay();
    init_game();
    start_timer();
  }
  
  }


