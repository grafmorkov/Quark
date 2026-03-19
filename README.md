# Quark

**Quark** is a minimalist compiled programming language, currently in the early stages of development.

Right now, it only supports **lexical analysis**. In future updates, I plan to add **parsing** and **AST (Abstract Syntax Tree)** generation.

---

## Getting Started

To build Quark from source:

```bash
# Clone the repository
git clone https://github.com/grafmorkov/Quark.git
cd Quark

# Create and enter the build directory
mkdir build
cd build

# Generate build files
cmake ..

# Compile the project
cmake --build .
```

---

## Running Quark

You can run the compiler on a `.qk` file like this:

```bash
./quark path/to/file.qk
```

> **Note:** At this point, Quark only **tokenizes the input**. Syntax checking and AST support will be added in upcoming versions.

---

## Contributing

If you want to help develop Quark, feel free to open issues, submit pull requests, or suggest features. Any contributions are welcome!

---

## License

This project is licensed under the **[GPL-3.0 License]([https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html)
