# ZMQ test

Pred kompiláciou ukážok z root adresára je potrebné nainštalovať balíček `libzmq-dev` príkazom

```bash
sudo apt install libzmq-dev
```

> testované v *Ubuntu 16.04* až *20.04*

Pre ukážky z adresára `zmqpp/` je potrebné nainštalovať balíček `libzmqpp-dev`, pre ukážky z adresára 'azmq/` je potrebné nainštalovať knižnicu boost asio.


## publisher/subscriber

Dvojica soketou publisher/subscriber umožnuje vytvoriť spojenie typu 1:N, kde jeden publisher produkuje správy a N subscriberov konzumuje správy.

> prvý v dvojici je soket na ktorý sa volá `bind` a druhý je soket na ktorý sa volá `connect`

Pozri ukážky `publisher_mon.cpp` a `subscriber_mon.cpp`.


## pull/push

Dvojica soketou pull/push umožnuje vytvoriť posjenie v ktorom push soket umožnuje zaslať správu pull soketu.

Pozri ukážky `pull_mon.cpp` a `push_mon.cpp`.


## router/dealer

Dvojica soketou router/dealer umožnuje vytvoriť neblokovanie (asynchrónne) spojenie v ktorom router čaká na otázku od dealeru na ktorú mu odpovie (ask, answer typ spojenia). 

Pozri ukážky `router_mon.cpp` a `dealer_mon.cpp`.


## notes


zmq.h a zmq.hpp sú štandardnou súčasťou libzmq knižnice

niektore C sample používajú knižnicu czmq, inštaláciu realizuje príkaz

	$ sudo apt-get install libczmq-dev 

C++ binding knižnica je v repozitároch ako

	libzmqpp-dev

