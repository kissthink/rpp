/*
可以把RPP当做伪代码写，
下面就是《算法导论》开篇的插入排序代码
（由于RPP数组从下标0开始而算法导论从1开始，故稍微修改了下）
*/

define ← =

void insertion_sort(rstr& a)
{
	for j ← 1 to a.count-1
		key ← a[j]
		i ← j-1
		while i>=0 && a.get(i)>key
			a[i+1] ← a[i]
			i ← i-1
		a[i+1] ← key
}

void main()
{
	rstr a="cab132"
	putsl a
	insertion_sort a
	putsl a
}