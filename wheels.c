/*
 * This source file contains functions only,
 * I try to make these functinos pure functions
 * or array handlers.
 *
 * Author: tjytlxwxhyzqfw 
 * Date: 2016-06-22
 */

/*
 * Appay f() to every element in the given array,
 * who's first element is pointed by *first.
 * And If result[] is not NULL, returns of f() will be stored into result[].
 *
 * This function will never change anything releated to its parameters except result[]
 *
 * @param first
 * 	- a pointer pointed to the first element of a given array.
 * 	- its step width is one, which means (unsigned long)(first + 1) - (unsigned long)first == 1;
 *
 * @param size
 * 	- number of elements in the array.
 *
 * @param elementsize
 * 	- size of an element in array.
 *
 * @param f(const int, const void *)
 * 	- @param 1 - index of the element.
 * 	- @param 2 - a pointer to current element.
 *
 * @param result
 *
 * @param ressize
 * 	- size of result[], it will be ignored if result[] is NULL
 *
 * @return
 */
void *foreach(const void *first, const int size, const int elementsize,
		void * (*f)(const int, const void *),
		void *result[], const int ressize) {

	int i = 0;
	const void *current = first;


	if (result) {
		while (i < size && i < ressize) {
			result[i] = f(i, current);
			current += elementsize;
			++i;
		}
	}

	while (i < size) {
		f(i, current);
		current += elementsize;
		++i;
	}

	return (void *)result;
}
