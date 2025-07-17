#ifndef DFPLAYER_H
#define DFPLAYER_H

#include <DFRobotDFPlayerMini.h>

void initDFPlayer();
void tryToConnectDFPlayer(int maxAttempts = 5);
void playNextTrack();
void playTrack(int track);
#endif