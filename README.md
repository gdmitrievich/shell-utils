<h1 align="center">Shell Utils</h1>

<h3 align="center">Implementation of cat and grep shell utilities aimed to consolidate structured programming</h2>

<h2>Table of Contents</h2>

- [Features](#features)
- [Tech Stack](#tech-stack)
- [Utils Description](#utils-description)
- [Project Experience](#project-experience)
- [License](#license)

<h2 id="features">Features</h2>

- `cat` command supports all flags from the [cat options](#cat-options) table.
- `grep` command supports all flags from the [grep options](#grep-options) table.
- `grep` supports pair combinations like `-vs`, `-rq`.

<h2 id="tech-stack">Tech Stack</h2>

- Languages: C (C11), bash.
- Libraries: Standart C Libraries, regex.h, getopt.h.
- Build System: Makefile.
- Version Control: git (GitFlow).

<h2 id="utils-description">Utils description</h2>

### Usage of cat

`cat [OPTION] [FILE]...`

### [cat options](#cat-options)

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -b (GNU: --number-nonblank) | numbers only non-empty lines |
| 2 | -e implies -v (GNU only: -E the same, but without implying -v) | but also display end-of-line characters as $  |
| 3 | -n (GNU: --number) | number all output lines |
| 4 | -s (GNU: --squeeze-blank) | squeeze multiple adjacent blank lines |
| 5 | -t implies -v (GNU: -T the same, but without implying -v) | but also display tabs as ^I  |

### Usage of grep

`grep [options] template [file_name]`

### [grep options](#grep-options)

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -f file | Take regexes from a file. |
| 10 | -o | Output the matched parts of a matching line. |

<h2 id="project-experience">Project Experience</h2>

In this project I learned `cat` & `grep` Shell utilities, regular expressions and consolidated structured programming.

I practised:
- Reading command line arguments.
- Project building with Makefile.
- Version Controling with git & GitFlow workflow.
- Clean code principles and practices:
	- SRP: common modules were reused between the utilities & stored in `common` dir.
	- Clear separation of concerns through modularization: cmd_args_data, cmd_args_reader, search, output etc.

The code was written according to [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

<h2 id="license">License</h2>

MIT © [Gleb Dmitrievich](https://github.com/gdmitrievich)
