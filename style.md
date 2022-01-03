## Naming

Title_Case for types, interfaces, enum variants
snake_case for vars and funcs
SCREAMING_SNAKE_CASE for constants and static variables

snake_case for namespaces
snake_case for c, cpp, and header files

similar to Rust style guide 
https://doc.rust-lang.org/1.0.0/style/style/naming/README.html
except Snake_Title_Case where TitleCase is used

K&R style braces (e.g. opening curly brace is on line below if statement)

Try to format member declarations "spreadsheet" style e.g.:
```
Metrics        metrics;
ImGuiIO*	   ig_io;
MetricsGuiPlot plot;

font_roboto_mono =    LoadFontEx("resources/fonts/RobotoMono-Regular.ttf",   28, nullptr, 255);
font_roboto_mono_sm = LoadFontEx("resources/fonts/RobotoMono-Regular.ttf",   12, nullptr, 255);
font =                LoadFontEx("resources/fonts/Merriweather-Regular.ttf", 14, nullptr, 255);
```
The types and names are in nice columns, hence spreadsheet style
Unfortunately auto-format often ruins this :(

Place * and & with the type, and not the variable
Good: `ImGuiIO* ig_io;`
Bad:  `ImGuiIO *ig_io;`

## Guidelines

- Avoid using polymorphism, clarity of simple direct function calls
  is preferred. (This makes following code's control flow and data flow MUCH
  simpler).
- Break long functions into short ones
- Small classes (with small interfaces) are preferred over large ones
  (Note: by interface I mean the public members of a class; i.e. a simple
  POD struct can be an interface)
- Only group data with a class that NEEDS to be grouped with it, prefer 
  struct members over many variable members.
- Avoid mutating class state as much as possible, it should be obvious by
  a function's name that class state is being modified. Functions that
  modify state should be short. Use `const` everywhere you can to reduce 
  accidental side effects.
- Use smart pointers where it is possible to do so.

- Use `#pragma once` as header guard (GCC, clang, and msvc all support it) 

- Use TraceLog whenever possible over printf and cout
