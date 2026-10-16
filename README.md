# Richard Burns Rally

A matching decompilation of Richard Burns Rally for Windows x86. The goal is
C/C++ source that rebuilds the selected executable byte for byte, followed
by a native port.

This repository holds no game data. You must supply your own disc image.

## Build

You need Git, Make, Python 3.11 or newer, a C++ compiler, bsdtar, unshield,
and Docker with working x86 emulation. The tested host is Apple Silicon
macOS with OrbStack.

The project uses Clang 23.1.1 and MSVC 13.10.3077. Compiler files and
libraries must match the hashes in [config/toolchain.json](config/toolchain.json)
and [config/msvc71.json](config/msvc71.json). A different release will fail
verification. MSVC runs through Docker and Wine; setup fetches it locally.

On macOS, install the command-line tools and dependencies:

```sh
xcode-select --install
brew install llvm unshield
```

Clone the repository and set up the tools. Start Docker before MSVC setup:

```sh
git clone https://github.com/shohamc1/rbr.git
cd rbr
make setup
make setup-msvc
```

Extract your disc image, then build and check the recovered functions:

```sh
make extract ISO='/absolute/path/to/disc.iso'
make verify-inputs
make build
make test check-overlay
```

Extraction needs about 5 GB plus the ISO. Game files stay under ignored
`original/`; build outputs stay under `build/`.

A successful overlay check prints `PASS intermediate overlay`. It combines
compiled functions with retained reference bytes. It does not produce a
complete source rebuild. `make check-integrated` fails until that build exists.

## Contribute

Read [AGENTS.md](AGENTS.md) for the matching workflow, analysis tools,
compiler details, and contribution rules.
