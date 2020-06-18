"""problem: given ['cat', 'cats', 'dog', 'catsdog'] set pf words find ones which are concatenation of other words from set
solution: bruteforce with caching with O(n*m^2) complexity

see [yb](https://www.youtube.com/watch?v=QGVCnjXmrNg) for more details"""

class Solution(object):
	def findAllConcatenatedWorlds(self, words):
		wordDict = set(words)
		cache = {}
		return [word for word in words if self._canForm(word, wordDict, cache)]

	def _canForm(self, word, wordDict, cache):
		if word in cache:
			return cache[word]
		for index in range(1, len(word)):
			prefix = word[:index]
			suffix = word[index:]
			if prefix in wordDict:
				if suffix in wordDict or self._canForm(suffix, wordDict, cache):
					return True
		return False

		
input = ['cat', 'cats', 'dog', 'catsdog']
print(Solution().findAllConcatenatedWorlds(input))
# ['catsdog']
