# Disable pagination to not have to press enter
set pagination off
# Load the kernel into GDB
file src/main.elf
# TUI (Text User Interface)
layout src
focus cmd
# Connect to st-util
tar extended-remote :4242
# Load the kernel onto the target
load
# Set breakpoints
#break *assert_failed
#break *flextimus_prime_pause_pressed
break *HD44780_PowerOn
# Print backtrace when a breakpoint is hit
# Re-enable pagination now that we are debugging
command 1
backtrace full
set pagination on
end
#command 2
#continue
#end
#command 3
#continue
#end
# Run the kernel
display flextimus_prime
continue
