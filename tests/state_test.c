//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"
#include "set_utils.h"


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
    Vector2 topleft = (Vector2){-1,1}; 
	Vector2 bottomright = (Vector2){1, -1};
    List list = state_objects(state,topleft,bottomright);
    
	// The list should exist and contain all asteroids contained in the area set by topright and bottomleft 
	// None, in this case
	TEST_ASSERT(list!=NULL);
	TEST_ASSERT(list_size(list) == 0);

    // This area of the screen is set to this size (>ASTEROID_MAX_DIST) so that it contains 
	// all asteroids created by state_create 
    topleft = (Vector2){-ASTEROID_MAX_DIST, ASTEROID_MAX_DIST};
	bottomright = (Vector2){ASTEROID_MAX_DIST, -ASTEROID_MAX_DIST};
    list = state_objects(state,topleft,bottomright);
    
	// The list should exist and contain all asteroids contained in the area set by topright and bottomleft
	// In this case all (ASTEROID_NUM many) asteroids created by state_create
    TEST_ASSERT(list != NULL);
    TEST_ASSERT(list_size(list) == ASTEROID_NUM);
    
	// The lists contents will be checked
	ListNode crntnode = list_first(list);
	Object crntobject;
	// When the lists is finished
	while (crntnode!=LIST_EOF){
		// We grab the current object from the list
		crntobject = list_node_value(list,crntnode);

		//Check that it is an asteroid
		TEST_ASSERT(crntobject->type == ASTEROID);
        
		// Check that the distance to the spaceship (0.0 currently) is within bounds 
		float distsqr = (crntobject->position.x)*(crntobject->position.x) + (crntobject->position.y)*(crntobject->position.y);
		
		TEST_ASSERT(distsqr <= (ASTEROID_MAX_DIST*ASTEROID_MAX_DIST));
		TEST_ASSERT(distsqr >= (ASTEROID_MIN_DIST*ASTEROID_MIN_DIST));
		
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

}

void test_state_update_two(){

	State state = state_create();
    
	// Only the fire bullet button is pressed
    struct key_state keys = { false, false, false, false, false, false, false };
	keys.space = true;
    
	// The state updates
	state_update(state,&keys);

	// The bullet is created at the spaceships nose. Since the ship has not moved, it's nose is located at {0, 1*SPACESHIPS_SIZE}
	Vector2 bulletlocation = {0,1*SPACESHIP_SIZE};
	List list = state_objects(state,bulletlocation,bulletlocation);

    // The list should not be empty, it should contain only one object (the bullet)
	TEST_ASSERT(list_first(list) != LIST_EOF);
	TEST_ASSERT(list_size(list) == 1);
    
	Object crntobject = list_node_value(list,list_first(list));

	// Check the object's data
	// If it's a bullet
    TEST_ASSERT(crntobject->type == BULLET);

    // If it has the correct location
	TEST_ASSERT(crntobject->position.x == bulletlocation.x);
	TEST_ASSERT(crntobject->position.y == bulletlocation.y);

	// If it has the correct speed
	TEST_ASSERT(crntobject->speed.x == 0);
	TEST_ASSERT(crntobject->speed.y == BULLET_SPEED);

	// If it has the correct size
	TEST_ASSERT(crntobject->size == BULLET_SIZE);

	// If it has the correct orientation (always 0,0)
   	TEST_ASSERT(crntobject->orientation.x == 0);
	TEST_ASSERT(crntobject->orientation.y == 0);

	

	state_update(state,&keys);

	// The location should have changed according to the speed
	TEST_ASSERT(crntobject->position.x == 0);
	TEST_ASSERT(crntobject->position.y == bulletlocation.y + BULLET_SPEED );
	
	// Even though the fire button is pressed, no bullet should have spawned, as not enough frames have passed yet
    list = state_objects(state,bulletlocation,bulletlocation);

	TEST_ASSERT(list_first(list) == LIST_EOF);

    // 15 updates + the previous one 
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);
	state_update(state,&keys);

    // On the 16nth frame, a bullet should have been spawned
	list = state_objects(state,bulletlocation,bulletlocation);

	TEST_ASSERT(list_first(list) != LIST_EOF);

	state_destroy(state);

}

void test_set_utils(){

	// Compare function returns positive if the first int is bigger, zero if they are equal or negative otherwise
	Set set = set_create((CompareFunc)strcmp,NULL);

	char* string1 = "aa";
	set_insert(set,string1);

	char* string2 = "an";
	set_insert(set,string2);

	char* string3 = "ag";
	set_insert(set,string3);

	char* string4 = "ad";
    set_insert(set,string4);
    
	SetNode node = set_first(set);

    // List should not be empty
	TEST_ASSERT( node != SET_EOF );

	// Check for correct size
	TEST_ASSERT(set_size(set) == 4);

    // First compare the first two nodes
	char* crntvalue;
	char* previousvalue = set_node_value(set, node);
	node = set_next(set, node);
	

	// Set should be in ascending order
	while (node != SET_EOF){
		crntvalue = set_node_value(set, node);

        TEST_ASSERT(strcmp(crntvalue,previousvalue) > 0);

		previousvalue = crntvalue;
		node = set_next(set, node);
	}

	// "ag" is contained in the set
	char* string5 = "ag";
	crntvalue = set_find_eq_or_greater(set, string5);

	TEST_ASSERT( strcmp(crntvalue,"ag") == 0 );

	// "ad" is contained in the set 
	char* string6 = "ad";
	crntvalue = set_find_eq_or_smaller(set, string6);

	TEST_ASSERT( strcmp(crntvalue,"ad") == 0 );

	// ac is not contained in the set. It's bigger than "aa"
	char* string7 = "ac";
	crntvalue = set_find_eq_or_greater(set, string7);

	TEST_ASSERT( strcmp(crntvalue,"ad") == 0 );
   
    // ac is not contained in the set. It's smaller than "ad"
    char* string8 = "ac";
	crntvalue = set_find_eq_or_smaller(set, string8);

	TEST_ASSERT( strcmp(crntvalue,"aa") == 0 );

	set_destroy(set);

}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },
	{ "test_state_update_two", test_state_update_two },
    { "test_state_utils" , test_set_utils },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
