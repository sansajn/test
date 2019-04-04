#include <iostream>
#include <string>
#include <boost/algorithm/searching/boyer_moore.hpp>

int main(int argc, char * argv[])
{
	std::string sentence = "It was the best of times, it was the worst of times, it was the age of wisdom, it was the age of foolishness, "
									"it was the epoch of belief, it was the epoch of incredulity, it was the season of Light, it was the season "
									"of Darkness, it was the spring of hope, it was the winter of despair, we had everything before us, we had "
									"nothing before us, we were all going direct to Heaven, we were all going direct the other way—in short, the "
									"period was so far like the present period, that some of its noisiest authorities insisted on its being received, "
									"for good or for evil, in the superlative degree of comparison only.";

	std::string word = "incredulity";
	
	auto searcher = boost::algorithm::make_boyer_moore(word);
	auto wordPosition = searcher(sentence);
	
	if (wordPosition.first != end(sentence))
	{
		std::cout << "The word " << word << " goes from position "
			<< std::distance(begin(sentence), wordPosition.first)
			<< " to position "
			<< std::distance(begin(sentence), wordPosition.second)
			<< std::endl;
	}
	
	return 0;
}
