# pico-delaygen

`pico-delaygen` is a glitch delay generator using the Raspberry Pi Pico, compatible with the FPGA-based [chipfail-glitcher](https://github.com/unixb0y/chipfail-glitcher).  
`chipfail-glitcher` is an open-source hardware fault injection framework that uses an FPGA to inject precise voltage or clock glitches into microcontrollers for security testing.
It automates glitch campaigns by sweeping parameters like delay and pulse width to induce faults such as instruction skips or protection bypasses.

---
## 🔌 Getting Started
### 1. Serial Setup
- Connect to the device via UART (**Big Endian**). For UART terminal emulators, this is not standard.
- Default baud rate: **115200**
- It appears as a serial device under `/dev/tty.usb*`.
- [Optional] Perform a Soft Reset using the reset command.

### 2. Basic Workflow
1. Set **pulse width** in clock cycles (default: `8 ns`/cycle) (see section Commands).
2. Set **delay** in clock cycles.
3. Arm the glitcher with the **glitch command**.
4. Wait for **state** to return to 0 (idle).

> 💡 A working example: [chipfail-glitcher.ipynb](https://github.com/unixb0y/chipfail-glitcher/blob/master/jupyter/chipfail-glitcher.ipynb)

---

## 💻 Commands
Alternative commands for hand driven mode.

| Command Name        | Shortcuts         | Code | Status |
|---------------------|------------------|------|--------|
| Soft Reset          | `@`, `r`         | 64   | ✓      |
| Toggle LED          | `A`, `t`         | 65   | ✓      |
| Power Cycle         | `B`              | 66   | ✗      |
| Set Pulse Width     | `C`, `p`         | 67   | ✓      |
| Set Delay           | `D`, `d`         | 68   | ✓      |
| Set Power Pulse     | `E`              | 69   | ✗      |
| Glitch              | `F`, `g`         | 70   | ✓      |
| Read GPIO 0–7       | `G`              | 71   | ✗      |
| Enable Power Cycle  | `H`              | 72   | ✗      |
| Get State of Glitch           | `I`, `s`         | 73   | ✓      |

---

## ⚙️ Configuration Options

Adjust `src/main.h` to modify behavior:

- `#define COMP_HAND` — Use **Little Endian** (for UART terminal emulators).
- `#define DEBUG` — Enable verbose UART output.
- `#define USE_PDND` — Support [Pico Debug’n’Dump PCB](https://pdnd.stacksmashing.net/)
### Default Pinout
- **Trigger (Input):** Pin 9  
- **Glitch Pulse (Output):** Pin 6

Change the Pinout to your liking in `src/main.c`.

---

## 📦 Building
Install and set-up [pico-sdk](https://github.com/raspberrypi/pico-sdk) first.
```bash
PICO_SDK_PATH=/path/to/pico-sdk cmake -S . -B build
cmake --build build
```

## ⚠️ Potential Issues

- UART terminal emulators may not trigger `tud_cdc_connected`, causing the Pico to resrc/main in IDLE state.
- UART terminal emulators typically send characters in **Little Endian** format. To support this, undefine `COMP_HAND` in `src/main.h` (see Configuration Options).(#configuration-options) for details.
