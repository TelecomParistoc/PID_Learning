OBJ_DIRS = ../logging ../optimize_pid ../robot_actions
OBJS = $(addsuffix /obj/*.o, $(OBJ_DIRS))
SRCS = $(wildcard *.cpp)
BINS = $(patsubst %.cpp, %, $(SRCS))

BUILD = build
ADDITIONAL = logs

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++11 -I.. -g
LDFLAGS = -lmotors -lrobotutils


.PHONY: all clean $(BUILD)

all : $(BUILD) $(ADDITIONAL) $(OBJ_DIRS) $(BINS)

% : %.cpp
	$(CC) -v $^ $(OBJS) $(CFLAGS) -o $(BUILD)/$@ $(LDFLAGS)

$(BUILD):
	@mkdir -p $@

$(ADDITIONAL):
	@mkdir -p $@

../logging:
	make -C $@ obj

../optimize_pid:
	make -C $@ obj

../robot_actions:
	make -C $@ obj

clean-soft:
	@rm -rf $(BUILD)
	@rm -rf $(ADDITIONAL)

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
