OBJ_DIR = obj
SRC_DIR = .
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))

CC = /home/pi/sol_prog-raspberry-pi-gcc-binary-47d234b6e26a/gcc-7.2.0/bin/g++-7.2.0 # g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14


all: createDir $(OBJ)

$(OBJ_DIR)/%.o: %.cpp $(OBJ_DIR)/%.d
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR)/%.d: %.cpp
	$(CC) $(CFLAGS) -MT $(patsubst %.d, %.o, $@) -MM -MF $@ -MP $<

obj: createDir $(OBJ)

createDir:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)


ifneq ($(wildcard $(OBJ_DIR)/.*),)
-include $(OBJ:%.o=%.d)
endif
