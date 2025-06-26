# BlueberryBinary_Loader
My loader BEFB (Berry Executable File Binary) for UBerryNix OS, dont fork it (⁠´⁠∩⁠｡⁠•⁠ ⁠ᵕ⁠ ⁠•⁠｡⁠∩⁠`⁠)

Structure
```
MyDaemon.befb
[0x0000] BEFB Header
├── Magic: "BEFB" (4 bytes)
├── Version: uint16
├── Segment Count: uint16
├── Offset Segment Table: uint32
├── Offset Icon Table: uint32
├── Offset Manifest: uint32
├── Offset Signature Block: uint32
└── Flags: uint16             ← Bitflags: debuggable, secure, encrypted, compressed

[0x0020] Segment Table (N × 32 bytes)
Each segment entry contains:
├── Name: char[8]            ← e.g., ".text", ".data"
├── Type: uint8              ← ENUM: 1=TEXT, 2=DATA, 3=RODATA, 4=BSS, etc.
├── Flags: uint8             ← R/W/X bitmask
├── Alignment: uint8
├── Reserved: uint8
├── Offset in File: uint32
├── Size in Bytes: uint32
└── Reserved/Padding: [12 bytes]

[0x0100+] Segment Binary Data (berrysegs/)
├── .text                    ← Executable binary code
├── .data                    ← Writable initialized values
├── .rodata                  ← Static const values
├── .bss                     ← Declared size only, no actual data in file
├── .cherryreva              ← Virtual address fixup table
├── .plugin                  ← Plugin binary/module symbols
├── .dbgmap                  ← Optional debug trace section
├── .register                ← Register entry info (.e.g RIP)
└── .hash                    ← Per-segment SHA256 or custom checksum

[optional offset] Icon Table (N × 24 bytes)
Each icon entry:
├── Type: uint8              ← PNG, SVG, Animated
├── Resolution: uint16       ← e.g., 64, 128, 512
├── Offset: uint32
├── Size: uint32
├── Reserved/Padding
v

[optional offset] icons/
├── icon_32.png
├── icon_64.png
├── icon_vector.svg
v

[optional offset] manifest.abbm
├── Binary metadata: AppID, Version, Arch, ABI, Required modules, Segment checksums
├── Supports: Encryption, Preload rules, Execution policies
v

[optional offset] signature.absign
├── Magic: "BERRY_SIG"
├── Block Size
├── Certificate (DER-encoded)
├── Signature (ECDSA of hash table + manifest)
├── Trusted Anchor / Vendor ID
v

[optional offset] .EOF
└── CRC32 / SHA256 / Custom EOF Marker like "EOF0x00"
```

My structure project 
```
├── include/                # Public BEFB headers (.h)
│   └── befb.h
├── src/                    # Core loader implementation
│   ├── main.c              # Entry point: argument parsing
│   ├── loader.c            # befb_load(): read file & segment table
│   ├── parser.c            # Parse segment table, icon table, etc.
│   ├── segment.c           # map_berrysegs(): .text, .data, .bss
│   ├── cherryreva.c        # RVA patcher (CherryReva)
│   ├── plugin.c            # load_plugins(): extensions & symbol resolution
│   ├── dbgmap.c            # init_debugger(): register debug hooks
│   ├── icon.c              # parse_icons(): load icon assets
│   ├── manifest.c          # parse_manifest(): read manifest.abbm
│   ├── hash.c              # verify_hash(): integrity check
│   ├── signature.c         # verify_signature(): authentication
│   ├── util.c              # Utility functions (endian, logging, I/O)
│   └── eof_marker.c        # check_eof_marker(): validate EOF marker
├── tests/                  # Unit tests & sample files
│   ├── sample.befb
│   └── test_loader.c
├── tools/                  # Helper tools for BEFB creation
│   ├── befb_packager.c
│   └── make_befb.sh
├── examples/               # End-to-end usage examples
│   ├── hello_world.abs
│   ├── build.sh
│   └── run.sh
├── docs/                   # Project documentation
│   ├── DESIGN.md
│   ├── USAGE.md
│   └── API.md
├── Makefile                # Build automation script
└── README.md               # Project overview & quick start guide
```
