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
- ...

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
