/*
本地调用
*/

main
{
#ifdef _RVM
	putsl cdecl["strlen","abc"]
	putsl cdecl_p[rf.find_dll("strlen"),"abc"]
	putsl cdecl["strcmp","abc","abc"]
	putsl cdecl["strcmp","abc","123"]
	putsl cdecl["strcmp","123","abc"]
#endif
#ifdef _JIT
	putsl mystrlen("abc")
	putsl mystrlen("abc")
	putsl mystrcmp("abc","abc")
	putsl mystrcmp("abc","123")
	putsl mystrcmp("123","abc")
#endif
	putsl rf.strlen("abc")
}

int mystrlen(char* s)
{
	p=rf.find_dll("strlen")
	push s
	call p
	mov s_ret,eax
	add esp,4
}

int mystrcmp(char* a,char* b)
{
	p=rf.find_dll("strcmp")
	push b
	push a
	call p
	mov s_ret,eax
	add esp,8
}