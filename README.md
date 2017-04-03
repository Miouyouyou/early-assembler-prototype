# Graphical Assembler
I'll like to call it a "Visual" assembler, but that would generate confusion.

## Main goals

My main purpose when building this Graphical Assembler is to be able to use it to build other programs :
* from any Computer, Tablet or Smartphone;
* using Assembly syntaxes as the main language;
* using a node-based graphical interface that allow for quick access, editing, management and re-arrangement of the different building blocks of a program (symbols, procedures, data, sections, ...);
* using a linear text editor to edit the code when deemed more efficient;
* with all the control and freedom that offer an assembler and a program linker;
* using appropriate helpers widgets and interfaces when editing without a keyboard (touch-panel interface, game controller, ...).

In short : A graphical multiplatform assembler and linker.

The program will be first design to generate ARMv7 machine code and build ELF executables for Linux and Android systems, as these are the systems I'll test and use this software on. Once done, others platforms will be added.  
The UI will be designed using OpenGL ES 2.x / 3.x renderers. 

The program will be considered completely useable once :
* it will be rewritten using itself twice. Once with the externally compiled version, once with the manually assembled version.
* developing the rewritten version through a VCS will be entirely possible. This mean easily accepting and including the patches provided through Git.

The program will be considered feature complete when all the planned features will be implemented.

## First PoC

This week

## Non-contractual sketches

### Nodes
![Example nodes](FirstPresentation/Sketches/Nodes.png)

### Procedures
![Example procedure](FirstPresentation/Sketches/Example-procedure-no-wires.png)
![Other procedure potential presentation](FirstPresentation/Sketches/node.png)
![Linear procedure example](FirstPresentation/Sketches/Linearcode.png)

### Sections editors
![Binary sections editor](FirstPresentation/Sketches/Sections.png)
![Example Data Editor](FirstPresentation/Sketches/ExampleDataEditor.png)

## Planned features

### Editing

* Node based, symbolic programming, à la Blender
* Use generated nodes and nodes groups as simple Input/Outputs symbols
* Generate editable procedures entirely composed of calling simple linked procedures, and use them as simple Input/Output symbolic procedures too.
* Editing the assembly code of nodes and text sections through
  * Using a simple basic text editor.  
    When using a nice keyboard and already knowing the code to type.  
    Plans to support external editors, with different assembler syntaxes.
  * Using an assisted interface, touch-screen friendly.  
    When editing from a smart phone, using predetermined lists of mnemonics and potential arguments, completed by soft keyboard inputs.
  * Using a spreadsheet-like "Function" interface :
    * Listing all the mnemonics, categorized, with full descriptions of their uses, along with all the defined procedures in the current project.
    * Providing a specific UI to setup the mnemonic/procedure inputs
 * Export/import the content of assembler nodes to/from text files designed for other assembler programs.  
With the ability to tag nodes with determined filepath for export.
* Generate multiple named views showing only the nodes currently worked on.  
  Multiple views can show the same node multiple times

### Basic assembling functionalities

* Define multiple symbols names for each procedure.
* Define multiple local symbols defining where local procedures can branch to.
* Define local data sections at any point of the procedure.
* Refer to other procedures local sections.
* Provide pre-generated control flow template procedures for basic branching, looping and jump tables.  
  While being able to add custom ones too.
* Makes it slightly easier to branch in the middle of another procedure.
  Potential stack issues are managed by the warned programmer.
* Support of a subset of the Sourceware AS syntax by default.  
  Support for custom syntaxes will be added with the editor import functions.

### Basic data management functionalities

* Define the structure of each data sections.
* Add different types of data through provided widgets.
* Import data from external files.
* Define multiple symbols names starting at the same address, identical to C unions.
* Define complex data structures, by easily defining the size of each component.
* Define static arrays and structured data, using provided and generated data structure definitions.
* Get the address and size of each data symbol, or sections, through specific helpers functions.

### Basic linking functionalities

* Manage and manually arrange the symbols, data and sections addresses and positions in generated binaries.
* Generate raw binaries, ELF binaries. Other binary formats will be added as plugins.
* Manage every part of each executable file format.

### Architecture specifics

* Ability to rewrite the same program for multiple architectures.  
  This will be mainly done by :
  * quickly stripping the code of every node while keeping their metadata;
  * providing a quick architecture switch system.
* Generate architecture/system specific nodes.  
  Some architectures might require helpers procedures that others architectures do not need.  
  These specific nodes will not be kept when switching between different architectures representations.

### Optimizations specifics

* Provide optimized versions of specific groups of assembler nodes, while keeping the non-optimized original nodes intact.  
  A lot of optimizations are only valid for a specific version of a program.  
  However losing the non-optimized hinder the whole development process, due to the code being too specialized. Being able to switch between the two avoid such problems.
* Generates optimized versions of nodes calling a specific procedure that should be inlined.  
  The point is to be able to tag a node as _always inlined_ and automatically generate optimized versions of every node that inline it.

### Multilingual

* Multilingual comments and documentation nodes, with fallbacks.  
  Comments and documentations are only useful when correctly written and understood.  
  It is far better to let people write their documentations in their own language first, and then let them translate it in English if they can, than having a complete documentation written in broken English.
* Multilingual symbols translations.  
  Meaning that the symbol name used by the binary will stay the same while the symbol seen in the interface will be translated to the current user's language.
* Standard UTF-16 support.  
  Top-down/Down-top writing systems support might be done after the first feature-complete release, though.

### Design nodes and metadata nodes

* Use metadata nodes to describe your entire program architecture quickly.
  Because, when developing, understanding the goals and architecture of the program is more useful than understanding what its current instructions do. 

### Verifications

* Test nodes units directly from the interface.  
  A simple unit tests lab, allowing to run _pure_ units directly from the interface.  
  Testing units depending on hidden state might also be added to the test lab when deemed easily possible.
* Add check requirements for arguments, guaranteed automatically or manually explicitly.  
  This will be limited to bound checking at first.  
  Manual guarantees being essential when "automatic" checks cannot be performed.

### Skinning

* Customize the interface look&feel.  
  This is essential ! The interface should be customizable to fit the programmer. 

### Templating

* Generate quick templates allowing to prototype ideas very quickly.

### Feature complete

* Provide this entire program rewritten in assembly, using this same program, when possible.

### Last functionnalities

* Add, share and edit libraries online, through open and documented protocols that anyone can implement.  
  No shady undocumented opaque 'cloud' sharing thingy.  
  Currently thinking about using HTTP and JSON/YAML but any suggestion is welcome. 

### Languages and libraries used

* C (C++ if clearly necessary) until rewrite
* OpenGL ES 2.x / 3.x renderers.  
  While I would like to write it entirely with OpenGL ES 3.x, most Raspberry Pi systems only support OpenGL ES 2.x, and these are one the main targets of such software. Nonwithstanding the good bunch of Android phones using OpenGL ES 2.x only graphical cards.
* Freetype for fonts management
* X11 and KMS support on Linux systems.

### Extensibility

* A plugin system will be put in place when deemed possible, once the program will be useable.

### Assembly languages aimed to be supported, in priority order

* ARMv7 (with Thumb mode !)
* ARMv8
* X86-64 (aka. AMD64)
* MIPS
* LLVM bytecode
* WebAssembly

### License

* Code : MIT License
* Graphical assets : CC0
