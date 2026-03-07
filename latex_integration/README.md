# Ademma LaTeX Integration

Ademma LaTeX integration has only been tested on Linux so far. It's likely that it will work on MacOS/OSX but that is untested. It's doubtful it will work for Windows at this time.

This LaTeX package requires that `-shell-escape` be passed to the LaTeX compiler.

## Setup

Ademma must be installed onto your system so that your system's shell can open Ademma with the command `ademma`. This can mean modifying your system's PATH variable or installing Ademma into a location pointed to by PATH. For Linux and MacOS/OSX this should be as simple as running the following command where your working directory has the `ademma` binary:

```
sudo cp ./ademma /usr/local/bin/
```

Next, the file `ademma.sty`, which is the LaTeX package implementation, needs to go somewhere your LaTeX compiler can find it. This can be within your LaTeX project at the top level or wherever your LaTeX compiler expects packages to be, which will depend on which LaTeX compiler is being used.

## Usage

Include the package in your project by adding `\usepackage{ademma}` where you are adding your other packages.

The ademma LaTeX package defines a few macros, the following of which are useful to users:

```
\ademmaclassical{...} % returns the admissible form of the given calculation-input in the classical setting
\ademmarmotivic{...} % returns the admissible form of the given calculation-input in the rmotivic setting
```

The calculation-input should adhere exactly to the calculation-input specifications when running `ademma --help`.

## Additional Info

All calculations are cached on your machine. For Linux and MacOS/OSX these are stored in the directory `~/.ademma`, which is created if it doesn't exist. The cached files contain the LaTeX compliant result that the ademma process produced. If the cached file exists for a calculation, ademma is not run and the cached file is used. The purpose of this is that some calculations can take a long time and the LaTeX compilation process will be held up if ademma is taking awhile on a calculation.

## Troubleshooting

If a calculation seems wrong or is in a different format than what is expected, then clearing the cache might make sense, especially if you have updated ademma and the update involves some bugfixes. You can clear your LaTeX calculation cache on Linux and MacOS/OSX by running `rm ~/.ademma/*`.

