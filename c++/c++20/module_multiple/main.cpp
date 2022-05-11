/* module interface unit C++20 module sample from https://itnext.io/c-20-modules-complete-guide-ae741ddbae3d

run

```bash
g++ -std=c++20 -fmodules-ts -xc++-system-header iostream
```

before

```bash
scons
```

to compile sample. */

import mod;

int main() {
	func();
	return 0;
}
