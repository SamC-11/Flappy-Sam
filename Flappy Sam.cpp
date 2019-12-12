 
// This program is my summative. ICS3U Sam Cung. This game is a flappy bird like game but the flappy bird is replaced by me and the 
//pipes are replaced by guitars. 

//Please don't get frustrated by the difficulty of the game and give me a 0 :( 
//I decided to only make 1 sam character because I wanted to finish everything else first.
#include <allegro.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Sam_Picture.h"
#include "Guitar_Picture.h"
#define TYPE 2

//globals
int score = 0;
int HighScore = 0;
//timer stuff
volatile long speed_counter = 0;

void increment_speed_counter() 
{
	speed_counter++; 
}
END_OF_FUNCTION(increment_speed_counter);
//Functions
void detectGroundBorder(struct Sam &Sam_Character);
void doAction(struct Sam &Sam_Character);
void initSam(struct Sam &Sam_Character);
void initPipe1(struct Pipe1 &Pipe_1);
void Pipe1Motion(struct Pipe1 &Pipe_1);
void HboxDisplay(BITMAP *buffer, struct Sam &Sam_Character, struct Pipe1 &Pipe_1);
void DetectCollision (BITMAP* buffer, struct Sam &Sam_Character, struct Pipe1 &Pipe_1);

//Structures
struct Sam
{
   DATAFILE *Sam_Pics;  
       
   int sam_x;
   int sam_y;   
   int jump;
   int gravity;
   
   int Hbox_Right;
   int Hbox_Left;
   int Hbox_Top;
   int Hbox_Bottom;
   
   double distance;
   float counter;
};

struct Pipe1
{
    BITMAP* Pipe_Sprite;
    
    int Pipe_x;
    int Pipe_y;    
    
    int Hbox_Right1;
    int Hbox_Left1;
    int Hbox_Top1;
    int Hbox_Bottom1;
    
    int Hbox_Right2;
    int Hbox_Left2;
    int Hbox_Top2;
    int Hbox_Bottom2;
    
    int centre_X;
    int colliding;
    int PipeNumber;
};

int frame_speed = 18;

