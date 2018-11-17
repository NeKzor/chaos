BINARY=chaos.so
STEAM=/home/nekz/.steam/steamapps/common/
SDIR=src/
ODIR=obj/

SRCS=$(wildcard $(SDIR)*.cpp)
SRCS+=$(wildcard $(SDIR)Modules/*.cpp)
SRCS+=$(wildcard $(SDIR)Utils/*.cpp)
OBJS=$(patsubst $(SDIR)%.cpp, $(ODIR)%.o, $(SRCS))

CC=g++
STFU=-Wno-unused-function -Wno-unused-variable -Wno-parentheses -Wno-unknown-pragmas
CFLAGS=-std=c++17 -m32 -fPIC -static-libstdc++ -shared -Wall $(STFU) -I$(SDIR)
PRINT=echo
EXPORT=cp -fu
COPY=cp -n
CREATE=mkdir -p
REMOVE=rm -rf

all: pre chaos post

chaos: $(OBJS)
	@$(CC) $(CFLAGS) -o $(BINARY) $^

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp
	@$(PRINT) $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@$(REMOVE) $(ODIR)chaos.o $(BINARY)

clean-all:
	@$(REMOVE)$(OBJS) $(BINARY)

pre:
	@$(CREATE) $(ODIR)
	@$(CREATE) $(ODIR)Modules/
	@$(CREATE) $(ODIR)Utils/

post:
	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal 2/$(BINARY)"
