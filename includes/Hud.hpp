#pragma once
#include "Player.hpp"
#include <string>


void stampa(const char * s);

void printGameHud(Player player, int evento, int p, int level, int loop);

void printStats(Player p, int level, int loop);

void printLog(int evento, int param, int loop);

std::string flavorTextA();
std::string flavorTextB();