//main function
int main (int argc, char *argv[])
{
    
    allegro_init();  
    install_keyboard();
    FILE* HScore;
    
   	set_color_depth(desktop_color_depth()); // Set the color depth
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); 
	
	BITMAP *background = NULL; //I am making a bitmap called background for my background
	BITMAP *buffer = NULL;
	BITMAP *babySam = NULL;
	//BITMAP *GuitarSet1 = NULL;
	
    struct Sam Sam_Character;
    struct Pipe1 Pipe_1;

    //struct Guitar Guitar_Type;

    background = load_bitmap("Flappy bird background.bmp", NULL); // Load our background
      	
	srand(time(0));
	
	install_timer(); // Initialize the timer routines
	
	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
	LOCK_FUNCTION(increment_speed_counter);//speed.
	
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //Set our BPS

	
	buffer = create_bitmap(640,480);
	initSam(Sam_Character); //initialize the sam sprite
	initPipe1(Pipe_1);
	
	int ij = rand()%TYPE; //randomize between the characters (This isn't used anymore because I ran out of time
	int frame_counter = 0;
	int frame_counter_2 = 0;
	int j = 0;
	
	HScore = fopen("High Score.txt", "r");      
    fscanf(HScore, "%d", &HighScore);
      
    fclose(HScore);
	
    while(!key[KEY_ESC]) //If the user hits escape, quit the program 
    {
            blit(background, buffer, -5,0,0,0,640,481);   
            
            textprintf_centre_ex(buffer, font, 640/2,445, makecol(255,0,0), -1, "Press Space to Jump! DONT HIT THE GUITAR NECKS!");
            
            
            draw_sprite(buffer, Pipe_1.Pipe_Sprite, Pipe_1.Pipe_x, Pipe_1.Pipe_y);

                                  		
		//This part will make the flappy sam do his action and all that good stuff 
	    while(speed_counter > 0)
		{
    		//this part will call the functions  
            doAction(Sam_Character);  
    		detectGroundBorder(Sam_Character);
    		Pipe1Motion(Pipe_1);
    	    DetectCollision (buffer, Sam_Character, Pipe_1);
		
    		speed_counter --;
    		frame_counter ++;
    		frame_counter_2 ++;
    		
    		if(frame_counter >= frame_speed)
    		{
                frame_counter = 0;                         
            }         
        }
        
        //This part makes the Sam character animate(the wings flap) and also display other things such as the highscore
        if( frame_counter < (frame_speed * 0.25) )
        {
            draw_sprite(buffer, (BITMAP*)Sam_Character.Sam_Pics[Samuel_Baby2].dat, Sam_Character.sam_x, Sam_Character.sam_y); // Draw the picture to the buffer 
            HboxDisplay(buffer, Sam_Character, Pipe_1);
            textprintf_ex(buffer, font, 0,0, makecol(255,5,255), -1, "%d : %d", Sam_Character.sam_x, Sam_Character.sam_y );		  
            textprintf_centre_ex(buffer, font, 640/2,455, makecol(255,5,255), -1, "%d", score );
            if ( score > HighScore )
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", score );                                   
            }

            else
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", HighScore ); 
            }
             		
        } 
        else if( frame_counter >= (frame_speed * 0.25) && frame_counter < (frame_speed * 0.5))
        {
            draw_sprite(buffer, (BITMAP*)Sam_Character.Sam_Pics[Samuel_Baby].dat, Sam_Character.sam_x, Sam_Character.sam_y); // Draw the picture to the buffer 
            HboxDisplay(buffer, Sam_Character, Pipe_1);
            textprintf_ex(buffer, font, 0,0, makecol(255,5,255), -1, "%d : %d", Sam_Character.sam_x, Sam_Character.sam_y );		  
            textprintf_centre_ex(buffer, font, 640/2,455, makecol(255,5,255), -1, "%d", score );
            if ( score > HighScore )
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", score );                                   
            }

            else
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", HighScore ); 
            } 	
        }   
        else if( frame_counter >= (frame_speed * 0.5) && frame_counter < (frame_speed * 0.75))
        {
            draw_sprite(buffer, (BITMAP*)Sam_Character.Sam_Pics[Samuel_Baby].dat, Sam_Character.sam_x, Sam_Character.sam_y); // Draw the picture to the buffer 
            HboxDisplay(buffer, Sam_Character, Pipe_1);
            textprintf_ex(buffer, font, 0,0, makecol(255,5,255), -1, "%d : %d", Sam_Character.sam_x, Sam_Character.sam_y );		  
            textprintf_centre_ex(buffer, font, 640/2,455, makecol(255,5,255), -1, "%d", score );
            if ( score > HighScore )
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", score );                                   
            }

            else
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", HighScore ); 
            }	
        }   
        else
        {
            draw_sprite(buffer, (BITMAP*)Sam_Character.Sam_Pics[Samuel_Baby2].dat, Sam_Character.sam_x, Sam_Character.sam_y); // Draw the picture to the buffer 
            HboxDisplay(buffer, Sam_Character, Pipe_1);
            textprintf_ex(buffer, font, 0,0, makecol(255,5,255), -1, "%d : %d", Sam_Character.sam_x, Sam_Character.sam_y );	
            textprintf_centre_ex(buffer, font, 640/2,455, makecol(255,5,255), -1, "%d", score );
            if ( score > HighScore )
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", score );                                   
            }

            else
            {
                 textprintf_centre_ex(buffer, font, 640/2,465, makecol(255,5,255), -1, "HighScore: %d", HighScore ); 
            }	  
        } 
        
        blit(buffer, screen, 0,0,0,0,640,481); // Draw the buffer to the screen 
        clear_bitmap(buffer);       
    }

    unload_datafile(Sam_Character.Sam_Pics);
	destroy_bitmap(background);
	destroy_bitmap(buffer); //Release the bitmap data 
	return 0;
    
    }

END_OF_MAIN()

