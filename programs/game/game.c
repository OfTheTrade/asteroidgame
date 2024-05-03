#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#include "interface.h"
#include "state.h"

State state;

KeyState keypresses(){
	// Check for keys presses
	KeyState keys = malloc(sizeof(*keys));

	if (IsKeyDown(KEY_UP)) keys->up = true;
	else keys->up = false;
	if (IsKeyDown(KEY_RIGHT)) keys->right = true;
	else keys->right = false;
	if (IsKeyDown(KEY_LEFT)) keys->left = true;
	else keys->left = false;
	if (IsKeyDown(KEY_ENTER)) keys->enter = true;
	else keys->enter = false;
	if (IsKeyDown(KEY_SPACE)) keys->space = true;
	else keys->space = false;
	if (IsKeyDown(KEY_N)) keys->n = true;
	else keys->n = false;
	if (IsKeyDown(KEY_P)) keys->p = true;
	else keys->p = false;

    return keys;
}

void update_and_draw() {
	KeyState keys = keypresses();
	
	state_update(state,keys);
	interface_draw_frame(state);

	free(keys);
}

int main() {
	state = state_create();
	interface_init();

	start_main_loop(update_and_draw);

	interface_close();

	return 0;
}