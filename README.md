![run-tests](../../workflows/run-tests/badge.svg)

## 2024 Project 1

Εκφώνηση: https://k08.chatzi.org/projects/project1/


### Προσωπικά στοιχεία

__Όνομα__: Ορλάνδος Αλέξιος

__Α.Μ.__: sdi2300149

### Ασκήσεις που παραδίδονται

Συμπληρώστε εδώ τις ασκήσεις (και τυχόν bonus) που παραδίδετε. Αν κάποια άσκηση
οβρίσκεται σε μη αναμενόμενα αρχεία αναφέρετε και τα αρχεία αυτά.
- 1
- 2
- 3

### Documentation

Συμπληρώστε εδώ __όσο documentation χρειάζεται__ ώστε οι βαθμολογ
ητές να
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

Firstly, the function now updates "speed_factor" when called and the game is not paused. It figures out how many times
it has been increased already from it's starting value of 1 and then checks if the game score is hight enough to cause
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
