# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Allow arbitrary calculation input with sub-expressions within `(`, `)` and powered sub-expressions.
- Allow explicit multiplication `*` between factors.
- `--option-exists` CLI option.

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

