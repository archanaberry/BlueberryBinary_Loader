# BlueberryBinary_Loader
My loader BEFB (Berry Executable File Binary) for UBerryNix OS, dont fork it (⁠´⁠∩⁠｡⁠•⁠ ⁠ᵕ⁠ ⁠•⁠｡⁠∩⁠`⁠)

Structure
```
MyDaemon.befb
├── [0x0000] BEFB Header
│   ├── Magic: "BEFBxyzA"
│   ├── Version: uint16
│   ├── Segment Count: uint16
│   ├── Offset Segment Table: uint32
│   ├── Offset Icon Table: uint32
│   ├── Offset Manifest: uint32
│   ├── Offset Signature Block: uint32
│   └── Flags: uint16  → compressed/encrypted/secure/debuggable
│
├── [0x0020] Segment Table (N × 32 bytes)
│   ├── Segment Name: char[8]    → ".text", ".data", etc
│   ├── Segment Type: uint8      → enum (1=TEXT, 2=DATA, 3=RODATA, 4=BSS, ...)
│   ├── Flags: uint8             → bitmask: R/W/X
│   ├── Align: uint8
│   ├── Reserved: uint8
│   ├── Offset: uint32           → lokasi di file
│   ├── Size: uint32
│   └── Reserved[12]
│
├── [0x0100+] Segment Data (raw binary)
│   ├── .text      → binary code
│   ├── .data      → static data
│   ├── .rodata    → const data
│   └── others     → bisa juga .tls, .meta, dll
│
├── [opt] Icon Table (N × 24 bytes)
│   ├── Type: uint8 (PNG=1, SVG=2, Animated=3)
│   ├── Resolution: uint16
│   ├── Offset: uint32
│   ├── Size: uint32
│   └── Reserved/Padding
│
├── [opt] Icon Blob Data
│   ├── icon_32.png
│   ├── icon_64.png
│   └── icon_vector.svg
│
├── [opt] `manifest.abbm`
│   ├── Binary-encoded metadata (AppID, version, preload, hash table, ABI info)
│   ├── Optional encryption
│   └── Dapat dibaca via `abrt --inspect`
│
├── [opt] Signature Block
│   ├── Magic: "BERRY_SIG"
│   ├── Block Size
│   ├── Public Key (DER)
│   ├── Digital Signature (ECDSA/SHA256)
│   └── Trust Anchor ID
│
└── [opt] EOF / Checksum
    └── CRC32, SHA256 sum, "EOF0x00"
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
