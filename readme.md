Input text using the rotor wheel

1. Call "read_string()" to start reading input. 
    This will block the thread/task until the input is "submitted";
2. Rotate the rotor wheel to select a character
3. Press the "EAST" (or "Right") button to "lock" the selected character and move to selecting the next.
4. Press the "SOUTH" (or "Down") button to quickly select "Space" and advance to the selecting the next character.
5. Press the Center button to "submit" the input. (Logically equivalent to pressing "Enter")

4.5. (FUTURE) Press the "WEST" (or "Left") button to move the cursor back one character and edit that.
(FUTURE) Use an easing function on the cycling of characters
(FUTURE) Display a RED | when the INPUT_BUFFER_LEN is reached

In the demonstration, the input currently being entered is displayed in White. Once the input is submitted, the received text will be drawn in Gray to signify that it is "locked in". Then the demo will ask for input again and repeat everything, but the input would continue from where the last displayed text ended.

Known issues:
- If the INPUT_BUFFER_LEN is exceeded, cycling the selected character stops working. "Filling" the string (beyond the buffer_len) happens to work, but it overwrites something in memory! (and that is probably what kills the cycling)