C_FILES_CLIENT = client.c

C_FILES_SERVER = server.c

C_FLAGS = -g -Wall -Wextra -Werror

CLIENT = client
SERVER = server

.PHONY: all clean fclean re
.DEFAULT: all

all: $(CLIENT) $(SERVER)

$(CLIENT): $(C_FILES_CLIENT) | build
	cc $(C_FLAGS) $^ -o client -L ./libft -lft

$(SERVER): $(C_FILES_SERVER) | build
	cc $(C_FLAGS) $^ -o server -L ./libft -lft

build:
	$(MAKE) -C libft

clean:
	$(MAKE) -C libft clean

fclean: clean
	-rm -f client
	-rm -f server 
	$(MAKE) -C libft fclean

re: fclean all
