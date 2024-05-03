#include "raylib.h"

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

	// Draw spaceship
	Vector2 topvertice = vec2_scale((state_info(state)->spaceship->orientation), SPACESHIP_SIZE);
	DrawTriangleLines(topvertice,vec2_rotate(topvertice,PI/3),vec2_rotate(topvertice,(-1)*(PI/3)));

    // Get list of objects to draw
	List list = state_objects(state, (Vector2){(SCREEN_WIDTH/2),(SCREEN_HEIGHT/2)}, (Vector2){(SCREEN_WIDTH/2),(SCREEN_HEIGHT/2)});
    
	float screenposx;
	float screenposy;
    
	// Draw every object in the list
	ListNode crntnode = list_first(List list);
	Object crntobject;

	While (crntnode != LIST_EOF){
		crntobject = list_node_value(crntnode);
        
		// Position should be relative to the spaceship
        screenposx = crntobject->position.x - (state_info(state)->spaceship->position.x);
        screenposy = crntobject->position.y - (state_info(state)->spaceship->position.y);

        if (crntobject->type == BULLET){
			DrawCircle(screenposx, screenposy, BULLET_SIZE, WHITE);
		}else{
            DrawCircleLines(screenposx, screenposy, crntobject->size, WHITE);
		}

		crntnode = list_next(crntnode);
	}	

    list_destroy(List list);

	// FPS counter and score
	DrawText(TextFormat("%04i", state_info(state)->score), 20, 20, 40, WHITE);
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