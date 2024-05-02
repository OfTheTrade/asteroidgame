//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"


///// Βοηθητικές συναρτήσεις ////////////////////////////////////////
//
// Ελέγχει την (προσεγγιστική) ισότητα δύο double
// (λόγω λαθών το a == b δεν είναι ακριβές όταν συγκρίνουμε double).
static bool double_equal(double a, double b) {
	return fabs(a-b) < 1e-6;
}

// Ελέγχει την ισότητα δύο διανυσμάτων
static bool vec2_equal(Vector2 a, Vector2 b) {
	return double_equal(a.x, b.x) && double_equal(a.y, b.y);
}
/////////////////////////////////////////////////////////////////////


void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);

    // Test that the spaceship was created properly
	TEST_ASSERT(info->spaceship !=NULL);

	TEST_ASSERT(info->spaceship->orientation.x == 0 );
	TEST_ASSERT(info->spaceship->orientation.y == 1 );

	TEST_ASSERT(info->spaceship->position.x == 0 );
	TEST_ASSERT(info->spaceship->position.y == 0 );

	TEST_ASSERT(info->spaceship->size == SPACESHIP_SIZE );
	TEST_ASSERT(info->spaceship->type == SPACESHIP );

    // The area of the screen is set so to this size (<ASTEROID_MAX_DIST) so that it 
	// does not contain any of the asteroids created by state_create  
    Vector2 topright = (Vector2){1,1}; 
	Vector2 bottomleft = (Vector2){-1, -1};
    List list = state_objects(state,topright,bottomleft);
    
	// The list should exist and contain all asteroids contained in the area set by topright and bottomleft 
	// None, in this case
	TEST_ASSERT(list!=NULL);
	TEST_ASSERT(list_size(list) == 0);

    // This area of the screen is set to this size (>=ASTEROID_MAX_DIST) so that it contains 
	// all asteroids created by state_create
    topright = (Vector2){ASTEROID_MAX_DIST, ASTEROID_MAX_DIST};
	bottomleft = (Vector2){-ASTEROID_MAX_DIST, -ASTEROID_MAX_DIST};
    list = state_objects(state,topright,bottomleft);
    
	// The list should exist and contain all asteroids contained in the area set by topright and bottomleft
	// In this case all (ASTEROID_NUM many) asteroids created by state_create
    TEST_ASSERT(list != NULL);
    TEST_ASSERT(list_size(list) == ASTEROID_NUM);
    
	// The lists contents will be checked
	ListNode crntnode = list_first(list);
	Object crntobject;
	// When the lists is finished
	while (crntnode!=NULL){
		// We grab the current object from the list
		crntobject = list_node_value(list,crntnode);

		//Check that it is an asteroid
		TEST_ASSERT(crntobject->type == ASTEROID);
        
		// Check that the distance to the spaceship (0.0 currently) is within bounds 
		float distsqr = (crntobject->position.x)*(crntobject->position.x) + (crntobject->position.y)*(crntobject->position.y);

		TEST_ASSERT(distsqr <= ASTEROID_MAX_DIST*ASTEROID_MAX_DIST);
		TEST_ASSERT(distsqr >= ASTEROID_MAX_DIST*ASTEROID_MAX_DIST);
		
		// Check that the size of the asteroid is within acceptable bounds
		TEST_ASSERT((crntobject->size <= ASTEROID_MAX_SIZE)&&(crntobject->size >= ASTEROID_MIN_SIZE));

        // Check to see speed is within acceptable bounds
		float linex = (crntobject->speed.x)*(crntobject->speed.x);
		float liney = (crntobject->speed.y)*(crntobject->speed.y);

		TEST_ASSERT(linex + liney <= ASTEROID_MAX_SPEED*ASTEROID_MAX_SPEED);
		TEST_ASSERT(linex + liney >= ASTEROID_MIN_SPEED*ASTEROID_MIN_SPEED);
		// Moving on to the next node
		crntnode=list_next(list,crntnode);
    }

}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false };

	// Χωρίς κανένα πλήκτρο, το διαστημόπλοιο παραμένει σταθερό με μηδενική ταχύτητα
	state_update(state, &keys);
	
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,0}) );

	// Με πατημένο το πάνω βέλος, η ταχήτητα αυξάνεται ενώ το διαστημόπλοιο παραμένει για την ώρα ακίνητο
	keys.up = true;
	state_update(state, &keys);

	// (INTERMITION) Pressing p pauses the game. It remains in the last state.

	keys.p = true;
	state_update(state, &keys);

    // Same as before
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,SPACESHIP_ACCELERATION}) );
    
	keys.p = false;
	state_update(state, &keys);

    // Same as before
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,SPACESHIP_ACCELERATION}) );
    
	// While pressing n the game will continue
	keys.n = true;

    // Without any further presses, the spaceship will move as dictated by the speed it was given before (SPACESHIP_ACCELERATION).
	// Then the speed will decrease by SPACESHIP_SLOWDOWN %.
	keys.up = false;
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,SPACESHIP_ACCELERATION}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed, (Vector2){0,SPACESHIP_ACCELERATION*SPACESHIP_SLOWDOWN}) );

    // Pressing only right, the orientation of the ship should change.
	// If we also press up, the ships speed should increase in both axes (x and y)
	keys.right = true;
	keys.up = true;
    state_update(state, &keys);
	
	TEST_ASSERT( !vec2_equal( state_info(state)->spaceship->orientation, (Vector2){0,0}));

	TEST_ASSERT( state_info(state)->spaceship->speed.x !=0);
	TEST_ASSERT( state_info(state)->spaceship->speed.y !=0);

	// Pressing left once after pressing right once should return the ship to it's original orientation (0,1)
	// After accelerating in both axes, the ships x and y cordinates should change.
	keys.up = false;
    keys.right = false;
    keys.left = true;
	state_update(state, &keys);

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->orientation, (Vector2){0,1}));

	TEST_ASSERT( state_info(state)->spaceship->position.x !=0);
	TEST_ASSERT( state_info(state)->spaceship->position.y !=0);	

	keys.left = false;
	keys.n = false;

	state_destroy(state);

    //// TESTS FOR COLLISIONS

	state_create(state);


}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