//This will initialize Sam
void initSam(struct Sam &Sam_Character)
{
    Sam_Character.Sam_Pics = load_datafile("Sam_Data.dat");
    Sam_Character.jump = FALSE;  
    Sam_Character.gravity = 3;
    Sam_Character.counter = 0.0;
    Sam_Character.distance = 4.0;             
    Sam_Character.sam_y = 100;
    Sam_Character.sam_x = 142;    
}


//This will make the flappy sam do the actions
void doAction( struct Sam &Sam_Character )
{
    
    //This part will make the flappy sam able to jump 
    int i;
    int j = 0;

    if(key[KEY_SPACE] && !(Sam_Character.sam_y <= 25)) //This part will make Sam jump.
	{      
        Sam_Character.jump = TRUE; //when space is pressed, the jump will be true
	}
	
	if(Sam_Character.jump == TRUE) //if the jump is true run this to make him jump
	{
        Sam_Character.gravity = 4;  
        Sam_Character.counter = 0.0;                   
        Sam_Character.distance+=0.1;                 
        Sam_Character.sam_y = Sam_Character.sam_y + int((sin(Sam_Character.distance) + cos(Sam_Character.distance)) * 8);
        //I got this from a website where there is a tutorial on how to make sprites jump for 2d platform games like mario.   
                       
        if(Sam_Character.distance >= 5.5)
        {
            Sam_Character.distance = 4;
            Sam_Character.jump = FALSE;                          
        }                            
   }
   else
   {
       Sam_Character.counter+=1;
       if(fmod(Sam_Character.counter,5)==0)
       {
           Sam_Character.gravity++;                                        
       }
       Sam_Character.sam_y+=Sam_Character.gravity;             
   }

}

void detectGroundBorder(struct Sam &Sam_Character) //This is here to make sure sam dies when he hits the ground
{
    FILE* HScore;
    if(Sam_Character.sam_y >= 381)
    {
        Sam_Character.sam_y = 381;  
        if (score > HighScore) {
        //When Sam dies write the highscore to the textfile and say "Game Over"
        HScore = fopen("High Score.txt", "w");    
        fprintf(HScore, "%d", score);
        fclose(HScore);
        
 
      }
   allegro_message("Game Over\nScore: %d", score);
	exit(EXIT_FAILURE);
              
    }     
    else if(Sam_Character.sam_y <= 0)
    {
        Sam_Character.sam_y = 0;
        Sam_Character.jump = FALSE;                       
    } 
    else if(Sam_Character.sam_x == 600)
    {
        Sam_Character.sam_x = 0 ;                      
    } 
    else if(Sam_Character.sam_x == 0)
    {
        Sam_Character.sam_x = 600;                      
    } 
}

void initPipe1(struct Pipe1 &Pipe_1) //This part initializes the pipe
{
     Pipe_1.PipeNumber = rand()%3+1; 
     Pipe_1.centre_X = 0; 
     switch(Pipe_1.PipeNumber)
     {
         case 1:
              Pipe_1.Pipe_Sprite = load_bitmap("GuitarSet1.bmp", NULL);  
              break; 
         case 2:
              Pipe_1.Pipe_Sprite = load_bitmap("GuitarSet2.bmp", NULL);  
              break; 
         case 3:
              Pipe_1.Pipe_Sprite = load_bitmap("GuitarSet3.bmp", NULL);  
              break; 
     } 
     Pipe_1.Pipe_x = 628;
     Pipe_1.Pipe_y = 0;
}

//This part controls the movement of the pipes
void Pipe1Motion(struct Pipe1 &Pipe_1)
{
     if (Pipe_1.Pipe_x > -10) 
     {
     Pipe_1.Pipe_x -= 3;   
     }  
     
     //This will generate a new pipe every time one pipe goes to 0
     if(Pipe_1.Pipe_x <= 0)
     {
     Pipe_1.Pipe_x = 640; 
     //This part will make the generated pipe random
     Pipe_1.PipeNumber = rand()%3+1;  
     switch(Pipe_1.PipeNumber)
     {
         case 1:
              Pipe_1.Pipe_Sprite = load_bitmap("GuitarSet1.bmp", NULL);  
              break; 
         case 2:
              Pipe_1.Pipe_Sprite = load_bitmap("GuitarSet2.bmp", NULL);  
              break; 
         case 3:
              Pipe_1.Pipe_Sprite = load_bitmap("GuitarSet3.bmp", NULL);  
              break;       
     } 
     
     }
     Pipe_1.centre_X = Pipe_1.Pipe_x + 5; 
      
}

 //This is here to display the hitbox (This helped me program the collisions)
