# =============================================================================
# Final, perfect Makefile for dbview — VS Code debugger ready!
# =============================================================================

# Output binary
TARGET   = bin/dbview

# Source and object files
SRC      = $(wildcard src/*.c)
OBJ      = $(SRC:src/%.c=obj/%.o)

# Compiler flags — -g is REQUIRED for debugging!
CFLAGS   = -g -Iinclude -Wall -Wextra -Werror
LDFLAGS  = 

# Default target — build the program
all: $(TARGET)

# Link final executable (keep debug symbols)
$(TARGET): $(OBJ)
	@echo "Linking $@"
	gcc -g -o $@ $^ $(LDFLAGS)

# Compile .c → .o with debug info
obj/%.o: src/%.c | obj bin
	@echo "Compiling $<"
	gcc $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
obj bin:
	@mkdir -p $@

# Clean everything — removes obj/, bin/, and test files
clean:
	@echo "Cleaning everything..."
	rm -rf obj bin *.db 2>/dev/null || true

# Run your full test sequence
test: clean all
	@echo "========================================="
	@echo "Creating new database..."
	./$(TARGET) -f mynewdb.db -n
	@echo "Adding Timmy..."
	./$(TARGET) -f mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"
	@echo "Adding Alice..."
	./$(TARGET) -f mynewdb.db -a "Alice Wonder,456 Fairy Tale Rd,160"
	@echo "========================================="
	@echo "Final list:"
	./$(TARGET) -f mynewdb.db -l
	@echo "========================================="
	@echo "Test complete!"

# Phony targets (not real files)
.PHONY: all clean test