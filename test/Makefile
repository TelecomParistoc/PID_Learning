OBJ_DIRS = ../logging ../optimize_pid ../robot_actions
OBJS = $(addsuffix /obj/*.o, $(OBJ_DIRS))
SRCS = $(wildcard *.cpp)
BINS = $(patsubst %.cpp, %, $(SRCS))

BUILD = build

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I.. -g
LDFLAGS =


.PHONY: all clean $(BUILD)

all : $(BUILD) $(OBJ_DIRS) $(BINS)

% : %.cpp
	echo $@
	$(CC) $^ $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(BUILD)/$@

$(BUILD):
	@mkdir -p $@


../logging:
	make -C $@ obj

../optimize_pid:
	make -C $@ obj

../robot_actions:
	make -C $@ obj

clean-soft:
	@rm -rf $(BUILD)

clean: clean-soft
	@for dir in $(OBJ_DIRS) ; do \
        make -C $$dir clean ; \
    done


ifeq ($(wildcard ../threads/obj/*.o),)
ifeq ($(wildcard ../logging/obj/*.o),)
ifeq ($(wildcard ../robot_actions/obj/*.o),)
.PHONY: $(OBJ_DIRS)
endif
endif
endif
