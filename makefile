# TODO: May need -lSDl2_image and -lSDL2_ttf

PROGRAM = Gravity
SOURCES = src
BUILD   = build
DEVEXT  = -dev
CXX    = clang++
FLAGS  = -Wall -pedantic -std=c++14 
CFLAGS = $(shell sdl2-config --cflags)
LIBS   = $(shell sdl2-config --libs) -lpthread -lOpenCL
MKDIR  = mkdir -p
RM     = rm -rf

MODE = release
BNRY = $(PROGRAM)
FLGS = $(FLAGS)
SDRS = $(shell find $(SOURCES) -type d | xargs echo)
SRCS = $(foreach d,. $(SDRS),$(wildcard $(addprefix $(d)/*,.cpp)))
OBJS = $(patsubst %.cpp,%.o,$(addprefix $(BUILD)/$(MODE)/,$(SRCS)))
DEPS = $(OBJS:%.o=%.d)
DIRS = $(sort $(dir $(OBJS)))

ifdef DEBUG
	BNRY := $(BNRY)$(DEVEXT)
	FLGS += -g -DDEBUG_MODE
	MODE  = debug
else
	FLGS += -O3
endif

.PHONY: all clean

all: $(BNRY)

clean:
	$(RM) $(BUILD) $(PROGRAM) $(PROGRAM)$(DEVEXT)

$(DIRS):
	$(MKDIR) $@

$(BNRY): $(OBJS)
	$(CXX) $(FLGS) $(LIBS) $(OBJS) -o $@

$(OBJS): | $(DIRS)

$(BUILD)/$(MODE)/%.o: %.cpp
	$(CXX) $(FLGS) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
