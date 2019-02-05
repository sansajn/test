### Ukážky

library: vygeneruje statickú knižnicu (add_library)

library_output: statická knižnica zo zmeneným výstupom (LIBRARY_OUTPUT_PATH)

shared_library: zdielatelná knižnica (add_library)

library_install: knižnica umožnujúca inštaláciu

extern_compiled_library: externá knižnica skompilovaná zo zdrojakov

**use_shared**: program používajúci dynamickú knižnicu, skompilujem príkazom

	$ mkdir build && cd build
	$ cmake ..
	$ make

**shared_with_dependency**: program volajúci dynamickú knižnicu s zavislosťou na inej dynamickej knižnici. Ukážku skompilujem príkazom

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```
