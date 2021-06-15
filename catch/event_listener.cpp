// event listener sample
#include <string>
#include <iostream>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using std::string, std::cout, std::endl;

namespace Catch {

struct custom_listener : TestEventListenerBase {

	using TestEventListenerBase::TestEventListenerBase;

	void testCaseStarting(TestCaseInfo const & info) override {
		TestEventListenerBase::testCaseStarting(info);

		stream << "test '" << info.name << "'\n";
	}

	void sectionStarting(SectionInfo const & info) override {
		TestEventListenerBase::sectionStarting(info);

		stream << "section '" << info.name << "'\n";
	}
};

CATCH_REGISTER_LISTENER(custom_listener)

}  // Catch

TEST_CASE("A") {
	SECTION("A.1") {}
	SECTION("A.2") {}
}

TEST_CASE("B") {

}
