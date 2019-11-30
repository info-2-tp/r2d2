#include "../inc/DR_tipos.h"
#define IZQ 0
#define DER 1

#define UP 1
#define DOWN 0

#define FRONT 0
#define BACK 1

#define BASE_MOTOR   2
#define KNIFES_MOTOR 3
#define TOWER_MOTOR  4

void moveteMotorPWM(int motor,int estado,int direccion);

void turnOnPWM(uint8_t on);
