# Slot Machine Emulator

A C-based emulator for slot machine game mechanics, designed to simulate spins, track winnings, and manage both fair and unfair play databases.

## Features

- **Slot Machine Simulation:** Emulates the core mechanics of a slot machine, including spinning reels, displaying results, and calculating winnings.
- **Database Integration:** Utilizes SQLite databases (`fair.db` and `unfair.db`) to store data and potentially track different modes of play.
- **Configurable Reels:** Multiple reels are "spun" programmatically, and results are loaded into a screen matrix for display.
- **Customizable Weights and Bets:** Reel outcomes can be influenced by bet amounts and custom weights, allowing for experimentation with fairness or odds.

## Getting Started

### Prerequisites

- A C compiler (e.g., `gcc`)
- [SQLite3](https://www.sqlite.org/index.html) development libraries

### Build Instructions

1. Clone the repository:
   ```sh
   git clone https://github.com/xellotath/Slot-Machine-Emulator.git
   ```
2. Navigate to the project directory (e.g., `SlotPrototype1`).
3. Compile the project:
   ```sh
   gcc main.cpp -lsqlite3 -o slotemulator
   ```
4. Run the emulator:
   ```sh
   ./slotemulator
   ```

## Usage

- On launch, the emulator creates (or opens) two SQLite databases: `fair.db` and `unfair.db`.
- The slot machine supports different bet amounts and calculates winnings based on weighted probabilities.
- The main mechanic involves spinning five reels and displaying the result in a 2D array.

## Project Structure

- `SlotPrototype1/main.cpp`: Main application entry point.
- `SlotPrototype1/slotinternals.h`: Contains the core slot machine logic, including the spin mechanics.
- `SlotPrototype1/zrtp.txt`: Numerical data, for configuring probabilities and reel weights.
se.

## Author

[https://github.com/xellotath](https://github.com/xellotath)

## Contributing

Pull requests and suggestions are welcome! Please open an issue to discuss your ideas or report bugs.
