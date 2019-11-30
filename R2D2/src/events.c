//
// Created by marcelo on 2/11/19.
//

#include "../inc/PR_buttons.h"
#include "../inc/actions.h"

message_header_t header;

char base_back() {
    return 0;
}

char base_middle() {
    return 0;
}

char base_front() {
    return base_on_init();
}

char emergency_button() {
    return stop_button_active();
}

char knifes_top() {
    return knifes_tower_on_top();
}

char run_button() {
    return run_button_active();
}

char knifes_are_ready() {
    return 0;
}

char knifes_is_there(unsigned short position) {
    return 0;
}
