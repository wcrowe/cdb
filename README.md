# This is a readme file
# cdb

```bash
cd ~/cdb
rm -rf build mynewdb.db
cmake -B build -S .
cmake --build build --target test
```

| What you want                        | Command                                           |
|--------------------------------------|---------------------------------------------------|
| Build with debug info (default)      | `cmake --build build`                             |
| Build WITHOUT debug info             | `cmake --build build --config Release`            |
| Run test (always works)              | `cmake --build build --target test`               |
| Full clean                           | `cmake --build build --target clean-all`          |
| Nuclear clean                        | `rm -rf build mynewdb.db`                         |