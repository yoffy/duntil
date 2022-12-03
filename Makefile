TARGET = duntil
CXXFLAGS ?= -Os
CXXFLAGS += -Wall -Wextra
LDFLAGS ?= -static-libstdc++ -static-libgcc

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o
