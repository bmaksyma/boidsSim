CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++

# CPPFLAGS = -I include
CPPFLAGS = -I include \
           -I include/ui \
           -I include/platform \
		   -I include/simulation \
           -I include/utils \
           -I include/fonts

CFLAGS = -g -std=gnu99 -O1 -Wall
CXXFLAGS = -g -std=gnu++11 -O1 -Wall
LDFLAGS += -static
LDLIBS += -lrt -lpthread

SRC_DIR := src
SOURCES := $(shell find $(SRC_DIR) -name '*.c' -o -name '*.cpp')

OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)

TARGET_EXE = simulation
TARGET_IP ?= 192.168.223.213
TARGET_DIR ?= /tmp/$(shell whoami)
TARGET_USER ?= root

SSH_OPTIONS = -i /home/biloumak/.ssh/mzapo-root-key
SSH_OPTIONS += -o 'ProxyJump=biloumak@postel.felk.cvut.cz'

ifeq ($(filter %.cpp,$(SOURCES)),)
  LINKER = $(CC)
  LDFLAGS += $(CFLAGS) $(CPPFLAGS)
else
  LINKER = $(CXX)
  LDFLAGS += $(CXXFLAGS) $(CPPFLAGS)
endif

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJECTS)
	$(LINKER) $(LDFLAGS) -L. $^ -o $@ $(LDLIBS)

.PHONY : dep all run copy-executable debug

dep: depend

depend: $(SOURCES) $(wildcard include/**/*.h)
	echo '# autogenerated dependencies' > depend
	ifneq ($(filter %.c,$(SOURCES)),)
		$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M $(filter %.c,$(SOURCES)) >> depend
	endif
	ifneq ($(filter %.cpp,$(SOURCES)),)
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) -w -E -M $(filter %.cpp,$(SOURCES)) >> depend
	endif

clean:
	rm -f *.o $(SRC_DIR)/**/*.o $(SRC_DIR)/*.o *.a $(TARGET_EXE) connect.gdb depend 
copy-executable: $(TARGET_EXE)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) killall gdbserver || true
	ssh $(SSH_OPTIONS) $(TARGET_USER)@$(TARGET_IP) mkdir -p $(TARGET_DIR)
	scp $(SSH_OPTIONS) $(TARGET_EXE) $(TARGET_USER)@$(TARGET_IP):$(TARGET_DIR)/$(TARGET_EXE)

run: copy-executable $(TARGET_EXE)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) $(TARGET_DIR)/$(TARGET_EXE)

ifeq ($(filter -o ProxyJump=,$(SSH_OPTIONS))$(SSH_GDB_TUNNEL_REQUIRED),)
  SSH_GDB_PORT_FORWARD=-L 12345:127.0.0.1:12345
  TARGET_GDB_PORT=127.0.0.1:12345
else
  TARGET_GDB_PORT=$(TARGET_IP):12345
endif

debug: copy-executable $(TARGET_EXE)
	xterm -e ssh $(SSH_OPTIONS) $(SSH_GDB_PORT_FORWARD) -t $(TARGET_USER)@$(TARGET_IP) gdbserver :12345 $(TARGET_DIR)/$(TARGET_EXE) &
	sleep 2
	echo >connect.gdb "target extended-remote $(TARGET_GDB_PORT)"
	echo >>connect.gdb "b main"
	echo >>connect.gdb "c"
	ddd --debugger gdb-multiarch -x connect.gdb $(TARGET_EXE)
docs:
	doxygen Doxyfile
clean-docs:
	rm -rf docs
-include depend
