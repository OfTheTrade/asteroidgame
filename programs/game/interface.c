#include "raylib.h"
#include "vec2.h"
#include "ADTList.h"

#include "state.h"
#include "interface.h"


void interface_init() {
	// Open the window, set the target fps
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");
    SetTargetFPS(60);
}

void interface_close() {
    // Close the window
	CloseWindow();
}

void interface_draw_frame(State state) {

    // Start every frame on a black canvas
	BeginDrawing();
	ClearBackground(BLACK);

    Vector2 middlevertice = (Vector2){SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
	// Vectors to draw spaceship
	Vector2 flippedvert = (Vector2){state_info(state)->spaceship->orientation.x,(-1)*(state_info(state)->spaceship->orientation.y)};
	Vector2 topvertice = vec2_scale(flippedvert, SPACESHIP_SIZE);
	Vector2 leftvertice = vec2_rotate(topvertice,(2*PI)/3);
	Vector2 rightvertice = vec2_rotate(topvertice,(-1)*((2*PI)/3));
    
	// Draw spaceship and it's nose
	DrawCircleLines(vec2_add(topvertice,middlevertice).x, vec2_add(topvertice,middlevertice).y, 5, WHITE);
	DrawTriangleLines(vec2_add(topvertice,middlevertice),vec2_add(leftvertice,middlevertice),vec2_add(rightvertice,middlevertice),WHITE);

    // Bounds of the state that will be drawn
	float topy = state_info(state)->spaceship->position.y + (SCREEN_HEIGHT/2);
	float bottomy = state_info(state)->spaceship->position.y - (SCREEN_HEIGHT/2);
	float rightx = state_info(state)->spaceship->position.x + (SCREEN_WIDTH/2);
	float leftx = state_info(state)->spaceship->position.x - (SCREEN_WIDTH/2);
	
	// Take the objects within those bounds
	List list = state_objects(state, (Vector2){rightx,topy}, (Vector2){leftx,bottomy});
    
	float screenposx;
	float screenposy;
    
	// Draw every object in the list
	ListNode crntnode = list_first(list);
	Object crntobject;

	while (crntnode != LIST_EOF){

		crntobject = list_node_value(list,crntnode);
        
		// Position should be adjasted
		if (leftx > 0){
        	screenposx = crntobject->position.x - (leftx);
		}else if(rightx < 0){
			screenposx = crntobject->position.x - (rightx);
		}
        if (bottomy > 0){
        	screenposy = crntobject->position.y - (bottomy);
		}else if(topy < 0){
			screenposy = crntobject->position.y - (topy);
		}
       
        screenposy = screenposy - (SCREEN_HEIGHT);  
        
		// Draw object
        if (crntobject->type == BULLET){
			DrawCircle(screenposx, screenposy, BULLET_SIZE, WHITE);
		}else{
            DrawCircleLines(screenposx, screenposy, crntobject->size, WHITE);
		}

		crntnode = list_next(list,crntnode);
	}	

    list_destroy(list);

	// FPS counter and score
	DrawText(TextFormat("%08i", state_info(state)->score), 20, 20, 40, WHITE);
	DrawFPS(SCREEN_WIDTH - 80, 0);

	// If the game is paused, make paused text
	if (state_info(state)->paused) {
		DrawText(
			"STOPPED || PRESS N TO RUN A FRAME",
			 GetScreenWidth() / 2 - MeasureText("STOPPED || PRESS N TO RUN A FRAME", 20) / 2,
			 GetScreenHeight() / 2 - 50, 20, WHITE
		);
	}

	EndDrawing();
}