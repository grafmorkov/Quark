# Quark

**Quark** is a minimalist compiled programming language, currently in the early stages of development.

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

## Tasks

| Stage               | Status        | Description |
|--------------------|--------------|------------|
| Lexical Analysis    | ✅ Completed | Tokenization of input source code |
| Syntax Analysis     | ✅ Completed | AST construction via parsing |
| Semantic Analysis   | ✅ Completed  | Type checking and scope resolution |
| Optimization        | 📝 Planned | Intermediate code optimization |
| Code Generation     | 📝 Planned | Target code emission |

---
## The Demo version of the syntax:
```qk
// This is a comment
/*
    This is another comment
*/
func main() void {
    var x: opt int; // can be none(null)
    var y: int; // basic variable, cannot be none(null)
}    
```

## Contributing

If you want to help develop Quark, feel free to open issues, submit pull requests, or suggest features. Any contributions are welcome!

---

## License

This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.html)**
