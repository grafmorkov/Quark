# Quark

**Quark** is a minimalist compiled programming language, that is currently building into **C**.

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
## Requirenment
To build Quark from source you have to install a C compiler. For example,
    - Clang, (The main compiler of Quark)
    - GCC,
    - etc.
If you want to change the compiler, just change this block of code in main:
```cpp
// Build clang
    if (opts.build || opts.run) {
        std::string cmd = "clang out.c -o out";

        if (std::system(cmd.c_str()) != 0) {
            utils::logger::error("Clang build failed\n");
            return 1;
        }
    }
```

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
| Code Generation     | ✅ Planned | Target code emission |
| Optimization        | 📝 Planned | Intermediate code optimization |

---
## The Syntax of the Quark:
```qk
// This is a comment
/*
    This is another comment
*/
func main() void {
    x: int = 10;

    y: int = x + 2;

    if(x > 5){
        x = 0; 
    }
    else{
        x = 1;
    }
}    
```

## Contributing

If you want to help develop Quark, you can just fork the repo, or dm grafmorkov(me) in Discord, Telegram, etc. 

---

## License

This project is licensed under the **[GPL-3.0 License](https://www.gnu.org/licenses/gpl-3.0.html)**
