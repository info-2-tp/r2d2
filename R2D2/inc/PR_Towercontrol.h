#include "DR_tipos.h"

#define STEP 	25

extern volatile uint16_t TowerPositionOld;
extern volatile uint16_t TowerPosition;

void Tower_Control ( uint16_t milimeters , uint8_t direction );
void update_tower( void );
void reset_tower( void );
