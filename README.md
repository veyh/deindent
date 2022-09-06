# deindent

`deindent` is a simple command line utility to remove extra indentation from text.

```
Usage: deindent [options] [file]

If file is omitted, reads from stdin.

Options:

  -V, --version                 output program version
  -h, --help                    output help information
  -t, --tabs <arg>              convert tabs to spaces
```

Example

```bash
deindent << EOF
  This line is indented by 2 spaces. After deindenting, it will have 0.
    This line is indented by 4 spaces. After deindenting, it will have 2.
EOF
```

Output

```
This line is indented by 2 spaces. After deindenting, it will have 0.
  This line is indented by 4 spaces. After deindenting, it will have 2.
```

## Releases

Available [here](https://soupbawx.com/deindent/).
