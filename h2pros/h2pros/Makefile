# GNU Makefile to build examples

INCLUDES = -I../../include
CFLAGS = -m32 $(INCLUDES)

include Makefile.i

OBJECTS = $(SOURCE:.c=.o)

all: $(NAME)

clean:
	rm -f $(OBJECTS) $(NAME)

$(NAME): $(OBJECTS)
	@echo "### Please double check that libad4.so is installed, if linking should fail..."
	$(CC) $(CFLAGS) -o $@ $^ -lad4 -lm -lncurses
