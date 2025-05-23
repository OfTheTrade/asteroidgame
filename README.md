![run-tests](../../workflows/run-tests/badge.svg)

## 2024 Project 1

Εκφώνηση: https://k08.chatzi.org/projects/project1/


### Προσωπικά στοιχεία

__Όνομα__: Ορλάνδος Αλέξιος

__Α.Μ.__: sdi2300149

### Ασκήσεις που παραδίδονται

Συμπληρώστε εδώ τις ασκήσεις (και τυχόν bonus) που παραδίδετε. Αν κάποια άσκηση
βρίσκεται σε μη αναμενόμενα αρχεία αναφέρετε και τα αρχεία αυτά.
- 1
- 2
- 3
- 4
-
- 6
### Documentation

Συμπληρώστε εδώ __όσο documentation χρειάζεται__ ώστε οι βαθμολογητές να
κατανοήσουν πλήρως τις λύσεις σας και να τις βαθμολογήσουν ανάλογα. Αυτό θα
πρέπει να γίνει ανεξάρτητα με το αν ο κώδικάς σας είναι καλά σχολιασμένος,
πράγμα που συνιστάται.

Part 1:
In state.c, state_info.c and state_objects.c were filled.

state_info.c, simply returns the struct pointer "info", which it finds by looking in the struct pointed to by "state".

state_objects.c creates and fills a list containing all objects contained in the vector containing all game objects
(contained in the struct "state") whose cordinates are within the bounds dictated by "top_left" and "bottom_right"
This is accomplished trough a loop, with every loop the next object is checked from the given vector until we reach
it's end. Every loop, we compare the x and y cordinates of the object and the given x and y cordinates of "top_left" and 
"bottom_right", and if the object is within acceptable bounds, it's added to the list. In the end the pointer to the
list is returned.

In state_test.c, test_state_create() was filled. Three types of tests were added:
1. Tests to see the spaceship's struct was created and filled with the correct data. 
2. Tests to see the that state_create creates asteroids in the correct location
(as dictated by ASTEROID_MAX_DISTANCE and ASTEROID_MIN_DISTANCE). To do this, state_objects is used, as an object
is only included in the list it creates if it is in the correct location as dictated by "top_left" and "bottom_right"
3. Tests to check if the objects created are of the correct type (asteroids), location and size. To do this,
state_objects is again used to create a list of all objects created by state_create.

Part 2:
In state.c, state_update was filled. In state_test.c, test_state_update was filled.

state_update handles updating the state to the next frame. It recieves the state and the struct of the keys
being pressed this frame. Firstly if the key p is pressed, the game becomes paused. Afterwards, if the game
isn't paused or n is pressed the rest of the function is passed. Else the function ends there. Next, the 
function updates all object in the vector (accessed through the struct "state") by changing their location 
according to their speed. Next the same thing happens but for the spaceship object. Then, if right is pressed,
the spaceship's orientation is rotated by the ammount dictated by (SPACESHIP_ROTATION) to the right. The same
then happens for the left, if the left key is pressed. If up is pressed, the spaceships speed is increased by
the ammmount dictated by it's rotation and (SPACESHIP_ROTATION). If it is not pressed, the spaceships speed is
decreased by suntracting or adding (depending on if the starting speed is positive or negative) to the speed vectors
x any y floats. Because the speed's orientation needs to stay the same, calculating the scale between the x speed
and the y speed is required. This scale is then used to moddify the two speeds correctly. If any of the x or y
speeds are 1. Negative (if the starting x or y speed was positive) 2. Positive (if the starting x or y speed
was positive) , it or they instead become zero.

test_state_update tests the previous function by pressing different buttons and calling state_update to see if it
updates the state properly. The following tests were added: 
1. A test for the p button. The function makes sure that if p is pressed, the state does not update.
2. A test for the n button. The function makes sure that if n is pressed while the game is paused, the
game updates the state despite it's paused statues, for one frame.
3. A test that if spaceship has gained a speed (SPACESHIP_ACCELERATION in this case), it will move that many
pixels in the correct direction next frame. In the same test, it is checked that when up is not pressed, the
spaceship loses speed (SPACESHIP_SLOWDOWN), but does not gain speed in the opposite direction.
4. A test for the right button. It is checked that the spaceship changes orientation to the right when pressing
the right button. In the same test, it is checked that the spaceship can gain speed on both the x and y axis (now
that it has turned from looking straight up).
5. A test for the left button. Since the ship has turned right for one frame and left for one frame, it should
have returned to it's original rotation.  In the same test, it is checked that the spaceship can move on both the
x and y axis (now that it has gained speed in both axis)
that it has turned from looking straight up).

Part 3:
In state.c, state_update was filled with the new requierments.
A check for bullets was added to state_test.c.

