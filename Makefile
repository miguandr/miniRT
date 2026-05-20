# Name
NAME = miniRT

# Compiler and Flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# OS detection
OS := $(shell uname -s)

# Minilibx
MLX_DIR = minilibx/
MLX_NAME = libmlx.a
MLX = $(MLX_DIR)$(MLX_NAME)
MLX_REPO = https://github.com/42Paris/minilibx-linux.git

ifeq ($(OS), Darwin)
	X11_INC = -I/opt/X11/include
	X11_LIB = -L/opt/X11/lib
	LIBS = $(X11_LIB) -lXext -lX11 -lm -L $(MLX_DIR) -lmlx
else
	X11_INC =
	X11_LIB =
	LIBS = -lXext -lX11 -lm -L $(MLX_DIR) -lmlx
endif

# Libft
LIBFT_DIR = libft/
LIBFT_NAME = libft.a
LIBFT = $(LIBFT_DIR)$(LIBFT_NAME)

# Includes
INC = -I ./includes/ \
      -I ./libft/ \
      -I $(MLX_DIR) \
      $(X11_INC)

# Source files
SRC_DIR = sources/
SRC = $(SRC_DIR)/error/error.c \
      $(SRC_DIR)/parser/ambient.c \
      $(SRC_DIR)/parser/camera.c \
      $(SRC_DIR)/parser/cylinder.c \
      $(SRC_DIR)/parser/light.c \
      $(SRC_DIR)/parser/parser.c \
      $(SRC_DIR)/parser/parser_utils.c \
      $(SRC_DIR)/parser/plane.c \
      $(SRC_DIR)/parser/sphere.c \
      $(SRC_DIR)/render/color.c \
      $(SRC_DIR)/render/color_utils.c \
      $(SRC_DIR)/render/events.c \
      $(SRC_DIR)/render/events_utils.c \
      $(SRC_DIR)/render/events_utils_2.c \
      $(SRC_DIR)/render/render_camera.c \
      $(SRC_DIR)/render/render_cylinder.c \
      $(SRC_DIR)/render/render_cylinder_utils.c \
      $(SRC_DIR)/render/render_plane.c \
      $(SRC_DIR)/render/render_sphere.c \
      $(SRC_DIR)/render/render.c \
      $(SRC_DIR)/utils/vector_utils.c \
      $(SRC_DIR)/utils/vector_utils_2.c \
      $(SRC_DIR)/utils/utils.c \
      $(SRC_DIR)/main.c \

# Object files
OBJ_DIR = obj/
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Build rules
all: $(MLX) $(LIBFT) $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(MLX):
	@echo "Making MLX..."
	@if [ ! -d "$(MLX_DIR)" ]; then \
		git clone -q $(MLX_REPO) $(MLX_DIR); \
	fi
	@if [ ! -f "$(MLX)" ]; then \
		$(MAKE) -C $(MLX_DIR) 2>&1 | grep -E "^configure|error" || true; \
	fi

$(LIBFT):
	@echo "Making Libft..."
	@make -s -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	@echo "Compiling MiniRT..."
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(MLX) $(LIBFT) $(INC) $(LIBS)
	@echo "\n📟 MiniRT ready.\n"

clean:
	@echo "Removing .o object files..."
	@rm -rf $(OBJ_DIR)
	@make -s clean -C $(LIBFT_DIR)
	@if [ -d "$(MLX_DIR)" ]; then \
		$(MAKE) -s -C $(MLX_DIR) clean 2>/dev/null; \
	fi
	@echo "💧 Clean done \n"


fclean:			clean
	@echo "Removing MiniRT..."
	@rm -f $(NAME)
	@rm -f $(LIBFT_DIR)$(LIBFT_NAME)
	@if [ -d "$(MLX_DIR)" ]; then \
		rm -rf $(MLX_DIR); \
	fi
	@echo "🧼 Fclean done \n"

re:		fclean all

# Phony targets represent actions not files
.PHONY: all clean fclean re
