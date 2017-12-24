SRCS = $(wildcard *.cpp)
BINS = $(patsubst %.cpp, %, $(SRCS))
OBJS = $(patsubst %, %.o, $(BINS))

BUILD = build

CC = g++
CFLAGS = -Wall -Werror -Wextra -O1 -std=c++14 -I.. -g
LDFLAGS =


.PHONY: all clean $(BUILD)

all : $(BUILD) $(BINS)

% : $(SRCS)
	echo $@
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $(BUILD)/$@

$(BUILD):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD)
