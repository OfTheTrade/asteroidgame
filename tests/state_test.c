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
		TEST_ASSERT((crntobject->position.x <= ASTEROID_MAX_DIST)&&(crntobject->position.x >= -ASTEROID_MAX_DIST));
		TEST_ASSERT((crntobject->position.y <= ASTEROID_MAX_DIST)&&(crntobject->position.y >= -ASTEROID_MAX_DIST));

	    TEST_ASSERT((crntobject->position.x >= ASTEROID_MIN_DIST)||(crntobject->position.x <= ASTEROID_MIN_DIST));
		TEST_ASSERT((crntobject->position.y >= ASTEROID_MIN_DIST)||(crntobject->position.y <= ASTEROID_MIN_DIST));  
        
		// Check that the size of the asteroid is within acceptable bounds
		TEST_ASSERT((crntobject->size <= ASTEROID_MAX_SIZE)&&(crntobject->size >= ASTEROID_MIN_SIZE));
         
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

	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->position, (Vector2){0,0}) );
	TEST_ASSERT( vec2_equal( state_info(state)->spaceship->speed,    (Vector2){0,SPACESHIP_ACCELERATION}) );

	// Προσθέστε επιπλέον ελέγχους
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};
