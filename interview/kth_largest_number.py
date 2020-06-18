"""problem: found k-th largest number from a given list [5, 7, 2, 3, 4, 1, 6]
solution: partitioning with O(2n) complexity
other possible solution: create a heap and then pop k-th elements from it

see [yb](https://www.youtube.com/watch?v=QGVCnjXmrNg) for more details"""

class Solution(object):
	def findKthLargest(self, arr, k):
		left = 0
		right = len(arr) - 1
		while left <= right:
			pivotIndex = self._partition(arr, left, right)
			if pivotIndex == len(arr) - k:
				return arr[pivotIndex]
			elif pivotIndex > len(arr) - k:
				right = pivotIndex - 1
			else:
				left = pivotIndex + 1
		return -1

	def _partition(self, arr, low, high):
		pivot = arr[high]
		index = low
		for j in range(low, high):
			if arr[j] <= pivot:
				arr[index], arr[j] = arr[j], arr[index]
				index += 1
		arr[index], arr[high] = arr[high], arr[index]
		return index


print(Solution().findKthLargest([5, 7, 2, 3, 4, 1, 6], 3))
# 5
