NAME			= test
CC				= c++

CFLAGS			= -std=c++98 -Wall -Wextra -Werror -g3 -fsanitize=address

INC_DIR 		= ft_container/
INC_FLAG 		= -I ${INC_DIR}
CFLAGS 			+= ${INC_FLAG}

MAIN_DIR        = test_2/
MAIN_SRC		= main

# NOTE : Add to SRC here
# ------------------------------------------------------ #
SRC = $(addsuffix .cpp, $(addprefix $(MAIN_DIR), $(MAIN_SRC))) \
# ------------------------------------------------------ #

OBJ = $(SRC:cpp=o)

all: $(NAME)

# Colors
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(RED)--------------------------------------------$(DEF_COLOR)"
	@echo "$(RED)|       ft_container compile finished.     |$(DEF_COLOR)"
	@echo "$(RED)--------------------------------------------$(DEF_COLOR)"
	@echo "$(RED)|                                          |$(DEF_COLOR)"
	@echo "$(RED)|                        Have fun!         |$(DEF_COLOR)"
	@echo "$(RED)--------------------------------------------$(DEF_COLOR)"

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
	@echo "$(RED)Compiling... \t$< $(DEF_COLOR)"
# #-----------------------------------------------------------------------

clean:
	@rm -f $(OBJ)
	@echo "$(RED)CPP obj files has been deleted.$(DEF_COLOR)"

fclean:
	@rm -f $(OBJ)
	@rm -f $(NAME)
	@echo "$(RED)archive files has been deleted.$(DEF_COLOR)"

re: fclean all
	@echo "$(RED)Cleaned and rebuilt CPP project.$(DEF_COLOR)"
