
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
	state->objects = vector_create(0, NULL);

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
    Object crntobject;
	if ((state->info.paused == false)||(keys->n == true)){
          
		  // Update the objects in the vector, changing their position according to their speed
	      for(int i = 0;i<vector_size(state->objects);i++){
		        crntobject = (Object)vector_get_at(state->objects,i);
				crntobject->position=vec2_add(crntobject->position,crntobject->speed);
		  }
		  // The same but for the spaceship
		  state->info.spaceship->position = vec2_add(state->info.spaceship->position,state->info.spaceship->speed);
          
		  // If right is pressed move the spaceships orientation to the right
		  if (keys->right  == true) 
		        state->info.spaceship->orientation = vec2_rotate(state->info.spaceship->orientation, -SPACESHIP_ROTATION);
		  // If left is pressed move the spaceships orientation to the left
		  if (keys->left == true) 
		        state->info.spaceship->orientation = vec2_rotate(state->info.spaceship->orientation, SPACESHIP_ROTATION);
		  
		  // If up is pressed , add to the spaceship's speed.
		  if (keys->up == true){ 
		        state->info.spaceship->speed = vec2_add(state->info.spaceship->speed,vec2_scale(state->info.spaceship->orientation,SPACESHIP_ACCELERATION));				
		  }else if((state->info.spaceship->speed.x !=0)||(state->info.spaceship->speed.y !=0)){
		        // If up is not pressed, reduce the spaceship's speed	    
				int addorsub1=-1;
				int addorsub2=-1;
                
			    float xvalue = state->info.spaceship->speed.x;
				// Figure out if the horizontal speed is positive so we can reduce it properly (add or sub)
				if (xvalue<0){
					xvalue=-xvalue;
                    addorsub1=1;
				}
				// Figure out if the vertical speed is positive so we can reduce it properly (add or sub)
				float yvalue = state->info.spaceship->speed.y;
				if (yvalue<0){
					yvalue=-yvalue;
                    addorsub2=1;
				}
                
				// The scale is usefull because we need to reduce the speed
				// by reducing it's x and y cordinates while not changing it's orientation
			    double scale = (xvalue) / ((double)xvalue + (double)yvalue);

				float holdx = (float)(state->info.spaceship->speed.x + (addorsub1*scale*SPACESHIP_SLOWDOWN));
				float holdy = (float)(state->info.spaceship->speed.y + (addorsub2*(1-scale)*SPACESHIP_SLOWDOWN));

				// Slowing down should not result in negative speeds
				// If the starting speed was positive, it should not end up negative
				if (addorsub1 == -1){
				    if (holdx < 0) holdx = 0;
				}else{
					// If it was not positive, it should not end up positive
					if (holdx > 0) holdx = 0;
				}
				// Same for the y cordinate
				if (addorsub2 == -1){
				    if (holdy < 0) holdy = 0;
				}else{
				    if (holdy > 0) holdy = 0;
				}
				state->info.spaceship->speed = (Vector2){holdx,holdy};
			}		
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
}
