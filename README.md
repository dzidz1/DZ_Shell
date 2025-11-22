# DZ Shell

**A lightweight Unix shell implementation in C**

DZ Shell is a functional Unix shell built from scratch, demonstrating core concepts like process management (fork/exec), PATH resolution, and environment variable handling.

## Demo

🎥 **[Watch the demo]https://www.youtube.com/watch?v=idEPh_Yheh0**

## Installation

```bash
git clone https://github.com/yourusername/dz-shell.git
cd dz-shell
make
./dz_shell
```

## Features

### Built-in Commands

| Command            | Description                              |
| ------------------ | ---------------------------------------- |
| `cd [path]`        | Change directory (`~`, `-`, `..`)        |
| `pwd`              | Print working directory                  |
| `echo [text]`      | Display text or variables (`$VAR`, `-n`) |
| `env`              | List environment variables               |
| `setenv VAR=value` | Set environment variable                 |
| `unsetenv VAR`     | Remove environment variable              |
| `which [command]`  | Show command location                    |
| `exit` / `quit`    | Exit shell                               |

### External Commands

Executes programs from PATH, absolute paths (`/bin/ls`), or relative paths (`./program`)

## Development

```bash
make test                   # Run all tests
make test_builtins_only    # Test built-ins
make clean                  # Clean build
```

## License

MIT License - see [LICENSE](LICENSE)

---

Built by Akaki
