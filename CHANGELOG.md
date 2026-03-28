# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0] - 2026-03-28

### Added

- CMake install rule to install the `ademma` binary to your system (Windows not supported yet).
- Allow arbitrary calculation input with sub-expressions within `(`, `)` and powered sub-expressions.
- Allow explicit multiplication `*` between factors.
- `--option-exists` CLI option.

### Changed

- Cached file filenames for LaTeX integration have special characters sanitized to allow better compatibility in the future, especially with smaller, lesser-known operating systems like Windows (still not supported for LaTeX integration yet). Old cache files will be ignored.

## [0.2.1] - 2026-03-09

### Added

- C-Motivic implementation (including LaTeX integration).

## [0.2.0] - 2026-03-07

### Added

- LaTeX integration (Linux only; maybe MacOS/OSX works too).

### Fixed

- Output `\rho` and `\tau` factors are now LaTeX compliant with their neighbors and have spaces after them if there is a factor following them (for readability).
- The TUI properly handles whitespace in inputs (previously whitespace would delimit TUI input, which was bad news).

## [0.1.1] - 2026-03-03

### Added

- Support for polynomials as calculation-input with whitespace allowed between factors and between terms and '+'.
- `--no-loop-tui` CLI option.

### Changed

- Tui loops by default (unless all required specifications are given via CLI).

## [0.1.0] - 2026-03-02

### Added

- Command Line Interface (CLI).

## [0.0.2] - 2026-02-28

### Added

- Testing for various known Classical and R-Motivic cases.
- Tui start option for choosing between R-Motivic and Classical admissification calculations.
- R-Motivic adem structures and admissify monomial function.

## [0.0.1] - 2026-02-22

### Added

- Basic tui for running classical adem monomial admissification calculations.
- Classical adem structures and admissify monomial function.

