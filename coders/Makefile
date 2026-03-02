NAME = codexion

CC = cc
CFLAGS = -Wall -Werror -Wextra -g -pthread 

# Directory
OBJ_DIR = .obj

# Sources
SRC =	main.c \
		utils.c \
		start.c \
		dongle.c \
		scheduler.c \
		routine.c \
		time.c \
		ft_strtol.c \
		init.c \
		monitor.c

# Objects
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re