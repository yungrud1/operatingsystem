# Worksheet 2 - Operating System Development

## Project Structure

You now have **two separate implementations**:

### **worksheet2_part1/** - Basic Framebuffer I/O
- **What it does**: Static display with colored text and C function testing
- **Features**: VGA framebuffer, cursor positioning, mathematical functions
- **Use case**: Demonstrates basic OS functionality and C integration

### **worksheet2_part2/** - Interactive Terminal
- **What it does**: Full interactive terminal with keyboard input
- **Features**: Interrupt handling, keyboard driver, command processing
- **Use case**: Demonstrates advanced OS features and user interaction

---

## How to Use Each Part

### **Part 1: Basic Framebuffer Demo**
```bash
cd worksheet2_part1
make run-curses
```
**What you'll see**: 
- Colored text display
- Mathematical function results
- Cursor positioning examples
- Static output (no keyboard input)

### **Part 2: Interactive Terminal**
```bash
cd worksheet2_part2  
make run-curses
```
**What you can do**:
- Type commands interactively
- Use: `help`, `version`, `echo [text]`, `clear`
- Real-time keyboard input
- Full terminal experience

---

## Quick Test Commands

### Test Part 1 (Static Display)
```bash
cd worksheet2_part1
make clean && make all && make run-curses
```

### Test Part 2 (Interactive Terminal)  
```bash
cd worksheet2_part2
make clean && make all && make run-curses
```

### To quit QEMU from either part:
1. Open new terminal
2. `telnet localhost 45454`
3. Type: `quit`

---

## What Each Part Demonstrates

| Feature | Part 1 | Part 2 |
|---------|--------|--------|
| **Framebuffer I/O** | Yes | Yes |
| **C Functions** | Yes | Yes |
| **Color Display** | Yes | Yes |
| **Interrupt System** | No | Yes |
| **Keyboard Input** | No | Yes |
| **Interactive Terminal** | No | Yes |
| **Command Processing** | No | Yes |
| **Real-time Input** | No | Yes |

---

## Recommended Testing Order

1. **Start with Part 1** to see the basic framebuffer functionality
2. **Then try Part 2** to experience the interactive features
3. **Compare both** to understand the progression

---

## Troubleshooting

### If nothing appears:
- Make sure you're using `make run-curses` (not just `make run`)
- Try `pkill qemu` first to kill any existing instances

### If keyboard doesn't work in Part 2:
- Rebuild: `make clean && make all`
- Ensure you're in the Part 2 directory
- Check that interrupts are enabled

### If build fails:
- Check you're in the correct directory
- Try: `make clean` first
- Ensure all dependencies are installed

---

## Success Indicators

### Part 1 Success:
- Colorful text display
- Mathematical results shown
- Multiple cursor positions
- "All tests completed successfully!" message

### Part 2 Success:
- Boot to prompt (`> `)
- Characters appear as you type
- Commands work (`help`, `echo`, etc.)
- Screen responds immediately to input

**Both parts demonstrate different aspects of operating system development!**
