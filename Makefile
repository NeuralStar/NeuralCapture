CC		= g++
CFLAGS	= -I includes -I libs -g3 -I libs/SFML-2.5.1/include
LDFLAGS = -L libs -l unicorn -L libs/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system
SRC		= training/Assets.cpp training/logic.cpp training/picker.cpp capture/debug.cpp capture/filter.cpp capture/init.cpp capture/output.cpp capture/record.cpp capture/network.cpp core/Config.cpp core/launch.cpp core/prepare.cpp main.cpp
OBJ		= $(SRC:.cpp=.o)
TARGET	= NeuralCapture

all: $(TARGET)
a: all

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

c: clean
clean:
	rm -f $(OBJ)

fc: fclean
fclean: clean
	rm -f $(TARGET)

re: remake
remake: fclean all

# export LD_LIBRARY_PATH=$PWD/libs:$PWD/libs/SFML-2.5.1/lib:$LD_LIBRARY_PATH

.PHONY: all clean fclean remake fix_libpath