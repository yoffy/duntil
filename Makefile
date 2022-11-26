TARGET = duntil
CXXFLAGS ?= -Os
CXXFLAGS += -Wall -Wextra

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o
