#ifndef MOVEMENT_H
#define MOVEMENT_H

// i = intersection fl = following line turn
void flTurn(int speed, int activeSensors); // use getActiveSensorsByInt(); here
void iTurn(int speed, bool left); // there should be a cooldown before trying to dettect line at MM

void foward(int speed, int time); // if time == -1 go on forever
void stop();

#endif
