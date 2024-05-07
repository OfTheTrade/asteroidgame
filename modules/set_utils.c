#include "set_utils.h"

#include "common_types.h"
#include "ADTSet.h"

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μικρότερη τιμή του set που είναι μεγαλύτερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_greater(Set set, Pointer value){

    // If the exact value is found, return the location
    Pointer returnvalue = set_find(set,value);
    if (returnvalue != SET_EOF) return returnvalue;
    
    // If value is not found, insert it and find the next "biggest" value (The next on the right)
    set_insert(set,value);

    SetNode node = set_find_node(set, value);
    node = set_next(set, node);
    returnvalue = set_node_value(set, node);

    set_remove(set,value);

    return returnvalue;
}

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μεγαλύτερη τιμή του set που είναι μικρότερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_smaller(Set set, Pointer value){
    // If the exact value is found, return the location
    Pointer returnvalue = set_find(set,value);
    if (returnvalue != SET_EOF) return returnvalue;
    
    // If value is not found, insert it and find the next "smallest" value (The next on the left)
    set_insert(set,value);

    SetNode node = set_find_node(set, value);
    node = set_previous(set, node);
    returnvalue = set_node_value(set, node);

    set_remove(set,value);

    return returnvalue;
}