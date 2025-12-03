TARGET   = bin/dbview
SRC      = $(wildcard src/*.c)
OBJ      = $(SRC:src/%.c=obj/%.o)
CFLAGS   = -Iinclude -Wall -Wextra

# This makes "make" do the full test sequence every time
all: test

# Full automated workflow — this is what runs when you just type "make"
test: clean $(TARGET)
	@echo "========================================"
	@echo "Creating new database..."
	./$(TARGET) -f mynewdb.db -n
	@echo "Adding Timmy..."
	./$(TARGET) -f mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"
	@echo "Adding Alice..."
	./$(TARGET) -f mynewdb.db -a "Alice Wonder,456 Fairy Tale Rd,160"
	@echo "Adding Bob..."
	./$(TARGET) -f mynewdb.db -a "Bob Builder,789 Construction Site,200"
	@echo "========================================"
	@echo "Final database content:"
	./$(TARGET) -f mynewdb.db -l
	@echo "========================================"
	@echo "All done! Database ready with 3 employees."

$(TARGET): $(OBJ)
	gcc -o $@ $^

obj/%.o: src/%.c | obj bin
	gcc $(CFLAGS) -c $< -o $@

obj bin:
	mkdir -p $@


clean:
	@echo "Cleaning everything..."
	rm -rf obj bin *.db 2>/dev/null || true
	@echo "Clean complete. Directories removed."

# If you ever want just a normal build (not the full test), use: make build
build: $(TARGET)

.PHONY: all test build clean