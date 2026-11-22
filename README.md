# Richard Burns Rally

A matching decompilation of Richard Burns Rally for Windows x86. The goal is
C++ source that rebuilds the selected executable byte for byte, followed by a
native port.

This repository holds no game data. You must supply your own disc image.

## Build

You need Git, Make, Python 3.11 or newer, bsdtar, unshield, Homebrew `llvm`
and `lld`, and Docker with working x86 emulation. The tested host is Apple
Silicon macOS with OrbStack.

To set up the tools on macOS, run these commands. Start Docker first:

```sh
xcode-select --install
brew install llvm lld unshield
git clone https://github.com/shohamc1/rbr.git
cd rbr
make setup
```

To extract your disc image and check the recovered functions, run:

```sh
make extract ISO='/absolute/path/to/disc.iso'
make test check
```

Extraction needs about 5 GB plus the ISO. Game files stay under ignored
`original/`; build outputs stay under `build/`.

`make check` compiles every source with MSVC 7.1 (13.10.3077), links
`build/RichardBurnsRally.exe`, and compares it with the original through
[reccmp](https://github.com/isledecomp/reccmp). The linked executable is a
comparison vehicle: functions without source are stubs, so it doesn't run.

## Contribute

Read [AGENTS.md](AGENTS.md) for the matching loop and source rules.