void HboxDisplay(BITMAP *buffer, struct Sam &Sam_Character, struct Pipe1 &Pipe_1)
{

 int HitboxShow = TRUE;
 
 Sam_Character.Hbox_Top = Sam_Character.sam_y;
 Sam_Character.Hbox_Bottom = Sam_Character.sam_y + 48;
 Sam_Character.Hbox_Left = Sam_Character.sam_x;
 Sam_Character.Hbox_Right = Sam_Character.sam_x + 55;
 
 switch(Pipe_1.PipeNumber)
 {
     case 1:
          Pipe_1.Hbox_Top1 = Pipe_1.Pipe_y;
          Pipe_1.Hbox_Bottom1 = Pipe_1.Pipe_y + 141;
          Pipe_1.Hbox_Left1 = Pipe_1.Pipe_x;
          Pipe_1.Hbox_Right1 = Pipe_1.Pipe_x + 11; 
          
          Pipe_1.Hbox_Top2 = Pipe_1.Pipe_y + 300;
          Pipe_1.Hbox_Bottom2 = Pipe_1.Hbox_Top2 + 124;
          Pipe_1.Hbox_Left2 = Pipe_1.Pipe_x;
          Pipe_1.Hbox_Right2 = Pipe_1.Pipe_x + 11; 
          break; 
     case 2:
          Pipe_1.Hbox_Top1 = Pipe_1.Pipe_y;
          Pipe_1.Hbox_Bottom1 = Pipe_1.Pipe_y + 55;
          Pipe_1.Hbox_Left1 = Pipe_1.Pipe_x;
          Pipe_1.Hbox_Right1 = Pipe_1.Pipe_x + 11; 
          
          Pipe_1.Hbox_Top2 = Pipe_1.Pipe_y + 216;
          Pipe_1.Hbox_Bottom2 = Pipe_1.Hbox_Top2 + 211;
          Pipe_1.Hbox_Left2 = Pipe_1.Pipe_x;
          Pipe_1.Hbox_Right2 = Pipe_1.Pipe_x + 11;  
          break; 
     case 3:
          Pipe_1.Hbox_Top1 = Pipe_1.Pipe_y;
          Pipe_1.Hbox_Bottom1 = Pipe_1.Pipe_y + 212;
          Pipe_1.Hbox_Left1 = Pipe_1.Pipe_x;
          Pipe_1.Hbox_Right1 = Pipe_1.Pipe_x + 11; 
          
          Pipe_1.Hbox_Top2 = Pipe_1.Pipe_y + 374;
          Pipe_1.Hbox_Bottom2 = Pipe_1.Hbox_Top2 + 53;
          Pipe_1.Hbox_Left2 = Pipe_1.Pipe_x;
          Pipe_1.Hbox_Right2 = Pipe_1.Pipe_x + 11;  
          break;       
 } 
 if(key[KEY_ENTER])
 {
     HitboxShow = TRUE;
 }
 if(!key[KEY_ENTER])
 {
     HitboxShow = FALSE;
 }
 //This draws the hitbox on the screen
 if ( HitboxShow == TRUE ){
 line(buffer, Sam_Character.Hbox_Left, Sam_Character.Hbox_Top,Sam_Character.Hbox_Right, Sam_Character.Hbox_Top, makecol(0,0,255));
 line(buffer, Sam_Character.Hbox_Left, Sam_Character.Hbox_Bottom,Sam_Character.Hbox_Right, Sam_Character.Hbox_Bottom, makecol(0,0,255));
 line(buffer, Sam_Character.Hbox_Left, Sam_Character.Hbox_Top,Sam_Character.Hbox_Left, Sam_Character.Hbox_Bottom, makecol(0,0,255));
 line(buffer, Sam_Character.Hbox_Right, Sam_Character.Hbox_Top,Sam_Character.Hbox_Right, Sam_Character.Hbox_Bottom, makecol(0,0,255));
 
 line(buffer, Pipe_1.Hbox_Left1, Pipe_1.Hbox_Top1,Pipe_1.Hbox_Right1, Pipe_1.Hbox_Top1, makecol(0,0,255));
 line(buffer, Pipe_1.Hbox_Left1, Pipe_1.Hbox_Bottom1,Pipe_1.Hbox_Right1, Pipe_1.Hbox_Bottom1, makecol(0,0,255));
 line(buffer, Pipe_1.Hbox_Left1, Pipe_1.Hbox_Top1,Pipe_1.Hbox_Left1, Pipe_1.Hbox_Bottom1, makecol(0,0,255));
 line(buffer, Pipe_1.Hbox_Right1, Pipe_1.Hbox_Top1,Pipe_1.Hbox_Right1, Pipe_1.Hbox_Bottom1, makecol(0,0,255));
 
 line(buffer, Pipe_1.Hbox_Left2, Pipe_1.Hbox_Top2,Pipe_1.Hbox_Right2, Pipe_1.Hbox_Top2, makecol(0,0,255));
 line(buffer, Pipe_1.Hbox_Left2, Pipe_1.Hbox_Bottom2,Pipe_1.Hbox_Right2, Pipe_1.Hbox_Bottom2, makecol(0,0,255));
 line(buffer, Pipe_1.Hbox_Left2, Pipe_1.Hbox_Top2,Pipe_1.Hbox_Left2, Pipe_1.Hbox_Bottom2, makecol(0,0,255));
 line(buffer, Pipe_1.Hbox_Right2, Pipe_1.Hbox_Top2,Pipe_1.Hbox_Right2, Pipe_1.Hbox_Bottom2, makecol(0,0,255));
 
 //line(buffer, Pipe_1.centre_X, Pipe_1.Hbox_Top2,Pipe_1.Hbox_Right2, Pipe_1.Hbox_Bottom2, makecol(0,0,255));
 line(buffer, Pipe_1.centre_X, 0, Pipe_1.centre_X, 640, makecol(0,0,255));
} 
}
//This part will detect the collisions
void DetectCollision (BITMAP* buffer, struct Sam &Sam_Character, struct Pipe1 &Pipe_1)
{
     FILE* HScore;
     //When sam dies, write the highscore to the textfile and say "Game Over"
     if (Sam_Character.Hbox_Top <= Pipe_1.Hbox_Bottom1 && (Sam_Character.Hbox_Left <= Pipe_1.Hbox_Right1 && Sam_Character.Hbox_Right >= Pipe_1.Hbox_Left1)){
      if (score > HighScore){
      HScore = fopen("High Score.txt", "w");      
      fprintf(HScore, "%d", score);     
      fclose(HScore);
      }
      HScore = fopen("High Score.txt", "r");
      allegro_message("Game Over\nScore: %d", score);
      exit(EXIT_FAILURE);
    
     }
     //This will detect the collisions
     if (Sam_Character.Hbox_Bottom >= Pipe_1.Hbox_Top2 && (Sam_Character.Hbox_Left <= Pipe_1.Hbox_Right2 && Sam_Character.Hbox_Right >= Pipe_1.Hbox_Left2)){
      if (score > HighScore) {
      //When Sam dies write the highscore to the textfile and say "Game Over"
      HScore = fopen("High Score.txt", "w");    
      fprintf(HScore, "%d", score);
      fclose(HScore);
      }
      
      HScore = fopen("High Score.txt", "r");
      allegro_message("Game Over\nScore: %d", score);
      exit(EXIT_FAILURE);  
     }
     
     //This part will give Sam points when he goes in between the pipes
     if(!Sam_Character.Hbox_Top <= Pipe_1.Hbox_Bottom1 && (Sam_Character.Hbox_Left <= Pipe_1.Hbox_Right1 && Sam_Character.Hbox_Right >= Pipe_1.Hbox_Left1) && Sam_Character.Hbox_Right <= Pipe_1.centre_X)
     {
         score++; 
                                    
     } 
     
}


















