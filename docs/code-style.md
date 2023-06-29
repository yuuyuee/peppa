# Peppa code style

## Macros

```c
#define PeModule_NAME
#define Pe_NAME
#define _Pe_NAME  // used to other macros only
```

## Types and Enumerates

```c
struct Pe_Type {} Pe_Type;
struct PeModule_Type {} PeModule_Type;

// enum Pe_Type {
//   Pe_Type_NAME
// };

// enum PeModule_Type {
//   PeModule_Type_NAME
// };

enum {
  PeModule_NAME
};

enum {
  Pe_NAME
};
```

## Constants

```c
static const int kName = 1;
static const int kModuleName = 2;
```

## Identifiers

```c
Pe_Type type;
```

## Functions

```c
void Pe_doSomething();
void PeModule_doSomething();
```
