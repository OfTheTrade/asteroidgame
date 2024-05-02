
#include <stdlib.h>

#include "ADTVector.h"
#include "ADTList.h"
#include "state.h"
#include "vec2.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	Vector objects;			// περιέχει στοιχεία Object (αστεροειδείς, σφαίρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	int next_bullet;		// Αριθμός frames μέχρι να επιτραπεί ξανά σφαίρα
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, Vector2 position, Vector2 speed, Vector2 orientation, double size) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->position = position;
	obj->speed = speed;
	obj->orientation = orientation;
	obj->size = size;
	return obj;
}

// Επιστρέφει έναν τυχαίο πραγματικό αριθμό στο διάστημα [min,max]

static double randf(double min, double max) {
	return min + (double)rand() / RAND_MAX * (max - min);
}

// Προσθέτει num αστεροειδείς στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα καρτεσιανό επίπεδο.
// - Η αρχή των αξόνων είναι η θέση του διαστημόπλοιου στην αρχή του παιχνιδιού
// - Στο άξονα x οι συντεταγμένες μεγαλώνουν προς τα δεξιά.
// - Στον άξονα y οι συντεταγμένες μεγαλώνουν προς τα πάνω.

static void add_asteroids(State state, int num) {
	for (int i = 0; i < num; i++) {
		// Τυχαία θέση σε απόσταση [ASTEROID_MIN_DIST, ASTEROID_MAX_DIST]
		// από το διστημόπλοιο.
		//
		Vector2 position = vec2_add(
			state->info.spaceship->position,
			vec2_from_polar(
				randf(ASTEROID_MIN_DIST, ASTEROID_MAX_DIST),	// απόσταση
				randf(0, 2*PI)									// κατεύθυνση
			)
		);

		// Τυχαία ταχύτητα στο διάστημα [ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED]
		// με τυχαία κατεύθυνση.
		//
		Vector2 speed = vec2_from_polar(
			randf(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED) * state->speed_factor,
			randf(0, 2*PI)
		);

		Object asteroid = create_object(
			ASTEROID,
			position,
			speed,
			(Vector2){0, 0},								// δεν χρησιμοποιείται για αστεροειδείς
			randf(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE)		// τυχαίο μέγεθος
		);
		vector_insert_last(state->objects, asteroid);
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.paused = false;				// Το παιχνίδι ξεκινάει χωρίς να είναι paused.
	state->speed_factor = 1;				// Κανονική ταχύτητα
	state->next_bullet = 0;					// Σφαίρα επιτρέπεται αμέσως
	state->info.score = 0;				// Αρχικό σκορ 0

	// Δημιουργούμε το vector των αντικειμένων, και προσθέτουμε αντικείμενα
	state->objects = vector_create(0, free);

	// Δημιουργούμε το διαστημόπλοιο
	state->info.spaceship = create_object(
		SPACESHIP,
		(Vector2){0, 0},			// αρχική θέση στην αρχή των αξόνων
		(Vector2){0, 0},			// μηδενική αρχική ταχύτητα
		(Vector2){0, 1},			// κοιτάει προς τα πάνω
		SPACESHIP_SIZE				// μέγεθος
	);

	// Προσθήκη αρχικών αστεροειδών
	add_asteroids(state, ASTEROID_NUM);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &(state->info);
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η θέση position βρίσκεται εντός του παραλληλογράμμου με πάνω αριστερή
// γωνία top_left και κάτω δεξιά bottom_right.

List state_objects(State state, Vector2 top_left, Vector2 bottom_right) {
	List list = list_create(free);
	// The current object from the main vector of objects, as well as it's cordinates
	Object crntobject;
	int crntx;
	int crnty;
    
	// The cordinates of top left
    int top_leftx=top_left.x;
	int top_lefty=top_left.y;

    // The cordinates of bottom right
	int bottom_rightx=bottom_right.x;
	int bottom_righty=bottom_right.y;

    // Loop for every object contained in vector
	for (int i=0;i<vector_size(state->objects);i++){
        
		// The current object is taken from the vector, and it's cordinates are held
        crntobject=(Object)vector_get_at(state->objects,i);
		crntx=crntobject->position.x;
		crnty=crntobject->position.y;
        
		// If the object is located within the bounds set by top right and bottom left, it is inserted in the list
		if ((top_leftx>=crntx)&(top_lefty>=crnty)&(bottom_rightx<=crntx)&(bottom_righty<=crnty)) list_insert_next(list,0,crntobject);
	}
    
	// The filled list is returned
	return list;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {

    // If p is pressed the game is paused
	if (keys->p == true) state->info.paused=true;
    
	// If the game isn't paused or n is pressed the state updates
	
	if ((state->info.paused == false)||(keys->n == true)){

        // Update the speed factor
		int scoreneeded = 1;
		for (float i = (state->speed_factor - 1); i >= 0; i = i - 0.1) scoreneeded = scoreneeded*100;
        if (state->info.score >= scoreneeded) state->speed_factor = state->speed_factor + 10;
        
        Object tempobject;
        Object crntobject;
	    Object bulletobject;
		int nearsteroids = 0;

		for(int i = 0;i<vector_size(state->objects);i++){

			crntobject = vector_get_at(state->objects,i);
			
		    // Update the objects in the vector, changing their position according to their speed
		    crntobject->position=vec2_scale(vec2_add(crntobject->position,crntobject->speed),state->speed_factor);

 			// Check to see how many asteroids are near the ship. Calculates the distance between
			// each asterpoid and the ship (raised to the second power) and checks that is <= the
			// max distance (raised to the second power) allowed
			float distxsqr = ((crntobject->position.x) - (state->info.spaceship->position.x))*((crntobject->position.x) - (state->info.spaceship->position.x));
			float distysqr = ((crntobject->position.y) - (state->info.spaceship->position.y))*((crntobject->position.y) - (state->info.spaceship->position.y));
			if ((distxsqr + distysqr) <= (ASTEROID_MAX_DIST*ASTEROID_MAX_DIST)){
				nearsteroids = nearsteroids + 1;
			}

		}
       
	   // Collision checks
       for(int i = 0;i<vector_size(state->objects);i++){

          crntobject = vector_get_at(state->objects,i);

		  if (crntobject->type == ASTEROID){
                
				// Check if this asteroid (crntobject) has colided with the spaceship
                if (CheckCollisionCircles(state->info.spaceship->position,SPACESHIP_SIZE,crntobject->position,crntobject->size)){

				    // Destroy the asteroid
					vector_set_at(state->objects,i, vector_get_at(state->objects,vector_size(state->objects) - 1));
					vector_remove_last(state->objects);

					// Update the score
					state->info.score = (int)((state->info.score)/2);
					
			    }else{ 
                
				   // Check if this asteroid (crntobject) has colided with this bullet (crntbullet);
			       for(int j = 0; j<vector_size(state->objects);j++){

				       bulletobject = vector_get_at(state->objects,j);	

				       // If the object is a bullet
				       if (bulletobject->type == BULLET){
					       // If it collides with this (crntobject) asteroid
						   if (CheckCollisionCircles(state->info.spaceship->position,SPACESHIP_SIZE,bulletobject->position,BULLET_SIZE)){

							   // If the asteroid will break into two or dissapear
							   if ( ( (crntobject->size)/2 ) >= ASTEROID_MIN_SIZE ){

								    // Add two smaller asteroids
                                    tempobject = create_object(ASTEROID,crntobject->position,vec2_rotate(vec2_scale(crntobject->speed,1.5),randf(0,2*PI)),(Vector2){0,0},(crntobject->size)/2 );
								    vector_insert_last(state->objects,tempobject);

								   tempobject = create_object(ASTEROID,crntobject->position,vec2_rotate(vec2_scale(crntobject->speed,1.5),randf(0,2*PI)),(Vector2){0,0},(crntobject->size)/2 );
								   vector_insert_last(state->objects,tempobject);
							   }
							   // Destroy the asteroid and the bullet
							   vector_set_at(state->objects,i, vector_get_at(state->objects,vector_size(state->objects) - 1));
							   vector_remove_last(state->objects);

							   vector_set_at(state->objects,j, vector_get_at(state->objects,vector_size(state->objects) - 1));
							   vector_remove_last(state->objects);
                            
							  // Update the score
							  state->info.score = state->info.score - 10;
							  if (state->info.score <= 0) state->info.score = 0;
						   }

				       }

			      }

				}
			
			}

	    }

	   
		// Add any asteroids needed to have ASTEROID_NUM many near the ship. Update the score
		if (nearsteroids < ASTEROID_NUM){
			 add_asteroids(state,ASTEROID_NUM - nearsteroids);
			 state->info.score = state->info.score + ASTEROID_NUM - nearsteroids;
		}

		// Update the spaceships location according to it's speed.
		state->info.spaceship->position = vec2_scale(vec2_add(state->info.spaceship->position,state->info.spaceship->speed),state->speed_factor);
          

        // If the space key is pressed and a bullet has not been fire for 15 states, create a bullet
		if ( (keys->space == true)&&(state->next_bullet <= 0) ){
			state->next_bullet = BULLET_DELAY + 1;
			// The bullet is created at the spaceship's location, with it's speed depending on the 
			// spaceships own speed and orientation as well as BULLET_SPEED.
			Object bullet = create_object(BULLET, state->info.spaceship->position, vec2_add(state->info.spaceship->speed,vec2_scale(state->info.spaceship->orientation,BULLET_SPEED)),(Vector2){0,0},BULLET_SIZE);
            vector_insert_last(state->objects, bullet);
		}
		state->next_bullet = state->next_bullet - 1;

		// If right is pressed move the spaceships orientation to the right
		if (keys->right  == true) 
		        state->info.spaceship->orientation = vec2_rotate(state->info.spaceship->orientation, -SPACESHIP_ROTATION);
		// If left is pressed move the spaceships orientation to the left
		if (keys->left == true) 
		        state->info.spaceship->orientation = vec2_rotate(state->info.spaceship->orientation, SPACESHIP_ROTATION);
		  
		// If up is pressed , add to the spaceship's speed.
		if (keys->up == true){ 
		    state->info.spaceship->speed = vec2_add(state->info.spaceship->speed,vec2_scale(state->info.spaceship->orientation,SPACESHIP_ACCELERATION));				
		}else{
		    state->info.spaceship->speed = vec2_scale(state->info.spaceship->speed,SPACESHIP_SLOWDOWN);
		}		
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Free all parts of "state" that are in the heap and then "state" itself
	free(state->info.spaceship);
    vector_destroy(state->objects);
	free(state);
}
