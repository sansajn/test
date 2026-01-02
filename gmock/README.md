# GMock Samples

# GMock Samples

Two GMock examples demonstrating modern C++ mocking without inheritance.

## What This Repository Contains

### hello.cpp - Basic GMock
- Template-based design without virtual functions
- Basic `EXPECT_CALL()` and `WillOnce()` usage
- Works with both real and mock objects

### real.cpp - Realistic Example
- `UserService` with business logic (validation, data transformation, error handling)
- `MockDataRepository` for external dependencies
- Behavior-focused tests that verify outcomes, not implementation details

## Installing Dependencies

```bash
sudo apt install libgtest-dev libgmock-dev
```

## Building and Running

```bash
mkdir build && cd build
cmake ..
cmake --build .

# Run samples
./hello
./real

# Or use CTest
ctest
```
