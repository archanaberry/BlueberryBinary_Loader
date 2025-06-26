# BlueberryBinary_Loader
My loader BEFB (Berry Executable File Binary) for UBerryNix OS, dont fork it (⁠´⁠∩⁠｡⁠•⁠ ⁠ᵕ⁠ ⁠•⁠｡⁠∩⁠`⁠)

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
