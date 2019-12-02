#include "../inc/PR_Relay.h"

void relay_control(uint32_t RelayID,uint32_t Action)
{
	buffer_relay[RelayID] = Action;
	active_relay();
}
