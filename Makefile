
vpath %.c src

.LIBPATTERNS = lib%.a
TARGET = SocketServer
CFLAGS = -O3 -Wall -c
LIBS =
OBJS := $(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

all: $(TARGET)
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	g++ $^ -o $@
	@echo 'Finished building target: $@'
	@echo ''

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)

