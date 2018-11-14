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
EXPORT=cp -fu
COPY=cp -n
CREATE=mkdir -p

all: pre chaos post

chaos: $(OBJS)
	@$(CC) $(CFLAGS) -o $(BINARY) $^

$(ODIR)%.o: $(SDIR)%.cpp $(SDIR)%.hpp
	@echo $@
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(ODIR)chaos.o $(BINARY)

clean-all:
	@rm -rf $(OBJS) $(BINARY)

pre:
	@$(CREATE) $(ODIR)
	@$(CREATE) $(ODIR)Modules/
	@$(CREATE) $(ODIR)Utils/

post:
	@$(CREATE) "$(STEAM)Portal 2/portal2/addons"
	@$(COPY) "addons/chaos.vdf" "$(STEAM)Portal 2/portal2/addons"
	@$(EXPORT) "$(BINARY)" "$(STEAM)Portal 2/portal2/addons/$(BINARY)"