Firstly, the function now updates "speed_factor" when called and the game is not paused. It figures out how many times
it has been increased already from it's starting value of 1 and then checks if the game score is high enough to cause
"speed_factor" to increase again. In the loop previously only used to change the coordinates of the objects in the states
object vector, a counter has been set up to count how many asteroids are within ASTEROID_MAX_DIST of the ship. After it 
exits the loop, if this ammount is less that ASTEROID_NUM, it adds enough asteroids for this number to become ASTEROID_NUM
through add_asteroids. The score is increased by one for every asteroid created this way. Collision checks where also 
added. The function parses the entire state vector , and (for every node) if the object it finds inside is a asteroid, it
first checks if it collides with the spaceship (through CheckCollisionCircles). If it does, the asteroid is removed from the
vector (by swapping the last nodes value with the current one and then removing the last node), and the score is halved. If 
it does not collide with the spaceship the vector of objects is accesed again , checking if any of the objects is a bullet
that collides with the current asteroid. If it is, the asteroid and the bullet is destroyed, 10 points are deducted from the
score (without going bellow zero) and if the asteroid is big enough two smaller and faster asteroids are spawned at it's location.
Lastly, the function now checks for a space key input. If it detects that, and it has been not been fired for 15 states, a bullet
is created on the spaceship, with it's speed being affected by BULLET_SPEED and the spaceships rotation and speed.

In state_test, a test function was added named test_state_update_two(). It tests the abillity of the spaceship to fire bullets:
1. It checks that pressing space spawns a bullet at the spaceships nose.
2. It checks that that bullet was created with the correct data in it's struct.
3. It checks that next frame the bullet will have moved according to it's speed.
4. It checks that another bullet won't be fired until 15 frames have passed. Another bullet will be fired on the 16nth frame. 

Part 4:
In include, interface.h was added.
In modules, interface.c was added.
In tests, state_test was updated to include a test called test_set_utils.

1. interface.h is a header file containing the statement of two function, set_find_eq_or_greater() and set_find_eq_or_smaller().
2. interface.c contains the implementation of those two. set_find_eq_or_greater does on of two things. If the given value is
contained inside the given set, that value is returned. If it not, then the value is inserted into the set, the new node containing
the value is found, the next node is reached (containing the next "biggest", according to the comparison function of the set) and
after the node containing the original value is removed, the value of the next node is returned. The same thing happens for
set_find_eq_or_smaller, except that the previous node's value is returned.
3. state_test.c now contains a function called test_set_utils. First it fills the set with some strings. It checks that the first
node is not SET_EOF (that it is not empty) and that the size of the list is correct. It then accesses the nodes linearly, making sure
that they are in ascending order (according to the comparison function). Lastly, set_find_eq_or_greater and set_find_eq_or_smaller
are checked, first that if they are given a value that is already inside the set, they will return that value, and if they are given
something else, they will return the next biggest or smallest value.


Part 6:
In include, interface.h was added.
In modules, interface.c was added.
In program/game, program.c and a makefile was added.

interface.h simply states the three function that initiate, update, and close the interface

interface.c contains the implementation of those functions. It contains:
1. interface_ititiate, that uses raylib to open the correct size of window and set the correct target fps
2. interface_close, that simply uses raylib to close the window
3. interface_draw_frame, that updates the interface for every frame. It uses raylib to begin drawing, first
clearing the background to be black, then calculating some vectors that will be used to transform the state
cordinates to interface cordinates. The midddlevector shows the middle of the screen, flippedorient and
flippedposition are the spaceships orientation and position with the y cordinated reversed, while transform
vector is the vector that will be added to the objects to move them to the correct interface cordinate.  It
first draws the spaceship, making sure to flip it's y cordinate. It is made out of an isoskeles triangle, with
each angle being 2π/3. A small circle also is drawn on it's front edge. The bounds of the space of the state
that we will use are calculated, and state_objects is used to return a list of all objects within. Now we will
access all nodes of the list, drawing either a bullet (full circle) or asteroid (line circle) on the cordinates
given by adding the transform vector to the location vector of the object (with the y cordinate reversed). Lastly,
an fps counter is drawn, and if the game is paused a pause message is also drawn. Raylib is used to stop drawing. 

game.c uses all previous programs to finally make a functional game.
1. It uses state_crate() to create a state. Then , interface_init() to initialize the interface.
2. The raylib function start_main_loop is used to call the function update_and_draw repeatedly until the game window closes.
Update and draw first uses the function keypresses to detect all keys pressed during this loop, then state_update is called
with the current state and the key presses. interface_draw_frame is then called with the state to update the interface. The
struct of keys created by keypresses() is freed. keypressess itself simply uses raylib to detect the pressed keys and updates
the key struct it creates with the correct bools.
3. Interface_close is called to close the interface. state_destroy is used to destroy the state and the program returns zero.
The program ends.

The makefile is used to compile game.c
