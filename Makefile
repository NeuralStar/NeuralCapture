NAME = NeuralCapture
COMPILER = g++
FLAGS = -Iincludes -Ilibs -Ilibs/SFML-2.5.1/include -Ilibs/opencv/include/opencv4 -g3 -std=c++11 

LINKS	= -Llibs -lunicorn  -lSDL2 \
			-L libs/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system \
			-L libs/opencv/lib -lopencv_core -lopencv_objdetect -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs

SRC		= training/Assets.cpp training/logic.cpp training/picker.cpp \
			capture/debug.cpp capture/filter.cpp capture/init.cpp capture/output.cpp capture/record.cpp capture/network.cpp capture/tracker.cpp \
			core/Config.cpp core/launch.cpp core/prepare.cpp main.cpp
OBJ		= $(SRC:.cpp=.o)

a: all
all: $(NAME)

$(NAME): $(OBJ)
	$(COMPILER) $(FLAGS) $(OBJ) -o $@ $(LINKS)

%.o: %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

c: clean
clean:
	rm -f $(OBJ)

fc: fclean
fclean: clean
	rm -f $(NAME)

re: remake
remake: fclean all

.DEFAULT_GOAL = all
.PHONY: all clean fclean remake fix_libpath

# export LD_LIBRARY_PATH=$PWD/libs:$PWD/libs/SFML-2.5.1/lib:$PWD/libs/opencv/lib:$LD_LIBRARY_PATH
