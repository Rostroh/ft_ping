NAME = ft_ping

INC = ft_ping.h
SRC = main.c \
      parser.c

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
	$(CC) $(FLGS) $^ -o $@

$(OBJS): $(INCS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	echo "MDR\n"
	$(CC) $(FLGS) -o $@ -c -fPIC $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	@$(MAKE) all

print-%:
	@echo $*=$($*)
