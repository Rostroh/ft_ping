NAME = ft_ping

INC = ft_ping.h
SRC = checksum.c \
	comm.c \
	ft_ping.c \
	ft_wait.c \
	ip.c \
	main.c \
	math.c \
      	parser.c \
	payload.c \
	socket.c \
	stat.c \
	time.c \
	tools.c

INC_DIR = ./include
SRC_DIR = ./srcs
OBJ_DIR = ./obj

INCS = $(INC:%=$(INC_DIR)/%)
SRCS = $(SRC:%=$(SRC_DIR)/%)
OBJS = $(SRC:%.c=$(OBJ_DIR)/%.o)

CC = gcc
WFLG = -Wall -Werror -Wextra
INC_FLG = -I $(INC_DIR)

FLGS = $(INC_FLG) \
	#$(WFLG) \


#------------------------------------------------------------#

all:
	@$(MAKE) $(NAME)


$(NAME): $(OBJS)
	echo "ICI ?"
	$(CC) $(FLGS) $^ -o $@ -lm

$(OBJS): $(INCS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	echo "MDR\n"
	$(CC) $(FLGS) -o $@ -lm -c -fPIC $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	@$(MAKE) all

print-%:
	@echo $*=$($*)
