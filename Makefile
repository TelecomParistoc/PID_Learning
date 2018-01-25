TARGET = PID_learning
TARGET_DIR = obj
OBJ = $(wildcard logging/obj/*.o optimize_pid/obj/*.o robot_actions/obj/*.o simulated_annealing/obj/*.o)

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14
LDFLAGS =-lmotors -lrobotutils

.PHONY: all

$(TARGET):
	@echo $(OBJ)
	make -C logging
	make -C optimize_pid
	make -C robot_actions
	make -C simulated_annealing
	$(CC) $(LDFLAGS) -o $(TARGET_DIR)/$(TARGET) $(OBJ)

all: $(TARGET)
