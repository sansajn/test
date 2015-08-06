// ukazka zapicu, citania a iterovania databazou
#include <string>
#include <iostream>
#include <cassert>
#include <leveldb/db.h>

using std::string;
using std::cout;

int main(int argc, char * argv[]) 
{
	leveldb::DB * db = nullptr;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "out/test.db", &db);
	assert(status.ok());

	// write
	leveldb::WriteOptions wopts;
	db->Put(wopts, "Patric", "Jane");
	db->Put(wopts, "Teresa", "Lisbon");
	db->Put(wopts, "Kimball", "Cho");
	db->Put(wopts, "Wayne", "Rigsby");
	db->Put(wopts, "Grace", "Van Pelt");

	// read
	std::string result;
	status = db->Get(leveldb::ReadOptions{}, "Kimball", &result);
	if (status.ok())
		cout << "key:Kimball, value:" << result << "\n";
	else
		cout << "key:Kimballl not found\n";

	// iteration
	leveldb::Iterator * it = db->NewIterator(leveldb::ReadOptions{});
	for (it->SeekToFirst(); it->Valid(); it->Next())
		cout << it->key().ToString() << ":" << it->value().ToString() << "\n";
	assert(it->status().ok() && "oops, something goes wrong");
	delete it;

	delete db;  // close database

	return 0;
}
