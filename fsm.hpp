#pragma once

#ifndef __FSM_HPP__
#define __FSM_HPP__

enum class FsmState {
    mainMenu,
    accounts,
    // CLI enabled
    // Factory Reset
};

enum class FsmEvent {
    nextMenuItem,
    prevMenuItem,
    enterMenuItem,
    leaveMenuItem
};

void fsm_jump_to(FsmState s);
void fsm_process(FsmEvent ev);

#endif // __FSM_HPP__
