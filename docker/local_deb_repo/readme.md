- do ľubovoľného adresára napr. `~/repo` nakopíruj balíčky

```bash
cp *.deb ~/repo
```

- vytvor zoznam balíčkou príkazom

```bash
dpkg-scanpackages . | gzip -c9 > Packages.gz
```

- spusti web server, príkazom

```bash
python3 -m http.server
```

- ďalej je potrebné pridať repozitár do zoznamu repozitárou v `/etc/apt/sources.list` príkazom

```bash
echo "deb [trusted=yes] http://10.1.1.4/debian ./" | tee -a /etc/apt/sources.list > /dev/null
```

kde 10.1.1.4 je ip adresa počítača s repozitárom

> ak chcem repozitar použiť pri builde docker obrazu, potom adresa nesmie byt 0.0.0.0 ale skutočná IP adresa počítača

[source](https://linuxconfig.org/easy-way-to-create-a-debian-package-and-local-package-repository)
