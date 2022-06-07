#pragma once
#include <QObject>
#include <iostream>

class foo : public QObject
{
	Q_OBJECT

public:
	foo() {}
	~foo() {}

public slots:
	void call() {std::cout << "foo::call()" << std::endl;}
};
