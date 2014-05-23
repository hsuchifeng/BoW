CC	=	c++
INCL	=	-I../include  `pkg-config --cflags opencv libpq protobuf`
LIBS	= 	`pkg-config --libs opencv libpq protobuf` 
TARGET_PATH	= ../bin
EXDATA  =  exData.cpp imgret.pb.cc
HS      =  ColorFilter.cpp ColorHist.cpp cFilter.cpp matchBaseColor.cpp
BIN	=  calcSift featsKmeans  server client genData
COMMAND = $(CC) $@.cpp $(EXDATA) $(HS) $(INCL) $(LIBS)  -o $(TARGET_PATH)/$@
all: calcSift featsKmeans server client
calcSift:
	$(COMMAND)
featsKmeans:
	$(COMMAND)
server:
	$(COMMAND)
client:
	$(COMMAND)
genData:
	$(COMMAND)
clean:
	rm $(TARGET_PATH)/$(BIN)
