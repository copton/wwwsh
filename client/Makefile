
CXX = g++
OBJECTS = base64.o ie.o panic.o wwwsh.o
TARGET = wwwsh.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

clean:
	del *.o
	del $(TARGET) 

%.o: %.cpp
	$(CXX) -c $^	