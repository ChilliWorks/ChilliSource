CC=/Volumes/xtools/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-g++
CFLAGS=-c -Wall -O3  -I../vc/include
#LDFLAGS=-L../vc/lib -lvcos -lbcm_host -lGLESv2 -lEGL -lvchiq_arm -lpthread -lrt -lm
LDFLAGS=-L../vc/lib -lvcos -lbcm_host -lGLESv2 -lEGL -lvchiq_arm
SOURCES=$(shell find ./ -name *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=helloWorld

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm *.o $(EXECUTABLE)

#gather all files in the engine that should be built
#CS_SOURCEFILES_CHILLISOURCE := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/ChilliSource/' '--extensions' 'cpp,c,cc')
#CS_SOURCEFILES_PLATFORM := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Platform/RPi/' '--extensions' 'cpp,c,cc')
#CS_SOURCEFILES_RENDERING := $(shell 'python' '$(CS_SCRIPT_GETFILESWITHEXTENSIONS)' '--directory' '$(CS_PROJECT_ROOT)/ChilliSource/Source/CSBackend/Rendering/OpenGL/' '--extensions' 'cpp,c,cc')
