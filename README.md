# Current progress

- Functional interpreter with support for function calls,operator precedence, closures and higher order functions

## Demonstration

https://github.com/RS2007/compiler-for-monkey/assets/83483297/1a03ec70-2408-4fe5-b8c2-09a9aa8e9c26

## Usage

- If you are not using an x86 64 machine, change the `TARGET` value in the
  makefile accordingly

### Compile

```bash
  make compile
```

### Execute

```bash
  ./bin/mc # for repl
  ./bin/mc <path-to-file> # for executing a file
```

## Sample program

### Fibonnaci numbers

```monkey
let fib = fn(x){
  if(x == 0){
      return 1;
  }
  if(x == 1){
      return 1;
  }
  return fib(x-1)+fib(x-2);
};
fib(15);
```

## TODO

- [ ] adding support for strings and arrays
- [ ] adding support for hashmaps/hashtables
- [ ] build a bytecode compiler
