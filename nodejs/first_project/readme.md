[A first project](https://nodejs.github.io/node-addon-examples/getting-started/first/)

inštalácia závislostí

```bash
sudo npm install -g yo
sudo npm install -g generator-napi-module
```

> prepínač `-g` znamená, že balíčky sa nainštalujú globálne

vytvorenie projektu

```bash
mkdir hello-world
cd hello-world
yo napi-module
```

príkaz `yo` vytvorý adresárovú štruktúru projektu ako

```console
$ tree -L 2
.
├── binding.gyp
├── build
│   ├── binding.Makefile
│   ├── config.gypi
│   ├── hello-world-native.target.mk
│   ├── Makefile
│   ├── node_modules
│   └── Release
├── lib
│   └── binding.js
├── node_modules
│   └── node-addon-api
├── package.json
├── package-lock.json
├── src
│   └── hello_world.cc
└── test
    └── test_binding.js
```

dôležité súbory sú 

- [`package.json`](https://nodejs.github.io/node-addon-examples/getting-started/first/#packagejson)
- [`binding.gyp`](https://nodejs.github.io/node-addon-examples/getting-started/first/#bindinggyp)
- [`lib/binding.js`](https://nodejs.github.io/node-addon-examples/getting-started/first/#libbindingjs)
- [`test/test_binding.js`](https://nodejs.github.io/node-addon-examples/getting-started/first/#testtest_bindingjs)

> **tip**: vo visual studiu code môžem test spustiť keď otvorím `package.json`, potom nad `scripts.test` property vydím *> Debug* na ktorý môžem kliknúť a spustím tak test

Test spustím príkazom

```console
$ npm run test

> hello-world@1.0.0 test /home/ja/devel/test/nodejs/first_project/hello-world
> node --napi-modules ./test/test_binding.js

Tests passed- everything looks OK!
```

s adresára projektu.


