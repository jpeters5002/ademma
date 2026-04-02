# Ademma
ADEM Mathematical Analyzer (ademma) is a computational tool to write arbitrary Steenrod algebra polynomials in admissible form. Ademma currently supports Classical (i.e., singular cohomology), R-Motivic, and C-Motivic Steenrod algebras over the prime 2.

## Binaries
Find the latest Windows and Linux release binaries at [Latest Release](https://github.com/jpeters5002/ademma/releases/latest).

## Dependencies
Ademma has no library dependencies other than the c++ standard library.

## Compiling
Ademma is around 100 times faster in Release mode than it is in Debug mode (mainly due to std::vector), so the default build type is set to Release.

If the test binary is desired, include the option `-DBUILD_TESTING=True` when running cmake.

### Windows (instructions untested)
Acquire CMake from [Download Latest](https://cmake.org/download/#latest).

Acquire Visual Studio from [Download Latest](https://visualstudio.microsoft.com/).

#### CMake option A - Terminal
Run the following command when the working directory (of a terminal) is in the root of ademma:
```
mkdir build && cd build && cmake ..
```

#### CMake option B - GUI
NOTE: There is no need to do this step if you did the 'CMake option A - Terminal' step.

Open a file explorer and navigate to the `ademma` repo on your machine. Add a folder in ademma called `build`.

Open the CMake GUI application and set the 'Where is the source code:' to be `ademma` and set the 'Where to build the binaries:' to be `ademma/build`.

Click 'Configure'. Leave any popup options default or blank. Click 'Generate'.

#### Compile with Visual Studio

If Visual Studio has not been set up for building c++ projects before, use the Visual Studio Installer to add the 'Desktop development with C++' workload.

The CMake step (either option) should have created a `.vcxproj` file within the `ademma/build` folder. Open this with Visual Studio by selecting 'File' then 'Open Project'.

Within the 'Solution Explorer' window of Visual Studio there should be an entry titled 'ademma'. Right click that and select 'Set as startup project'. Right click it again and select 'Build'.

Now within the `ademma/build/Release` folder there should be `ademma.exe` which can be run in a terminal.

### Linux
Acquire CMake, GNU Make, and the c++ compiler of your choice (these steps show GCC). System specific instructions on aquiring build tools are in their own sections below.

Run the following command when the working directory is in the root of ademma:
```
mkdir build && cd build && cmake .. && make
```

That will produce an executable file named `ademma` in the build folder that the prior command created and changed directories to.

#### Acquire build tools - Debian based
```
sudo apt install build-essential cmake
```

#### Acquire build tools - Arch based
```
sudo pacman -S cmake make gcc
```

### Other
Ademma should adhere to the c++11 standard, so as long as your machine has cmake and a c++11 or later compiler, it should be possible to compile using standard cmake generation and c++ compiling procedures for your system.

## Running
With an executable form of ademma on your machine in the working directory of your terminal, on Windows run
```
./ademma.exe --help
```
and on Linux run
```
./ademma --help
```
or run without `--help` for a Terminal User Interface (TUI) experience.

See `ademma/latex_integration/README.md` for details on LaTeX integration (currently only Linux supported but MacOS/OSX may work too).

## Planned Future Work
- Web implementation.
- Supporting primes other than 2.
- And more!
- Request your own features to be added to ademma [here](https://github.com/jpeters5002/ademma/issues).

## Credits

Lead Designer and Programmer
  * Brian Billman

Math Wizard
  * Joshua A. Peterson, PhD

