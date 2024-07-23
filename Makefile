################################################################################
###############                     CONFIG                        ##############
################################################################################

NAME := webserv
CC := c++
CFLAGS = -Wextra -Wall -Werror -std=c++98 -MMD -MP $(addprefix -I, $(INC_DIRS))

################################################################################
###############                 PRINT OPTIONS                     ##############
################################################################################

G := \033[32m
X := \033[0m
BO := $(shell tput bold)
LOG := printf "[$(BO)$(G)ⓘ INFO$(X)] %s\n"

################################################################################
###############                  DIRECTORIES                      ##############
################################################################################

OBJ_DIR := _obj
INC_DIRS := . ./include/
SRC_DIRS := ./srcs/

# Tell the Makefile where headers and source files are
vpath %.hpp $(INC_DIRS)
vpath %.cpp $(SRC_DIRS)

################################################################################
###############                  SOURCE FILES                     ##############
################################################################################

SRCS := main.cpp Lexer.cpp EventLogger.cpp Parser.cpp Utils.cpp \
		SocketManager.cpp HttpRequest.cpp HttpResponse.cpp

OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))

################################################################################
########                         COMPILING                      ################
################################################################################

all: $(NAME)

$(NAME): $(OBJS)
	@$(LOG) "Linking object files to $@"
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(LOG) "Compiling $(notdir $@)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(LOG) "Creating object directory."
	@mkdir -p $@

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		$(LOG) "Cleaning $(notdir $(OBJ_DIR))"; \
		rm -rf $(OBJ_DIR); \
	else \
		$(LOG) "No objects to clean."; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		$(LOG) "Cleaning $(notdir $(NAME))"; \
		rm -f $(NAME); \
	else \
		$(LOG) "No library to clean."; \
	fi

re: fclean all

-include $(OBJS:$(OBJ_DIR)/%.o=$(OBJ_DIR)/%.d)

.PHONY: all fclean clean re
