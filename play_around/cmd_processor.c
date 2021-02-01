#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "cmd_processor.h"

const char * day[7] = {
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat",
  "Sun"
};

const char * weather_lst[7] = {
  "Blizzard",
  "Comet Strike",
  "Drought",
  "Comet Strike",
  "Volcanic eruption",
  "Flood",
  "Meteor Shower",
};

const char* get_weather(char cmd[]) {
  size_t len = strlen(cmd) - 1;
  if (*cmd && cmd[len] == '\n') {
    cmd[len] = '\0';
  }

  if (strcmp(cmd, "quit") == 0) {
    return "exit";
  }

  for (int i = 0; i < 7; i++) {
    if (strcmp(cmd, day[i]) == 0) {
      return weather_lst[i];
    }
  }
  return "Invalid command";
}