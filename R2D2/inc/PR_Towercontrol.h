#include "DR_tipos.h"

#define STEP 	24

extern volatile uint16_t TowerPositionOld;
extern volatile uint16_t TowerPosition;

void Tower_Control ( uint16_t milimeters );
void update_tower( void );
