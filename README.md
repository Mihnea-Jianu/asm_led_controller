# ASM LED Controller

The ASM LED Controller Board is an algorithmic state machine, designed to store
and run a microprogram which controls the states of the 22 LEDs that make up the
on-board, heart-shaped LED array.

## Features

- 22 individually addressable LEDs.
- Microprograms up to 1023 microinstructions long.
- 3 separate clock speeds.
- Fully digital flash interface.

## Schematic Overview

![Microprogram Control Unit](images/control_unit.png)

### Clock Generator

The clock generator is simply a `CD4060BE binary counter`, which is setup to use
its built-in oscillator with the RC setup.

From the frequencies generated at the pins of the chip, I have selected a 
slow one `[Q8]` and a fast one `[Q7]`, and hooked them up to a SP3T slide
switch, for clock speed selection. The third pole of the slide switch is 
connected to GND, equivalent to a paused clock.

The output of this slide switch is the main clock of the circuit.

### Progam Counter

The program counter is another `CD4060BE binary counter`, whose outputs are used
to iterate through the addresses of the memory chips.

**When in RUN mode** 
- The counter is driven up by the pulse of the clock generator.
- The counter is reset when the current instruction's reset bit is pulled high.

**When in FLASH mode**
- The counter is driven up by the flash interface's XCLK.
- The counter is reset by the flash interface's XRST.

### Microprogram Memory

The microprogram memroy consists of 3 concatenated `AS6C1008-55PCN SRAM` chips.
Their addresses are 8 bits wide, so we have a total of 24 bits per address.

The current address of the memory chips is controlled by the program counter,
and the outputs of the memory chips go directly to the LED array.

**When in RUN mode**
- We read the LED configuration stored in memory, at the current address.
- We read the reset bit stored in memory, at the current address.

**When in FLASH mode**
- We write an LED configuration to the memory, at the current address.
- We write the reset bit to the memory, at the current address.

## Flashing

To flash the device, you will need a development board that has 26 usable digital
output pins, like the **Arduino Mega**.

The `asm_led_flasher.ino` sketch, under `software/`, is setup to use pins 22-47
of the Arduino Mega, which must be connected to the flash interface.

**The flash interface consists of 26 digital pins:**

<table>
    <thead> 
        <tr>
            <th>Pin</th> <th>Arduino Mega Pin</th> <th>Type</th> <th>Role</th>
        </tr> 
    </thead>
    <tbody>
        <tr>
            <td>L1 - L22</td>
            <td>26 - 47</td>
            <td>data pin</td>
            <td>load LED configurations into memory</td>
        </tr>
        <tr>
            <td>RST</td>
            <td>25</td>
            <td>data pin</td>
            <td>load reset bit into memory</td>
        </tr>
        <tr>
            <td>XWE</td>
            <td>24</td>
            <td>control pin</td>
            <td>write the values at the data pins into memory</td>
        </tr>
        <tr>
            <td>XRST</td>
            <td>23</td>
            <td>control pin</td>
            <td>reset the current address of the memory</td>
        </tr>
        <tr>
            <td>XCLK</td>
            <td>22</td>
            <td>control pin</td>
            <td>increment the current address of the memory</td>
        </tr>
    </tbody>
</table>

**Set the on-board switches to the following configuration:**

<table>
    <thead> 
        <tr>
            <th>Switch</th> <th>Position</th>
        </tr> 
    </thead>
    <tbody>
        <tr>
            <td>ON/OFF</td>
            <td>ON</td>
        </tr>
        <tr>
            <td>ON/OFF LED</td>
            <td>ON</td>
        </tr>
        <tr>
            <td>RUN/FLASH</td>
            <td>FLASH</td>
        </tr>
        <tr>
            <td>FREQUENCY</td>
            <td>PAUSE</td>
        </tr>
    </tbody>
</table>

Once everything is setup, connect the Arduino Mega to your computer, and upload
the `software/asm_led_flasher.ino` sketch to it.

- When the builtin LED lights up, you have 10 seconds to flip the top RUN/FLASH
switch back to RUN. 

- When the builtin LED lights up again, you can flip the bottom RUN/FLASH switch
back to RUN, disconnect the flash interface from the Arduino Mega, and unpause
the clock.

## Repository Contents

`hardware/`
- KiCad project files (schematic, layout).
- Custom libraries (symbols, footprints, 3D models, library tables).

`software/`
- Arduino IDE sketch, for flashing the board.
