CC	=	clang++ 
INCL	=	-I../include  `pkg-config --cflags opencv libpq protobuf`
LIBS	= 	`pkg-config --libs opencv libpq protobuf` 
TARGET_PATH	= ../bin
EXDATA  =  exData.cpp imgret.pb.cc
HS      =  ColorFilter.cpp ColorHist.cpp cFilter.cpp matchBaseColor.cpp
BIN	=  calcSift featsKmeans wordHist server client
COMMAND = $(CC) $@.cpp $(EXDATA) $(HS) $(INCL) $(LIBS)  -o $(TARGET_PATH)/$@

first:
	echo "please specific a target"
all:
	echo "building all"
	$(BIN)
test:
	$(COMMAND)
calcSift:
	$(COMMAND)
featsKmeans:
	$(COMMAND)
wordHist:
	$(COMMAND)
server:
	$(COMMAND)	
client:
	$(COMMAND)
clean:
	rm $(TARGET_PATH)/$(BIN)
